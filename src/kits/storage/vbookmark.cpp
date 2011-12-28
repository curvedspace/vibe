/****************************************************************************
 * This file is part of Vibe.
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
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Vibe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Vibe.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QtCore/QStack>
#include <QtCore/QDateTime>
#include <QtCore/QMimeData>
#include <QtCore/QStringList>
#include <QtCore/QDebug>

#include <VibeCore/VMimeType>
#include <VibeCore/VStringHandler>

#include "vbookmark.h"

#define METADATA_VISION_OWNER "http://www.vision-os.org"
#define METADATA_FREEDESKTOP_OWNER "http://freedesktop.org"
#define METADATA_MIME_OWNER "http://www.freedesktop.org/standards/shared-mime-info"

#define VISION_URI_LIST_MIMETYPE "application/x-vision-urilist"

/*
 * Utility functions.
 */

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

static QDomNode findMetadata(const QString &forOwner, QDomNode &parent, bool create)
{
    bool forOwnerIsVision = forOwner == METADATA_VISION_OWNER;

    QDomElement metadataElement;
    for (QDomNode _node = parent.firstChild(); !_node.isNull(); _node = _node.nextSibling()) {
        QDomElement elem = _node.toElement();
        if (!elem.isNull() && elem.tagName() == "metadata") {
            const QString owner = elem.attribute("owner");
            if (owner == forOwner)
                return elem;
            if (owner.isEmpty() && forOwnerIsVision)
                metadataElement = elem;
        }
    }
    if (create && metadataElement.isNull()) {
        metadataElement = parent.ownerDocument().createElement("metadata");
        parent.appendChild(metadataElement);
        metadataElement.setAttribute("owner", forOwner);

    } else if (!metadataElement.isNull() && forOwnerIsVision) {
        // XXX: I'm not sure if this is good, we shouln't take over foreign metatdata
        metadataElement.setAttribute("owner", METADATA_VISION_OWNER);
    }

    return metadataElement;
}

/*
 * VBookmark
 */

VBookmark::VBookmark()
{
}

VBookmark::VBookmark(const QDomElement &elem) :
    element(elem)
{
}

bool VBookmark::isGroup() const
{
    QString tag = element.tagName();
    return (tag == "folder"
            || tag == "xbel"); // don't forget the toplevel group
}

bool VBookmark::isSeparator() const
{
    return (element.tagName() == "separator");
}

bool VBookmark::isNull() const
{
    return element.isNull();
}

bool VBookmark::hasParent() const
{
    QDomElement parent = element.parentNode().toElement();
    return !parent.isNull();
}

QString VBookmark::text() const
{
    return VStringHandler::elide(fullText(), Qt::ElideMiddle);
}

QString VBookmark::fullText() const
{
    if (isSeparator())
        return tr("--- separator ---");

    QString text = element.namedItem("title").toElement().text();
    text.replace('\n', ' ');
    return text;
}

void VBookmark::setFullText(const QString &fullText)
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

QUrl VBookmark::url() const
{
    return QUrl(element.attribute("href").toAscii()); // Decodes it from utf8
}

void VBookmark::setUrl(const QUrl &url)
{
    element.setAttribute("href", url.toString());
}

QString VBookmark::icon() const
{
    QDomNode metaDataNode = metaData(METADATA_FREEDESKTOP_OWNER, false);
    QDomElement iconElement = cd(metaDataNode, "bookmark:icon", false).toElement();

    QString icon = iconElement.attribute("name");

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
                    VMimeType mime(_mimeType);
                    return mime.iconName();
                }

                // Get icon from URL
                icon = VMimeType::iconNameForUrl(url());
            }
        }
    }

    return icon;
}

void VBookmark::setIcon(const QString &icon)
{
    QDomNode metaDataNode = metaData(METADATA_FREEDESKTOP_OWNER, true);
    QDomElement iconElement = cd_or_create(metaDataNode, "bookmark:icon").toElement();
    iconElement.setAttribute("name", icon);
}

QString VBookmark::description() const
{
    if (isSeparator())
        return QString();

    QString description = element.namedItem("desc").toElement().text();
    description.replace('\n', ' ');
    return description;
}

void VBookmark::setDescription(const QString &description)
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

QString VBookmark::mimeType() const
{
    QDomNode metaDataNode = metaData(METADATA_MIME_OWNER, false);
    QDomElement mimeTypeElement = cd(metaDataNode, "mime:mime-type", false).toElement();
    return mimeTypeElement.attribute("type");
}

void VBookmark::setMimeType(const QString &mimeType)
{
    QDomNode metaDataNode = metaData(METADATA_MIME_OWNER, true);
    QDomElement iconElement = cd_or_create(metaDataNode, "mime:mime-type").toElement();
    iconElement.setAttribute("type", mimeType);
}

bool VBookmark::showInToolbar() const
{
    if (element.hasAttribute("showintoolbar")) {
        bool show = element.attribute("showintoolbar") == "yes";
        const_cast<QDomElement *>(&element)->removeAttribute("showintoolbar");
        const_cast<VBookmark *>(this)->setShowInToolbar(show);
    }
    return  metaDataItem("showintoolbar") == "yes";
}


void VBookmark::setShowInToolbar(bool show)
{
    setMetaDataItem("showintoolbar", show ? "yes" : "no");
}

VBookmarkGroup VBookmark::parentGroup() const
{
    return VBookmarkGroup(element.parentNode().toElement());
}

VBookmarkGroup VBookmark::toGroup() const
{
    Q_ASSERT(isGroup());
    return VBookmarkGroup(element);
}

QString VBookmark::address() const
{
    if (element.tagName() == "xbel")
        return ""; // not QString() !
    else {
        if (element.parentNode().isNull()) {
            // Avoid an infinite loop
            Q_ASSERT(false);
            return "ERROR";
        }

        VBookmarkGroup group = parentGroup();
        QString parentAddress = group.address();
        int pos = group.indexOf(*this);
        Q_ASSERT(pos != -1);
        return parentAddress + '/' + QString::number(pos);
    }
}

int VBookmark::positionInParent() const
{
    return parentGroup().indexOf(*this);
}

QDomElement VBookmark::internalElement() const
{
    return element;
}

VBookmark VBookmark::standaloneVBookmark(const QString &text, const QUrl &url, const QString &icon)
{
    QDomDocument doc("xbel");
    QDomElement elem = doc.createElement("xbel");
    doc.appendChild(elem);
    VBookmarkGroup grp(elem);
    grp.addBookmark(text, url, icon);
    return grp.first();
}

QString VBookmark::commonParent(const QString &first, const QString &second)
{
    QString A = first;
    QString B = second;
    QString error("ERROR");
    if (A == error || B == error)
        return error;

    A += '/';
    B += '/';

    uint lastCommonSlash = 0;
    uint lastPos = A.length() < B.length() ? A.length() : B.length();
    for (uint i = 0; i < lastPos; ++i) {
        if (A[i] != B[i])
            return A.left(lastCommonSlash);
        if (A[i] == '/')
            lastCommonSlash = i;
    }
    return A.left(lastCommonSlash);
}

void VBookmark::updateAccessMetadata()
{
    qDebug() << Q_FUNC_INFO << " " << address() << " " << url().toString();

    // TODO: Also set time_modified
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
}

QString VBookmark::parentAddress(const QString &address)
{
    return address.left(address.lastIndexOf(QLatin1Char('/')));
}

uint VBookmark::positionInParent(const QString &address)
{
    return address.mid(address.lastIndexOf(QLatin1Char('/')) + 1).toInt();
}

QString VBookmark::previousAddress(const QString &address)
{
    uint pp = positionInParent(address);
    return pp > 0
           ? parentAddress(address) + QLatin1Char('/') + QString::number(pp - 1)
           : QString();
}

QString VBookmark::nextAddress(const QString &address)
{
    return parentAddress(address) + QLatin1Char('/') +
           QString::number(positionInParent(address) + 1);
}

QDomNode VBookmark::metaData(const QString &owner, bool create) const
{
    QDomNode infoNode = cd(internalElement(), "info", create);
    if (infoNode.isNull()) return QDomNode();
    return findMetadata(owner, infoNode , create);
}

QString VBookmark::metaDataItem(const QString &key) const
{
    QDomNode metaDataNode = metaData(METADATA_VISION_OWNER, false);
    for (QDomElement e = metaDataNode.firstChildElement(); !e.isNull(); e = e.nextSiblingElement()) {
        if (e.tagName() == key) {
            return e.text();
        }
    }
    return QString();
}

void VBookmark::setMetaDataItem(const QString &key, const QString &value, MetaDataOverwriteMode mode)
{
    QDomNode metaDataNode = metaData(METADATA_VISION_OWNER, true);
    QDomNode item = cd_or_create(metaDataNode, key);
    QDomText text = get_or_create_text(item);
    if (mode == DontOverwriteMetaData && !text.data().isEmpty()) {
        return;
    }

    text.setData(value);
}

bool VBookmark::operator==(const VBookmark &rhs) const
{
    return element == rhs.element;
}

void VBookmark::populateMimeData(QMimeData *mimeData) const
{
    VBookmarkList bookmarkList;
    bookmarkList.append(*this);
    bookmarkList.populateMimeData(mimeData);
}

/*
 * VBookmarkList
 */

VBookmarkList::VBookmarkList() :
    QList<VBookmark>()
{
}

void VBookmarkList::populateMimeData(QMimeData *mimeData) const
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

bool VBookmarkList::canDecode(const QMimeData *mimeData)
{
    return mimeData->hasFormat(QString::fromLatin1("application/x-xbel")) ||
           mimeData->hasFormat(QString::fromLatin1("text/uri-list")) ||
           mimeData->hasFormat(QString::fromLatin1(VISION_URI_LIST_MIMETYPE));
}

QStringList VBookmarkList::mimeDataTypes()
{
    return QStringList() << QString::fromLatin1("application/x-xbel")
           << QString::fromLatin1("text/uri-list")
           << QString::fromLatin1(VISION_URI_LIST_MIMETYPE);
}

VBookmarkList VBookmarkList::fromMimeData(const QMimeData *mimeData, QDomDocument &doc)
{
    VBookmarkList bookmarks;
    QByteArray payload = mimeData->data("application/x-xbel");
    if (!payload.isEmpty()) {
        doc.setContent(payload);
        QDomElement elem = doc.documentElement();
        const QDomNodeList children = elem.childNodes();
        for (int childno = 0; childno < children.count(); childno++)
            bookmarks.append(VBookmark(children.item(childno).toElement()));
        return bookmarks;
    }
    // TODO:
#if 0
    const QUrl::List urls = QUrl::List::fromMimeData(mimeData);
    if (!urls.isEmpty()) {
        QUrl::List::ConstIterator uit = urls.begin();
        QUrl::List::ConstIterator uEnd = urls.end();
        for (; uit != uEnd ; ++uit) {
            //qDebug() << "VBookmark::List::fromMimeData url=" << (*uit);
            bookmarks.append(VBookmark::standaloneVBookmark(
                                 (*uit).prettyUrl(), (*uit)));
        }
    }
#endif
    return bookmarks;
}

/*
 * VBookmarkGroup
 */

VBookmarkGroup::VBookmarkGroup() :
    VBookmark(QDomElement())
{
}

VBookmarkGroup::VBookmarkGroup(const QDomElement &elem) :
    VBookmark(elem)
{
}

bool VBookmarkGroup::isOpen() const
{
    return element.attribute("folded") == "no"; // default is: folded
}

VBookmark VBookmarkGroup::first() const
{
    return VBookmark(nextKnownTag(element.firstChildElement(), true));
}

VBookmark VBookmarkGroup::previous(const VBookmark &current) const
{
    return VBookmark(nextKnownTag(current.element.previousSiblingElement(), false));
}

VBookmark VBookmarkGroup::next(const VBookmark &current) const
{
    return VBookmark(nextKnownTag(current.element.nextSiblingElement(), true));
}

int VBookmarkGroup::indexOf(const VBookmark &child) const
{
    uint counter = 0;
    for (VBookmark bk = first(); !bk.isNull(); bk = next(bk), ++counter) {
        if (bk.element == child.element)
            return counter;
    }
    return -1;
}

QDomElement VBookmarkGroup::nextKnownTag(const QDomElement &start, bool goNext) const
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

VBookmarkGroup VBookmarkGroup::createNewFolder(const QString &text)
{
    if (isNull())
        return VBookmarkGroup();

    QDomDocument doc = element.ownerDocument();
    QDomElement groupElem = doc.createElement("folder");
    element.appendChild(groupElem);
    QDomElement textElem = doc.createElement("title");
    groupElem.appendChild(textElem);
    textElem.appendChild(doc.createTextNode(text));
    return VBookmarkGroup(groupElem);
}

VBookmark VBookmarkGroup::createNewSeparator()
{
    if (isNull())
        return VBookmark();

    QDomDocument doc = element.ownerDocument();
    Q_ASSERT(!doc.isNull());
    QDomElement sepElem = doc.createElement("separator");
    element.appendChild(sepElem);
    return VBookmark(sepElem);
}

bool VBookmarkGroup::moveBookmark(const VBookmark &item, const VBookmark &after)
{
    QDomNode n;

    if (!after.isNull())
        n = element.insertAfter(item.element, after.element);
    else { // first child
        if (element.firstChild().isNull()) // Empty element -> set as real first child
            n = element.insertBefore(item.element, QDomElement());

        // We have to skip everything up to the first valid child
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

VBookmark VBookmarkGroup::addBookmark(const VBookmark &bm)
{
    element.appendChild(bm.internalElement());
    return bm;
}

VBookmark VBookmarkGroup::addBookmark(const QString &text, const QUrl &url, const QString &icon)
{
    if (isNull())
        return VBookmark();

    QDomDocument doc = element.ownerDocument();
    QDomElement elem = doc.createElement("bookmark");
    elem.setAttribute("href", url.toString());

    QDomElement textElem = doc.createElement("title");
    elem.appendChild(textElem);
    textElem.appendChild(doc.createTextNode(text));

    VBookmark newBookmark = addBookmark(VBookmark(elem));

    // As icons are moved to metadata, we have to use the VBookmark API for this
    newBookmark.setIcon(icon.isEmpty() ? VMimeType::iconNameForUrl(url) : icon);
    return newBookmark;
}

void VBookmarkGroup::deleteBookmark(const VBookmark &bk)
{
    element.removeChild(bk.element);
}

bool VBookmarkGroup::isToolbarGroup() const
{
    return (element.attribute("toolbar") == "yes");
}

QDomElement VBookmarkGroup::findToolbar() const
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
                QDomElement result = VBookmarkGroup(e).findToolbar();
                if (!result.isNull())
                    return result;
            }
        }
    }
    return QDomElement();
}

QList<QUrl> VBookmarkGroup::groupUrlList() const
{
    QList<QUrl> urlList;
    for (VBookmark bm = first(); !bm.isNull(); bm = next(bm)) {
        if (bm.isSeparator() || bm.isGroup())
            continue;
        urlList << bm.url();
    }
    return urlList;
}

/*
 * VBookmarkGroupTraverser
 */

VBookmarkGroupTraverser::~VBookmarkGroupTraverser()
{
}

void VBookmarkGroupTraverser::traverse(const VBookmarkGroup &root)
{
    QStack<VBookmarkGroup> stack;
    stack.push(root);
    VBookmark bk = root.first();
    for (;;) {
        if (bk.isNull()) {
            if (stack.count() == 1) // only root is on the stack
                return;
            if (stack.count() > 0) {
                visitLeave(stack.top());
                bk = stack.pop();
            }
            bk = stack.top().next(bk);
        } else if (bk.isGroup()) {
            VBookmarkGroup gp = bk.toGroup();
            visitEnter(gp);
            bk = gp.first();
            stack.push(gp);
        } else {
            visit(bk);
            bk = stack.top().next(bk);
        }
    }
}

void VBookmarkGroupTraverser::visit(const VBookmark &)
{
}

void VBookmarkGroupTraverser::visitEnter(const VBookmarkGroup &)
{
}

void VBookmarkGroupTraverser::visitLeave(const VBookmarkGroup &)
{
}
