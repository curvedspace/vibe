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

#ifndef VFILEMANAGERNAVIGATIONMODEL_H
#define VFILEMANAGERNAVIGATIONMODEL_H

#include <QAbstractItemModel>

#include <VibeCore/VGlobal>

/** \addtogroup storage Storage Kit
 *  @{
 */

class VFileManagerNavigationModelPrivate;

/**
 * \class VFileManagerNavigationModel vfilemanagernavigationmodel.h <VFileManagerNavigationModel>
 *
 * \brief Model for file manager navigation bars.
 *
 * This class provides a model suitable for tree-based file manager
 * navigation bars, such as the one found in Tracker or open/save common
 * dialogs.
 *
 * \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VIBE_EXPORT VFileManagerNavigationModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(VFileManagerNavigationModel)
public:
    explicit VFileManagerNavigationModel(QObject *parent = 0);
    ~VFileManagerNavigationModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

private:
    Q_PRIVATE_SLOT(d_ptr, void _q_initDeviceList())
    Q_PRIVATE_SLOT(d_ptr, void _q_deviceAdded(const QString &))
    Q_PRIVATE_SLOT(d_ptr, void _q_deviceRemoved(const QString &))
    Q_PRIVATE_SLOT(d_ptr, void _q_itemChanged(const QString &))
    Q_PRIVATE_SLOT(d_ptr, void _q_reloadBookmarks())
#if 0
    Q_PRIVATE_SLOT(d_ptr, void _q_storageSetupDone(VHardware::ErrorType, QVariant))
    Q_PRIVATE_SLOT(d_ptr, void _q_storageTeardownDone(VHardware::ErrorType, QVariant))
#endif

    VFileManagerNavigationModelPrivate *const d_ptr;
};

/** @}*/

#endif // VFILEMANAGERNAVIGATIONMODEL_H
