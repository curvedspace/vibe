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

/*
 * This code is inspired by KFilePlacesModel from KDE libraries.
 * KFilePlacesModel was made by the following people:
 *      Kevin Ottens <ervin@kde.org>
 *      David Faure <faure@kde.org>
 */

/** \addtogroup storage Storage Kit
 *  @{
 */

class QAction;

class VBookmark;
class VDevice;

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

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
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
     * @param index the index which contains the row to fetch the information from.
     * @returns if the given index is hidden.
     */
    bool isHidden(const QModelIndex &index) const;

    /**
     * @param index the index which contains the row to fetch the information from.
     * @returns if the given index needs to be set up.
     */
    bool isSetupNeeded(const QModelIndex &index) const;

    /**
     * @param index the index which contains the row to fetch the information from.
     * @returns if the given index is a top-level item (Favorites or Devices).
     */
    bool isTopLevel(const QModelIndex &index) const;

    /**
     * @param index the index which contains the row to fetch the information from.
     * @returns if the given index is a device item.
     */
    bool isDevice(const QModelIndex &index) const;

    /**
     * @param index the index which contains the row to fetch the information from.
     * @returns if the given index reccomends to be drawn with a capacity bar
     * because it's a volume device item.
     */
    bool isCapacityBarReccomended(const QModelIndex &index) const;

    /**
     * @param parent the parent index.
     * @returns the number of hidden items.
     */
    int hiddenCount(const QModelIndex &parent) const;

    /**
     * Returns the closest item for the URL \a url.
     * The closest item is defined as item which is equal to
     * the URL or at least is a parent URL. If there are more than
     * one possible parent URL candidates, the item which covers
     * the bigger range of the URL is returned.
     *
     * Example: the url is '/home/peter/Fringe/Files'.
     * Available items are:
     * - /home/peter
     * - /home/peter/Fringe
     *
     * The returned item will the one for '/home/peter/Fringe'.
     */
    QModelIndex closestItem(const QUrl &url) const;

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

    /**
     * If the item corresponding to the given index is a removable device, return
     * an action to release it.
     *
     * @param index the index which contains the item.
     */
    QAction *teardownActionForIndex(const QModelIndex &index) const;

    /**
     * If the item corresponding to the given index is an optical disc. return
     * an action to eject it.
     *
     * @param index the index which contains the item.
     */
    QAction *ejectActionForIndex(const QModelIndex &index) const;

    /**
     * Invoke teardown action for the device item corresponding to the given
     * index.
     *
     * @param index the index which represents the device to teardown.
     */
    void requestTeardown(const QModelIndex &index);

    /**
     * Invoke eject action for the device item corresponding to the given
     * index. Eject can be performed only on optical drives.
     *
     * @param index the index which represents the device to eject.
     */
    void requestEject(const QModelIndex &index);

    /**
     * Invoke setup action for the device item corresponding to the given
     * index.
     *
     * @param index the index which represents the device to setup.
     */
    void requestSetup(const QModelIndex &index);

signals:
    void setupDone(const QModelIndex &index, bool success);
    void errorMessage(const QString &msg);

private:
    Q_PRIVATE_SLOT(d_ptr, void _q_initDeviceList())
    Q_PRIVATE_SLOT(d_ptr, void _q_deviceAdded(const QString &))
    Q_PRIVATE_SLOT(d_ptr, void _q_deviceRemoved(const QString &))
    Q_PRIVATE_SLOT(d_ptr, void _q_itemChanged(const QString &))
    Q_PRIVATE_SLOT(d_ptr, void _q_reloadFavorites())
    Q_PRIVATE_SLOT(d_ptr, void _q_reloadDevices())
    Q_PRIVATE_SLOT(d_ptr, void _q_storageTeardownDone(VHardware::ErrorType, QVariant))
    Q_PRIVATE_SLOT(d_ptr, void _q_storageSetupDone(VHardware::ErrorType, QVariant))

    VFilePlacesModelPrivate *const d_ptr;
};

/** @}*/

#endif // VFILEPLACESMODEL_H
