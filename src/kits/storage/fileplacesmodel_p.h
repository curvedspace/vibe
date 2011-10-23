/****************************************************************************
 * This file is part of Qube.
 *
 * Copyright (c) 2007 Kevin Ottens
 * Copyright (c) 2007 David Faure
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	David Faure <faure@kde.org>
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *	Kevin Ottens <ervin@kde.org>
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

#ifndef QUBE_FILEPLACESMODEL_P_H
#define QUBE_FILEPLACESMODEL_P_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QSet>

#include <Qube/Hardware/Predicate>

#include "bookmarkmanager.h"

namespace Qube
{
    namespace Storage
    {
        class FilePlacesModel;

        class FilePlacesModelPrivate
        {
        public:
            FilePlacesModelPrivate(FilePlacesModel *self) :
                q(self),
                bookmarkManager(0),
                sharedBookmarks(0) {
            }

            ~FilePlacesModelPrivate() {
                delete sharedBookmarks;
                qDeleteAll(items);
            }

            FilePlacesModel *q;

            QList<FilePlacesItem*> items;
            QSet<QString> availableDevices;
            QMap<QObject*, QPersistentModelIndex> setupInProgress;

            Qube::Hardware::Predicate predicate;
            BookmarkManager *bookmarkManager;
            FilePlacesSharedBookmarks * sharedBookmarks;

            void reloadAndSignal();
            QList<FilePlacesItem *> loadBookmarkList();

            void _q_initDeviceList();
            void _q_deviceAdded(const QString &udi);
            void _q_deviceRemoved(const QString &udi);
            void _q_itemChanged(const QString &udi);
            void _q_reloadBookmarks();
            void _q_storageSetupDone(Qube::Hardware::ErrorType error, QVariant errorData);
            void _q_storageTeardownDone(Qube::Hardware::ErrorType error, QVariant errorData);
        };
    }
}

#endif // QUBE_FILEPLACESMODEL_P_H
