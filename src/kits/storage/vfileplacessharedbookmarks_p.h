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

#ifndef VFILEPLACESSHAREDBOOKMARKS_P_H
#define VFILEPLACESSHAREDBOOKMARKS_P_H

#include <QtCore/QObject>

#include <VibeCore/VGlobal>

#include "vbookmarkmanager.h"

/**
 *  keeps the FilePlacesModel bookmarks and the shared bookmark spec
 *  shortcuts in sync
 */
class VIBE_EXPORT VFilePlacesSharedBookmarks : public QObject
{
    Q_OBJECT
public:
    VFilePlacesSharedBookmarks(VBookmarkManager *mgr);
    ~VFilePlacesSharedBookmarks() {
        /* delete m_sharedBookmarkManager; */
    }

private:
    bool integrateSharedBookmarks();
    bool exportSharedBookmarks();

    VBookmarkManager *m_placesBookmarkManager;
    VBookmarkManager *m_sharedBookmarkManager;

private Q_SLOTS:
    void slotSharedBookmarksChanged();
    void slotBookmarksChanged();
};

#endif // VFILEPLACESSHAREDBOOKMARKS_P_H
