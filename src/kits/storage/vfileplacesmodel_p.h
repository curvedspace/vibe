/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
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

#ifndef VFILEPLACESMODEL_P_H
#define VFILEPLACESMODEL_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Vibe API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <VibeHardware/VPredicate>

class VBoomarkManager;

namespace VPrivate
{
    class FilePlacesItem;
}

class VFilePlacesModelPrivate
{
    Q_DECLARE_PUBLIC(VFilePlacesModel)
public:
    VFilePlacesModelPrivate(VFilePlacesModel *parent);
    ~VFilePlacesModelPrivate();

    VPrivate::FilePlacesItem *rootItem;
    VPrivate::FilePlacesItem *favoritesRootItem;
    VPrivate::FilePlacesItem *devicesRootItem;

    QList<VPrivate::FilePlacesItem *> favoriteItems;
    QList<VPrivate::FilePlacesItem *> deviceItems;

    QSet<QString> availableDevices;
    VPredicate predicate;

    QMap<QObject *, QPersistentModelIndex> setupInProgress;

    VBookmarkManager *bookmarkManager;

    VFilePlacesModel *const q_ptr;

    QList<VPrivate::FilePlacesItem *> loadFavoritesList();
    QList<VPrivate::FilePlacesItem *> loadDevicesList();
    void reloadList(const QModelIndex &parent,
                    QList<VPrivate::FilePlacesItem *> currentItems,
                    QList<VPrivate::FilePlacesItem *> &items);

    void _q_initDeviceList();
    void _q_deviceAdded(const QString &udi);
    void _q_deviceRemoved(const QString &udi);
    void _q_itemChanged(const QString &udi);
    void _q_reloadFavorites();
    void _q_reloadDevices();
    void _q_storageTeardownDone(VHardware::ErrorType error, QVariant errorData);
    void _q_storageSetupDone(VHardware::ErrorType error, QVariant errorData);
};

#endif // VFILEPLACESMODEL_P_H
