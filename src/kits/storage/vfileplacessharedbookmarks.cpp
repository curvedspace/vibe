/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2008 Norbert Frese
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *	Norbert Frese <nf2@scheinwelt.at>
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

#include <QtCore/QObject>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QMimeData>
#include <QtCore/QDebug>

#include "vfileplacessharedbookmarks_p.h"
#include "vbookmarkmanager.h"

/*
 * Utility functions.
 */

static bool compareBookmarks(const VBookmark &bookmark1, const VBookmark &bookmark2)
{
    return (bookmark1.url() == bookmark2.url() || bookmark1.text() == bookmark2.text());
}

static bool deepCompareDomNodes(const QDomNode &node1, const QDomNode &node2)
{

    // compare name and value
    if (node1.nodeName() != node2.nodeName() || node1.nodeValue() != node2.nodeValue())
        return false;

    // recursively compare children
    const QDomNodeList node1Children  = node1.childNodes();
    const QDomNodeList node2Children  = node2.childNodes();

    if (node1Children.count() != node2Children.count())
        return false;

    for (int i = 0; i < node1Children.count(); i++) {
        if (!deepCompareDomNodes(node1Children.at(i), node2Children.at(i)))
            return false;
    }
    return true;
}

/*
static QString nodeAsString(const QDomNode & node1)
{
    QString str;
    QTextStream ts( &str, QIODevice::WriteOnly );
    ts << node1;
    return str;
}
*/

static bool exactCompareBookmarks(const VBookmark &bookmark1, const VBookmark &bookmark2)
{
    //kDebug() << "excat comparing:\n" << nodeAsString(bookmark1.internalElement()) << "\nwith:\n" << nodeAsString(bookmark2.internalElement());
    return deepCompareDomNodes(bookmark1.internalElement(), bookmark2.internalElement());
}

static void cloneBookmarkContents(const VBookmark &target, const VBookmark &source)
{
    const QDomElement targetEl = target.internalElement();
    QDomNode parent = targetEl.parentNode();
    QDomNode clonedNode = source.internalElement().cloneNode(true);
    parent.replaceChild(clonedNode , targetEl);
}

static VBookmark cloneBookmark(const VBookmark &toClone)
{
    const QDomNode cloned = toClone.internalElement().cloneNode(true);
    return VBookmark(cloned.toElement());
}


static void emptyBookmarkGroup(VBookmarkGroup &root)
{
    VBookmark bookmark = root.first();
    while (!bookmark.isNull()) {
        VBookmark bookmarkToRemove = bookmark;
        bookmark = root.next(bookmark);
        root.deleteBookmark(bookmarkToRemove);
    }
}

static int bookmarkGroupSize(VBookmarkGroup &root)
{
    int count = 0;
    VBookmark bookmark = root.first();
    while (!bookmark.isNull()) {
        count++;
        bookmark = root.next(bookmark);
    }
    return count;
}

/*
 * VFilePlacesSharedBookmarks
 */

VFilePlacesSharedBookmarks::VFilePlacesSharedBookmarks(VBookmarkManager *mgr)
{
    m_placesBookmarkManager = mgr;

#ifdef GIGI
    const QString file = KStandardDirs().localxdgdatadir() + "/user-places.xbel";
#else
    const QString file("~/.local/share/user-places.xbel");
#endif
    m_sharedBookmarkManager = VBookmarkManager::managerForExternalFile(file);

    connect(m_sharedBookmarkManager, SIGNAL(changed(const QString &, const QString &)),
            this, SLOT(slotSharedBookmarksChanged()));
    connect(m_sharedBookmarkManager, SIGNAL(bookmarksChanged(const QString &)),
            this, SLOT(slotSharedBookmarksChanged()));

    connect(m_placesBookmarkManager, SIGNAL(changed(const QString &, const QString &)),
            this, SLOT(slotBookmarksChanged()));
    connect(m_placesBookmarkManager, SIGNAL(bookmarksChanged(const QString &)),
            this, SLOT(slotBookmarksChanged()));

    integrateSharedBookmarks();
}

bool VFilePlacesSharedBookmarks::integrateSharedBookmarks()
{
    VBookmarkGroup root = m_placesBookmarkManager->root();
    VBookmark bookmark = root.first();

    VBookmarkGroup sharedRoot = m_sharedBookmarkManager->root();
    VBookmark sharedBookmark = sharedRoot.first();

    bool dirty = false;

    while (!bookmark.isNull()) {
        //kDebug() << "importing" << bookmark.text();

        // skip over system items
        if (bookmark.metaDataItem("isSystemItem") == "true") {
            bookmark = root.next(bookmark);
            continue;
        }

        // do the bookmarks match?
        if (!sharedBookmark.isNull() && compareBookmarks(bookmark, sharedBookmark)) {
            //kDebug() << "excat comparing: targetbk:\n" << nodeAsString(bookmark.internalElement()) << "\nsourcbk:\n" << nodeAsString(sharedBookmark.internalElement());

            if (!exactCompareBookmarks(bookmark, sharedBookmark)) {
                VBookmark cloneTarget = bookmark;
                VBookmark cloneSource = sharedBookmark;

                sharedBookmark = sharedRoot.next(sharedBookmark);
                bookmark = root.next(bookmark);

                //kDebug() << "cloning" << cloneSource.text();
                //kDebug() << "cloning: target=\n" << nodeAsString(cloneTarget.internalElement()) << "\n source:\n" << nodeAsString(cloneSource.internalElement());

                cloneBookmarkContents(cloneTarget, cloneSource);
                dirty = true;
                continue;
            } else {
                //kDebug() << "keeping" << bookmark.text();
            }
            sharedBookmark = sharedRoot.next(sharedBookmark);
            bookmark = root.next(bookmark);
            continue;
        }

        // they don't match -> remove
        //kDebug() << "removing" << bookmark.text();
        VBookmark bookmarkToRemove = bookmark;
        bookmark = root.next(bookmark);
        root.deleteBookmark(bookmarkToRemove);

        dirty = true;
    }

    // append the remaining shared bookmarks
    while (!sharedBookmark.isNull()) {
        root.addBookmark(cloneBookmark(sharedBookmark));
        sharedBookmark = sharedRoot.next(sharedBookmark);
        dirty = true;
    }

    return dirty;
}

bool VFilePlacesSharedBookmarks::exportSharedBookmarks()
{
    VBookmarkGroup root = m_placesBookmarkManager->root();
    VBookmark bookmark = root.first();

    VBookmarkGroup sharedRoot = m_sharedBookmarkManager->root();
    VBookmark sharedBookmark = sharedRoot.first();

    bool dirty = false;

    // first check if they are the same
    int count = 0;
    while (!bookmark.isNull()) {
        //kDebug() << "exporting..." << bookmark.text();

        // skip over system items
        if (bookmark.metaDataItem("isSystemItem") == "true") {
            bookmark = root.next(bookmark);
            continue;
        }
        count++;

        // end of sharedBookmarks?
        if (sharedBookmark.isNull()) {
            dirty = true;
            break;
        }

        // do the bookmarks match?
        if (compareBookmarks(bookmark, sharedBookmark)) {
            if (!exactCompareBookmarks(bookmark, sharedBookmark)) {
                dirty = true;
                break;
            }
        } else {
            dirty = true;
            break;
        }
        sharedBookmark = sharedRoot.next(sharedBookmark);
        bookmark = root.next(bookmark);
    }

    //kDebug() << "dirty=" << dirty << " oldsize=" << bookmarkGroupSize(sharedRoot) << " count=" << count;

    if (bookmarkGroupSize(sharedRoot) != count)
        dirty = true;

    if (dirty) {
        emptyBookmarkGroup(sharedRoot);

        // append all bookmarks
        VBookmark bookmark = root.first();

        while (!bookmark.isNull()) {

            if (bookmark.metaDataItem("isSystemItem") == "true") {
                bookmark = root.next(bookmark);
                continue;
            }

            sharedRoot.addBookmark(cloneBookmark(bookmark));
            bookmark = root.next(bookmark);
            dirty = true;
        }
    }

    return dirty;

}

void VFilePlacesSharedBookmarks::slotSharedBookmarksChanged()
{
    //kDebug() << "shared bookmarks changed";
    bool dirty = integrateSharedBookmarks();
    if (dirty) m_placesBookmarkManager->emitChanged();
}

void VFilePlacesSharedBookmarks::slotBookmarksChanged()
{
    //kDebug() << "places bookmarks changed";
    bool dirty = exportSharedBookmarks();
    if (dirty) m_sharedBookmarkManager->emitChanged();
}

#include "vfileplacessharedbookmarks_p.moc"
