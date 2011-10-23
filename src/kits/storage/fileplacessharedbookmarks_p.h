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

#ifndef QUBE_FILEPLACESSHAREDBOOKMARKS_P_H
#define QUBE_FILEPLACESSHAREDBOOKMARKS_P_H

#include <QtCore/QObject>

#include "bookmarkmanager.h"

namespace Qube
{
    namespace Storage
    {
        /**
         *  keeps the FilePlacesModel bookmarks and the shared bookmark spec
         *  shortcuts in sync
         */
        class FilePlacesSharedBookmarks : public QObject
        {
            Q_OBJECT
        public:
            FilePlacesSharedBookmarks(BookmarkManager * mgr);
            ~FilePlacesSharedBookmarks() {
                /* delete m_sharedBookmarkManager; */
            }

        private:
            bool integrateSharedBookmarks();
            bool exportSharedBookmarks();

            BookmarkManager *m_placesBookmarkManager;
            BookmarkManager *m_sharedBookmarkManager;

        private Q_SLOTS:
            void slotSharedBookmarksChanged();
            void slotBookmarksChanged();
        };
    }
}

#endif // QUBE_FILEPLACESSHARED_P_H
