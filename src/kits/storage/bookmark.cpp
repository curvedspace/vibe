/****************************************************************************
 * This file is part of Qube.
 *
 * Copyright (c) 2000-2005 David Faure
 * Copyright (c) 2003 Alexander Kellett
 * Copyright (c) 2008 Norbert Frese
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	David Faure <faure@kde.org>
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *	Norbert Frese <nf2@scheinwelt.at>
 *	Alexander Kellett <lypanov@kde.org>
 *
 * Qube is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Qube is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Qube.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QtCore/QStack>
#include <QtCore/QDateTime>
#include <QtCore/QMimeData>
#include <QtCore/QStringList>
#include <QtCore/QDebug>

#include <Qube/Core/MimeType>
#include <Qube/Core/StringHandler>

#include "bookmark.h"

#define METADATA_KDE_OWNER "http://www.kde.org"
#define METADATA_FREEDESKTOP_OWNER "http://freedesktop.org"
#define METADATA_MIME_OWNER "http://www.freedesktop.org/standards/shared-mime-info"

#define QUBE_URI_LIST_MIMETYPE "application/x-vision-urilist"

////// utility functions

static QDomNode cd(QDomNode node, const QString &name, bool create)
{
    QDomNode subnode = node.namedItem(name);
    if (create && subnode.isNull()) {
        subnode = node.ownerDocument().createElement(name);
        node.appendChild(subnode);
    }
    return subnode;
}

static QDomNode cd_or_create(QDomNode node, const QString &name)
{
    return cd(node, name, true);
}

static QDomText get_or_create_text(QDomNode node)
{
    QDomNode subnode = node.firstChild();
    if (subnode.isNull()) {
        subnode = node.ownerDocument().createTextNode("");
        node.appendChild(subnode);
    }
    return subnode.toText();
}

static QDomNode findMetadata(const QString & forOwner, QDomNode& parent, bool create)
{
    bool forOwnerIsKDE = forOwner == METADATA_KDE_OWNER;

    QDomElement metadataElement;
    for (QDomNode _node = parent.firstChild(); !_node.isNull(); _node = _node.nextSibling()) {
        QDomElement elem = _node.toElement();
        if (!elem.isNull() && elem.tagName() == "metadata") {
            const QString owner = elem.attribute("owner");
            if (owner == forOwner)
                return elem;
            if (owner.isEmpty() && forOwnerIsKDE)
                metadataElement = elem;
        }
    }
    if (create && metadataElement.isNull()) {
        metadataElement = parent.ownerDocument().createElement("metadata");
        parent.appendChild(metadataElement);
        metadataElement.setAttribute("owner", forOwner);

    } else if (!metadataElement.isNull() && forOwnerIsKDE) {
        // i'm not sure if this is good, we shouln't take over foreign metatdata
        metadataElement.setAttribute("owner", METADATA_KDE_OWNER);
    }
    return metadataElement;
}

namespace Qube
{
    namespace Storage
    {
        /*
         * BookmarkGroup
         */

        BookmarkGroup::BookmarkGroup()
            : Bookmark(QDomElement())
        {
        }

        BookmarkGroup::BookmarkGroup(const QDomElement &elem)
            : Bookmark(elem)
        {
        }

        bool BookmarkGroup::isOpen() const
        {
            return element.attribute("folded") == "no"; // default is: folded
        }

        Bookmark BookmarkGroup::first() const
        {
            return Bookmark(nextKnownTag(element.firstChildElement(), true));
        }

        Bookmark BookmarkGroup::previous(const Bookmark & current) const
        {
            return Bookmark(nextKnownTag(current.element.previousSiblingElement(), false));
        }

        Bookmark BookmarkGroup::next(const Bookmark & current) const
        {
            return Bookmark(nextKnownTag(current.element.nextSiblingElement(), true));
        }

        int BookmarkGroup::indexOf(const Bookmark& child) const
        {
            uint counter = 0;
            for (Bookmark bk = first(); !bk.isNull(); bk = next(bk), ++counter) {
                if (bk.element == child.element)
                    return counter;
            }
            return -1;
        }

        QDomElement BookmarkGroup::nextKnownTag(const QDomElement &start, bool goNext) const
        {
            static const QString &bookmark("bookmark");
            static const QString &folder("folder");
            static const QString &separator("separator");

            for (QDomElement elem = start; !elem.isNull();) {
                QString tag = elem.tagName();
                if (tag == folder || tag == bookmark || tag == separator)
                    return elem;
                if (goNext)
                    elem = elem.nextSiblingElement();
                else
                    elem = elem.previousSiblingElement();
            }
            return QDomElement();
        }

        BookmarkGroup BookmarkGroup::createNewFolder(const QString & text)
        {
            if (isNull())
                return BookmarkGroup();

            QDomDocument doc = element.ownerDocument();
            QDomElement groupElem = doc.createElement("folder");
            element.appendChild(groupElem);
            QDomElement textElem = doc.createElement("title");
            groupElem.appendChild(textElem);
            textElem.appendChild(doc.createTextNode(text));
            return BookmarkGroup(groupElem);
        }

        Bookmark BookmarkGroup::createNewSeparator()
        {
            if (isNull())
                return Bookmark();

            QDomDocument doc = element.ownerDocument();
            Q_ASSERT(!doc.isNull());
            QDomElement sepElem = doc.createElement("separator");
            element.appendChild(sepElem);
            return Bookmark(sepElem);
        }

        bool BookmarkGroup::moveBookmark(const Bookmark & item, const Bookmark & after)
        {
            QDomNode n;

            if (!after.isNull())
                n = element.insertAfter(item.element, after.element);
            else { // first child
                if (element.firstChild().isNull()) // Empty element -> set as real first child
                    n = element.insertBefore(item.element, QDomElement());

                // we have to skip everything up to the first valid child
                QDomElement firstChild = nextKnownTag(element.firstChild().toElement(), true);
                if (!firstChild.isNull())
                    n = element.insertBefore(item.element, firstChild);
                else {
                    // No real first child -> append after the <title> etc.
                    n = element.appendChild(item.element);
                }
            }

            return (!n.isNull());
        }

        Bookmark BookmarkGroup::addBookmark(const Bookmark &bm)
        {
            element.appendChild(bm.internalElement());
            return bm;
        }

        Bookmark BookmarkGroup::addBookmark(const QString &text, const QUrl &url, const QString &icon)
        {
            if (isNull())
                return Bookmark();

            QDomDocument doc = element.ownerDocument();
            QDomElement elem = doc.createElement("bookmark");
            elem.setAttribute("href", url.toString());

            QDomElement textElem = doc.createElement("title");
            elem.appendChild(textElem);
            textElem.appendChild(doc.createTextNode(text));

            Bookmark newBookmark = addBookmark(Bookmark(elem));

            // as icons are moved to metadata, we have to use the Bookmark API for this
            newBookmark.setIcon(icon.isEmpty() ? Qube::Core::MimeType::iconNameForUrl(url) : icon);
            return newBookmark;
        }

        void BookmarkGroup::deleteBookmark(const Bookmark &bk)
        {
            element.removeChild(bk.element);
        }

        bool BookmarkGroup::isToolbarGroup() const
        {
            return (element.attribute("toolbar") == "yes");
        }

        QDomElement BookmarkGroup::findToolbar() const
        {
            if (element.attribute("toolbar") == "yes")
                return element;
            for (QDomNode n = element.firstChild(); !n.isNull() ; n = n.nextSibling()) {
                QDomElement e = n.toElement();
                // Search among the "folder" children only
                if (e.tagName() == "folder") {
                    if (e.attribute("toolbar") == "yes")
                        return e;
                    else {
                        QDomElement result = BookmarkGroup(e).findToolbar();
                        if (!result.isNull())
                            return result;
                    }
                }
            }
            return QDomElement();
        }

        QList<QUrl> BookmarkGroup::groupUrlList() const
        {
            QList<QUrl> urlList;
            for (Bookmark bm = first(); !bm.isNull(); bm = next(bm)) {
                if (bm.isSeparator() || bm.isGroup())
                    continue;
                urlList << bm.url();
            }
            return urlList;
        }

        /*
         * Bookmark
         */

        Bookmark::Bookmark()
        {
        }

        Bookmark::Bookmark(const QDomElement &elem) : element(elem)
        {
        }

        bool Bookmark::isGroup() const
        {
            QString tag = element.tagName();
            return (tag == "folder"
                    || tag == "xbel"); // don't forget the toplevel group
        }

        bool Bookmark::isSeparator() const
        {
            return (element.tagName() == "separator");
        }

        bool Bookmark::isNull() const
        {
            return element.isNull();
        }

        bool Bookmark::hasParent() const
        {
            QDomElement parent = element.parentNode().toElement();
            return !parent.isNull();
        }

        QString Bookmark::text() const
        {
            return Qube::Core::StringHandler::elide(fullText(), Qt::ElideMiddle);
        }

        QString Bookmark::fullText() const
        {
            if (isSeparator())
                return tr("--- separator ---");

            QString text = element.namedItem("title").toElement().text();
            text.replace('\n', ' '); // #140673
            return text;
        }

        void Bookmark::setFullText(const QString &fullText)
        {
            QDomNode titleNode = element.namedItem("title");
            if (titleNode.isNull()) {
                titleNode = element.ownerDocument().createElement("title");
                element.appendChild(titleNode);
            }

            if (titleNode.firstChild().isNull()) {
                QDomText domtext = titleNode.ownerDocument().createTextNode("");
                titleNode.appendChild(domtext);
            }

            QDomText domtext = titleNode.firstChild().toText();
            domtext.setData(fullText);
        }

        QUrl Bookmark::url() const
        {
            return QUrl(element.attribute("href").toAscii()); // Decodes it from utf8
        }

        void Bookmark::setUrl(const QUrl &url)
        {
            element.setAttribute("href", url.toString());
        }

        QString Bookmark::icon() const
        {
            QDomNode metaDataNode = metaData(METADATA_FREEDESKTOP_OWNER, false);
            QDomElement iconElement = cd(metaDataNode, "bookmark:icon", false).toElement();

            QString icon = iconElement.attribute("name");

            // migration code
            if (icon.isEmpty())
                icon = element.attribute("icon");
            if (icon == "www") // common icon for kde3 bookmarks
                return "internet-web-browser";
            // end migration code

            if (icon == "bookmark_folder") {
                return "folder-bookmarks";
            }
            if (icon.isEmpty()) {
                // Default icon depends on URL for bookmarks, and is default directory
                // icon for groups.
                if (isGroup()) {
                    icon = "folder-bookmarks";
                } else {
                    if (isSeparator()) {
                        icon = "edit-clear"; // whatever
                    } else {
                        // Get icon from mimeType
                        QString _mimeType = mimeType();
                        if (!_mimeType.isEmpty()) {
                            Qube::Core::MimeType mime(_mimeType);
                            return mime.iconName();
                        }

                        // Get icon from URL
                        icon = Qube::Core::MimeType::iconNameForUrl(url());
                    }
                }
            }
            return icon;
        }

        void Bookmark::setIcon(const QString &icon)
        {
            QDomNode metaDataNode = metaData(METADATA_FREEDESKTOP_OWNER, true);
            QDomElement iconElement = cd_or_create(metaDataNode, "bookmark:icon").toElement();
            iconElement.setAttribute("name", icon);

            // migration code
            if(!element.attribute("icon").isEmpty())
                element.removeAttribute("icon");
        }

        QString Bookmark::description() const
        {
            if (isSeparator())
                return QString();

            QString description = element.namedItem("desc").toElement().text();
            description.replace('\n', ' '); // #140673
            return description;
        }

        void Bookmark::setDescription(const QString &description)
        {
            QDomNode descNode = element.namedItem("desc");
            if (descNode.isNull()) {
                descNode = element.ownerDocument().createElement("desc");
                element.appendChild(descNode);
            }

            if (descNode.firstChild().isNull()) {
                QDomText domtext = descNode.ownerDocument().createTextNode(QString());
                descNode.appendChild(domtext);
            }

            QDomText domtext = descNode.firstChild().toText();
            domtext.setData(description);
        }

        QString Bookmark::mimeType() const
        {
            QDomNode metaDataNode = metaData(METADATA_MIME_OWNER, false);
            QDomElement mimeTypeElement = cd(metaDataNode, "mime:mime-type", false).toElement();
            return mimeTypeElement.attribute("type");
        }

        void Bookmark::setMimeType(const QString &mimeType)
        {
            QDomNode metaDataNode = metaData(METADATA_MIME_OWNER, true);
            QDomElement iconElement = cd_or_create(metaDataNode, "mime:mime-type").toElement();
            iconElement.setAttribute ("type", mimeType);
        }

        bool Bookmark::showInToolbar() const
        {
            if(element.hasAttribute("showintoolbar")) {
                bool show = element.attribute("showintoolbar") == "yes";
                const_cast<QDomElement *>(&element)->removeAttribute("showintoolbar");
                const_cast<Bookmark *>(this)->setShowInToolbar(show);
            }
            return  metaDataItem("showintoolbar") == "yes";
        }


        void Bookmark::setShowInToolbar(bool show)
        {
            setMetaDataItem("showintoolbar", show ? "yes" : "no");
        }

        BookmarkGroup Bookmark::parentGroup() const
        {
            return BookmarkGroup(element.parentNode().toElement());
        }

        BookmarkGroup Bookmark::toGroup() const
        {
            Q_ASSERT(isGroup());
            return BookmarkGroup(element);
        }

        QString Bookmark::address() const
        {
            if (element.tagName() == "xbel")
                return ""; // not QString() !
            else {
                // Use keditbookmarks's DEBUG_ADDRESSES flag to debug this code :)
                if (element.parentNode().isNull()) {
                    Q_ASSERT(false);
                    return "ERROR"; // Avoid an infinite loop
                }
                BookmarkGroup group = parentGroup();
                QString parentAddress = group.address();
                int pos = group.indexOf(*this);
                Q_ASSERT(pos != -1);
                return parentAddress + '/' + QString::number(pos);
            }
        }

        int Bookmark::positionInParent() const
        {
            return parentGroup().indexOf(*this);
        }

        QDomElement Bookmark::internalElement() const
        {
            return element;
        }

        Bookmark Bookmark::standaloneBookmark(const QString & text, const QUrl & url, const QString & icon)
        {
            QDomDocument doc("xbel");
            QDomElement elem = doc.createElement("xbel");
            doc.appendChild(elem);
            BookmarkGroup grp(elem);
            grp.addBookmark(text, url, icon);
            return grp.first();
        }


        QString Bookmark::commonParent(const QString &first, const QString &second)
        {
            QString A = first;
            QString B = second;
            QString error("ERROR");
            if(A == error || B == error)
                return error;

            A += '/';
            B += '/';

            uint lastCommonSlash = 0;
            uint lastPos = A.length() < B.length() ? A.length() : B.length();
            for(uint i=0; i < lastPos; ++i) {
                if(A[i] != B[i])
                    return A.left(lastCommonSlash);
                if(A[i] == '/')
                    lastCommonSlash = i;
            }
            return A.left(lastCommonSlash);
        }

        void Bookmark::updateAccessMetadata()
        {
            qDebug() << "Qube::Storage::Bookmark::updateAccessMetadata " << address() << " " << url().toString();

            const uint timet = QDateTime::currentDateTime().toTime_t();
            setMetaDataItem("time_added", QString::number(timet), DontOverwriteMetaData);
            setMetaDataItem("time_visited", QString::number(timet));

            QString countStr = metaDataItem("visit_count"); // TODO use spec'ed name
            bool ok;
            int currentCount = countStr.toInt(&ok);
            if (!ok)
                currentCount = 0;
            currentCount++;
            setMetaDataItem("visit_count", QString::number(currentCount));

            // TODO - for 4.0 - time_modified
        }

        QString Bookmark::parentAddress(const QString & address)
        {
            return address.left(address.lastIndexOf(QLatin1Char('/')));
        }

        uint Bookmark::positionInParent(const QString & address)
        {
            return address.mid(address.lastIndexOf(QLatin1Char('/')) + 1).toInt();
        }

        QString Bookmark::previousAddress(const QString & address)
        {
            uint pp = positionInParent(address);
            return pp>0
                   ? parentAddress(address) + QLatin1Char('/') + QString::number(pp-1)
                   : QString();
        }

        QString Bookmark::nextAddress(const QString & address)
        {
            return parentAddress(address) + QLatin1Char('/') +
                   QString::number(positionInParent(address)+1);
        }

        QDomNode Bookmark::metaData(const QString &owner, bool create) const
        {
            QDomNode infoNode = cd(internalElement(), "info", create);
            if (infoNode.isNull()) return QDomNode();
            return findMetadata(owner, infoNode , create);
        }

        QString Bookmark::metaDataItem(const QString &key) const
        {
            QDomNode metaDataNode = metaData(METADATA_KDE_OWNER, false);
            for (QDomElement e = metaDataNode.firstChildElement(); !e.isNull(); e = e.nextSiblingElement()) {
                if (e.tagName() == key) {
                    return e.text();
                }
            }
            return QString();
        }

        void Bookmark::setMetaDataItem(const QString &key, const QString &value, MetaDataOverwriteMode mode)
        {
            QDomNode metaDataNode = metaData(METADATA_KDE_OWNER, true);
            QDomNode item = cd_or_create(metaDataNode, key);
            QDomText text = get_or_create_text(item);
            if (mode == DontOverwriteMetaData && !text.data().isEmpty()) {
                return;
            }

            text.setData(value);
        }


        bool Bookmark::operator==(const Bookmark& rhs) const
        {
            return element == rhs.element;
        }

////

        BookmarkGroupTraverser::~BookmarkGroupTraverser()
        {
        }

        void BookmarkGroupTraverser::traverse(const BookmarkGroup &root)
        {
            QStack<BookmarkGroup> stack;
            stack.push(root);
            Bookmark bk = root.first();
            for(;;) {
                if(bk.isNull()) {
                    if(stack.count() == 1) // only root is on the stack
                        return;
                    if(stack.count() > 0) {
                        visitLeave(stack.top());
                        bk = stack.pop();
                    }
                    bk = stack.top().next(bk);
                } else if(bk.isGroup()) {
                    BookmarkGroup gp = bk.toGroup();
                    visitEnter(gp);
                    bk = gp.first();
                    stack.push(gp);
                } else {
                    visit(bk);
                    bk = stack.top().next(bk);
                }
            }
        }

        void BookmarkGroupTraverser::visit(const Bookmark &)
        {
        }

        void BookmarkGroupTraverser::visitEnter(const BookmarkGroup &)
        {
        }

        void BookmarkGroupTraverser::visitLeave(const BookmarkGroup &)
        {
        }

        void Bookmark::populateMimeData(QMimeData* mimeData) const
        {
            Bookmark::List bookmarkList;
            bookmarkList.append(*this);
            bookmarkList.populateMimeData(mimeData);
        }

        Bookmark::List::List() :
            QList<Bookmark>()
        {
        }

        void Bookmark::List::populateMimeData(QMimeData* mimeData) const
        {
            QList<QUrl> urls;

            QDomDocument doc("xbel");
            QDomElement elem = doc.createElement("xbel");
            doc.appendChild(elem);

            for (const_iterator it = begin(), end = this->end() ; it != end ; ++it) {
                urls.append((*it).url());
                elem.appendChild((*it).internalElement().cloneNode(true /* deep */));
            }

// TODO:
#if 0
            // This sets text/uri-list and text/plain into the mimedata
            urls.populateMimeData(mimeData, QUrl::MetaDataMap());
#endif

            mimeData->setData("application/x-xbel", doc.toByteArray());
        }

        bool Bookmark::List::canDecode(const QMimeData *mimeData)
        {
            return mimeData->hasFormat(QString::fromLatin1("application/x-xbel")) ||
                   mimeData->hasFormat(QString::fromLatin1("text/uri-list")) ||
                   mimeData->hasFormat(QString::fromLatin1(QUBE_URI_LIST_MIMETYPE));
        }

        QStringList Bookmark::List::mimeDataTypes()
        {
            return QStringList() << QString::fromLatin1("application/x-xbel")
                   << QString::fromLatin1("text/uri-list")
                   << QString::fromLatin1(QUBE_URI_LIST_MIMETYPE);
        }

        Bookmark::List Bookmark::List::fromMimeData(const QMimeData *mimeData, QDomDocument& doc)
        {
            Bookmark::List bookmarks;
            QByteArray payload = mimeData->data("application/x-xbel");
            if (!payload.isEmpty()) {
                doc.setContent(payload);
                QDomElement elem = doc.documentElement();
                const QDomNodeList children = elem.childNodes();
                for (int childno = 0; childno < children.count(); childno++)
                    bookmarks.append(Bookmark(children.item(childno).toElement()));
                return bookmarks;
            }
// TODO:
#if 0
            const QUrl::List urls = QUrl::List::fromMimeData(mimeData);
            if (!urls.isEmpty()) {
                QUrl::List::ConstIterator uit = urls.begin();
                QUrl::List::ConstIterator uEnd = urls.end();
                for (; uit != uEnd ; ++uit) {
                    //qDebug() << "Qube::Storage::Bookmark::List::fromMimeData url=" << (*uit);
                    bookmarks.append(Bookmark::standaloneBookmark(
                                         (*uit).prettyUrl(), (*uit)));
                }
            }
#endif
            return bookmarks;
        }
    }
}
