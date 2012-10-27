/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2007 Kevin Ottens
 * Copyright (c) 2007 David Faure
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    David Faure <faure@kde.org>
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Kevin Ottens <ervin@kde.org>
 *
 * $BEGIN_LICENSE:LGPL2$
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * $END_LICENSE$
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

#include <solid/predicate.h>

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
    Solid::Predicate predicate;

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
    void _q_storageTeardownDone(Solid::ErrorType error, QVariant errorData);
    void _q_storageSetupDone(Solid::ErrorType error, QVariant errorData);
};

#endif // VFILEPLACESMODEL_P_H
