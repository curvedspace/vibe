/****************************************************************************
 * This file is part of Qube.
 *
 * Copyright (c) 2008 Norbert Frese
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *	Norbert Frese <nf2@scheinwelt.at>
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

#include <QtCore/QObject>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QMimeData>
#include <QtCore/QDebug>

#include "fileplacessharedbookmarks_p.h"
#include "bookmarkmanager.h"

namespace Qube
{
    namespace Storage
    {
//////////////// utility functions

        static bool compareBookmarks(const Bookmark & bookmark1, const Bookmark & bookmark2)
        {
            return (bookmark1.url() == bookmark2.url() || bookmark1.text() == bookmark2.text());
        }

        static bool deepCompareDomNodes(const QDomNode & node1, const QDomNode & node2)
        {

            // compare name and value
            if (node1.nodeName() != node2.nodeName() || node1.nodeValue() != node2.nodeValue())
                return false;

            // recursively compare children
            const QDomNodeList node1Children  = node1.childNodes();
            const QDomNodeList node2Children  = node2.childNodes();

            if (node1Children.count () != node2Children.count ())
                return false;

            for (int i=0; i<node1Children.count (); i++) {
                if (!deepCompareDomNodes(node1Children.at(i), node2Children.at(i) ))
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

        static bool exactCompareBookmarks(const Bookmark & bookmark1, const Bookmark & bookmark2)
        {
            //kDebug() << "excat comparing:\n" << nodeAsString(bookmark1.internalElement()) << "\nwith:\n" << nodeAsString(bookmark2.internalElement());
            return deepCompareDomNodes(bookmark1.internalElement(), bookmark2.internalElement());
        }

        static void cloneBookmarkContents(const Bookmark & target, const Bookmark & source)
        {
            const QDomElement targetEl = target.internalElement();
            QDomNode parent = targetEl.parentNode ();
            QDomNode clonedNode = source.internalElement().cloneNode(true);
            parent.replaceChild (clonedNode , targetEl );
        }

        static Bookmark cloneBookmark(const Bookmark & toClone)
        {
            const QDomNode cloned = toClone.internalElement().cloneNode(true);
            return Bookmark(cloned.toElement ());
        }


        static void emptyBookmarkGroup(BookmarkGroup & root)
        {
            Bookmark bookmark = root.first();
            while (!bookmark.isNull()) {
                Bookmark bookmarkToRemove = bookmark;
                bookmark = root.next(bookmark);
                root.deleteBookmark(bookmarkToRemove);
            }
        }

        static int bookmarkGroupSize(BookmarkGroup & root)
        {
            int count=0;
            Bookmark bookmark = root.first();
            while (!bookmark.isNull()) {
                count++;
                bookmark = root.next(bookmark);
            }
            return count;
        }

//////////////// class FilePlacesSharedBookmarks

        FilePlacesSharedBookmarks::FilePlacesSharedBookmarks(BookmarkManager * mgr)
        {
            m_placesBookmarkManager = mgr;

#ifdef GIGI
            const QString file = KStandardDirs().localxdgdatadir() + "/user-places.xbel";
#else
            const QString file("~/.local/share/user-places.xbel");
#endif
            m_sharedBookmarkManager = BookmarkManager::managerForExternalFile(file);

            connect(m_sharedBookmarkManager, SIGNAL(changed(const QString&, const QString&)),
                    this, SLOT(slotSharedBookmarksChanged()));
            connect(m_sharedBookmarkManager, SIGNAL(bookmarksChanged(const QString&)),
                    this, SLOT(slotSharedBookmarksChanged()));

            connect(m_placesBookmarkManager, SIGNAL(changed(const QString&, const QString&)),
                    this, SLOT(slotBookmarksChanged()));
            connect(m_placesBookmarkManager, SIGNAL(bookmarksChanged(const QString&)),
                    this, SLOT(slotBookmarksChanged()));

            integrateSharedBookmarks();
        }

        bool FilePlacesSharedBookmarks::integrateSharedBookmarks()
        {
            BookmarkGroup root = m_placesBookmarkManager->root();
            Bookmark bookmark = root.first();

            BookmarkGroup sharedRoot = m_sharedBookmarkManager->root();
            Bookmark sharedBookmark = sharedRoot.first();

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
                        Bookmark cloneTarget=bookmark;
                        Bookmark cloneSource = sharedBookmark;

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
                Bookmark bookmarkToRemove = bookmark;
                bookmark = root.next(bookmark);
                root.deleteBookmark(bookmarkToRemove);

                dirty = true;
            }

            // append the remaining shared bookmarks
            while(!sharedBookmark.isNull()) {
                root.addBookmark(cloneBookmark(sharedBookmark));
                sharedBookmark = sharedRoot.next(sharedBookmark);
                dirty = true;
            }

            return dirty;
        }

        bool FilePlacesSharedBookmarks::exportSharedBookmarks()
        {
            BookmarkGroup root = m_placesBookmarkManager->root();
            Bookmark bookmark = root.first();

            BookmarkGroup sharedRoot = m_sharedBookmarkManager->root();
            Bookmark sharedBookmark = sharedRoot.first();

            bool dirty = false;

            // first check if they are the same
            int count=0;
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
                    dirty=true;
                    break;
                }

                // do the bookmarks match?
                if (compareBookmarks(bookmark, sharedBookmark)) {
                    if (!exactCompareBookmarks(bookmark, sharedBookmark)) {
                        dirty = true;
                        break;
                    }
                } else {
                    dirty=true;
                    break;
                }
                sharedBookmark = sharedRoot.next(sharedBookmark);
                bookmark = root.next(bookmark);
            }

            //kDebug() << "dirty=" << dirty << " oldsize=" << bookmarkGroupSize(sharedRoot) << " count=" << count;

            if (bookmarkGroupSize(sharedRoot) != count)
                dirty=true;

            if (dirty) {
                emptyBookmarkGroup(sharedRoot);

                // append all bookmarks
                Bookmark bookmark = root.first();

                while(!bookmark.isNull()) {

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

        void FilePlacesSharedBookmarks::slotSharedBookmarksChanged()
        {
            //kDebug() << "shared bookmarks changed";
            bool dirty = integrateSharedBookmarks();
            if (dirty) m_placesBookmarkManager->emitChanged();
        }

        void FilePlacesSharedBookmarks::slotBookmarksChanged()
        {
            //kDebug() << "places bookmarks changed";
            bool dirty = exportSharedBookmarks();
            if (dirty) m_sharedBookmarkManager->emitChanged();
        }
    }
}

#include "fileplacessharedbookmarks_p.moc"
