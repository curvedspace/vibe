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

#ifndef VFILEPLACESMODEL_H
#define VFILEPLACESMODEL_H

#include <QAbstractItemModel>

#include <VibeCore/VGlobal>

/** \addtogroup storage Storage Kit
 *  @{
 */

class VBookmark;
class VFilePlacesModelPrivate;

/**
 * \class VFilePlacesModel vfileplacesmodel.h <VFilePlacesModel>
 *
 * \brief Model for file manager navigation bars.
 *
 * This class provides a model suitable for tree-based file manager
 * navigation bars, such as the one found in Tracker or open/save common
 * dialogs.
 *
 * It shows bookmark items under the Favorites top-level item and devices,
 * such as optical discs and volumes, under the Devices top-level item.
 *
 * \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VIBE_EXPORT VFilePlacesModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(VFilePlacesModel)
    Q_ENUMS(AdditionalRoles)
public:
    enum AdditionalRoles {
        UrlRole = Qt::UserRole + 1,
        HiddenRole = Qt::UserRole + 2,
        SetupNeededRole = Qt::UserRole + 3,
        FixedDeviceRole = Qt::UserRole + 4,
        CapacityBarReccomendedRole = Qt::UserRole + 5
    };

    explicit VFilePlacesModel(QObject *parent = 0);
    ~VFilePlacesModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    /**
     * @param index the index which contains the row to fetch the icon.
     * @returns the icon for the given index.
     */
    QIcon icon(const QModelIndex &index) const;

    /**
     * @param index the index which contains the row to fetch the text.
     * @returns the text for the given index.
     */
    QString text(const QModelIndex &index) const;

    /**
     * @param index the index which contains the row to fetch the URL.
     * @returns the URL for the given index.
     */
    QUrl url(const QModelIndex &index) const;

    /**
     * @param index the index which contains the row to fetch the information.
     * @returns if the given index is hidden.
     */
    bool isHidden(const QModelIndex &index) const;

    /**
     * @param index the index which contains the row to fetch the information.
     * @returns if the given index needs to be set up.
     */
    bool isSetupNeeded(const QModelIndex &index) const;

    /**
     * @param index the index which contains the row to fetch the information.
     * @returns if the given index is a device item.
     */
    bool isDevice(const QModelIndex &index) const;

    /**
     * @param index the index which contains the row to fetch the information.
     * @returns if the given index reccomends to be drawn with a capacity bar
     * because it's a volume device item.
     */
    bool isCapacityBarReccomended(const QModelIndex &index) const;

    /**
     * @param index the index which contains the row to look at.
     * @returns the corresponding bookmark for the given index.
     */
    VBookmark bookmarkForIndex(const QModelIndex &index) const;

    /**
     * @param index the index which contains the row to look at.
     * @returns the corresponding device for the given index.
     */
    VDevice deviceForIndex(const QModelIndex &index) const;

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

    VFilePlacesModelPrivate *const d_ptr;
};

/** @}*/

#endif // VFILEPLACESMODEL_H
