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

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QIcon>
#include <QTimer>
#include <QAction>

#include <VibeCore/VStandardDirectories>
#include <VibeStorage/VBookmarkManager>
#include <VibeHardware/VDeviceNotifier>
#include <VibeHardware/VDevice>

#include "vfileplacesmodel.h"
#include "vfileplacesmodel_p.h"
#include "private/fileplacesitem.h"

using namespace VPrivate;

/*
 * VFilePlacesModelPrivate
 */

VFilePlacesModelPrivate::VFilePlacesModelPrivate(VFilePlacesModel *parent) :
    q_ptr(parent),
    favoritesRootItem(0),
    devicesRootItem(0)
{
    // Root items
    rootItem = new FilePlacesItem(QIcon(), QString());
    favoritesRootItem = new FilePlacesItem(QIcon(),
                                           QT_TRANSLATE_NOOP("File manager navigation top-level", "Favorites"),
                                           rootItem);
    devicesRootItem = new FilePlacesItem(QIcon(),
                                         QT_TRANSLATE_NOOP("File manager navigation top-level", "Devices"),
                                         rootItem);

    // Devices predicate
    predicate = VPredicate::fromString(
                    "[[[[ StorageVolume.ignored == false AND [ StorageVolume.usage == 'FileSystem' OR StorageVolume.usage == 'Encrypted' ]]"
                    " OR "
                    "[ IS StorageAccess AND StorageDrive.driveType == 'Floppy' ]]"
                    " OR "
                    "OpticalDisc.availableContent & 'Audio' ]"
                    " OR "
                    "StorageAccess.ignored == false ]");
    Q_ASSERT(predicate.isValid());

    // Standard places bookmark manager
    QString fileName = QString("%1/tracker/bookmarks.xbel")
                       .arg(VStandardDirectories::findDirectory(VStandardDirectories::UserDataDirectory));
    bookmarkManager = VBookmarkManager::managerForFile(fileName, "filemanager");

    // If the bookmarks file doesn't exist, create initial items
    VBookmarkGroup root = bookmarkManager->root();
    if (root.first().isNull() || !QFile::exists(fileName)) {
        FilePlacesItem::createSystemBookmark(bookmarkManager,
                                             "Desktop", QT_TRANSLATE_NOOP("Places Bookmarks", "Desktop"),
                                             QUrl::fromLocalFile(VStandardDirectories::findDirectory(VStandardDirectories::UserDesktopDirectory)),
                                             "user-desktop");
        FilePlacesItem::createSystemBookmark(bookmarkManager,
                                             "Home", QT_TRANSLATE_NOOP("Places Bookmarks", "Home"),
                                             QUrl::fromLocalFile(VStandardDirectories::findDirectory(VStandardDirectories::UserHomeDirectory)),
                                             "user-home");
        FilePlacesItem::createSystemBookmark(bookmarkManager,
                                             "Applications", QT_TRANSLATE_NOOP("Places Bookmarks", "Applications"),
                                             QUrl::fromLocalFile(VStandardDirectories::findDirectory(VStandardDirectories::UserApplicationsDirectory)),
                                             "folder-applications");
        FilePlacesItem::createSystemBookmark(bookmarkManager,
                                             "Documents", QT_TRANSLATE_NOOP("Places Bookmarks", "Documents"),
                                             QUrl::fromLocalFile(VStandardDirectories::findDirectory(VStandardDirectories::UserDocumentsDirectory)),
                                             "folder-documents");
        FilePlacesItem::createSystemBookmark(bookmarkManager,
                                             "Music", QT_TRANSLATE_NOOP("Places Bookmarks", "Music"),
                                             QUrl::fromLocalFile(VStandardDirectories::findDirectory(VStandardDirectories::UserMusicDirectory)),
                                             "folder-sound");
        FilePlacesItem::createSystemBookmark(bookmarkManager,
                                             "Movies", QT_TRANSLATE_NOOP("Places Bookmarks", "Movies"),
                                             QUrl::fromLocalFile(VStandardDirectories::findDirectory(VStandardDirectories::UserMoviesDirectory)),
                                             "folder-video");
        FilePlacesItem::createSystemBookmark(bookmarkManager,
                                             "Pictures", QT_TRANSLATE_NOOP("Places Bookmarks", "Pictures"),
                                             QUrl::fromLocalFile(VStandardDirectories::findDirectory(VStandardDirectories::UserPicturesDirectory)),
                                             "folder-image");
        FilePlacesItem::createSystemBookmark(bookmarkManager,
                                             "Downloads", QT_TRANSLATE_NOOP("Places Bookmarks", "Downloads"),
                                             QUrl::fromLocalFile(VStandardDirectories::findDirectory(VStandardDirectories::UserDownloadsDirectory)),
                                             "folder-download");

        // Force bookmarks to be saved so that open/save dialogs or applications
        // using these bookmarks won't have to create the items above
        bookmarkManager->saveAs(fileName, false);
    }
}

VFilePlacesModelPrivate::~VFilePlacesModelPrivate()
{
    delete rootItem;
    delete favoritesRootItem;
    delete devicesRootItem;
}

void VFilePlacesModelPrivate::_q_initDeviceList()
{
    Q_Q(VFilePlacesModel);

    VDeviceNotifier *notifier = VDeviceNotifier::instance();

    q->connect(notifier, SIGNAL(deviceAdded(const QString &)),
               q, SLOT(_q_deviceAdded(const QString &)));
    q->connect(notifier, SIGNAL(deviceRemoved(const QString &)),
               q, SLOT(_q_deviceRemoved(const QString &)));

    const QList<VDevice> &deviceList = VDevice::listFromQuery(predicate);

    foreach(const VDevice & device, deviceList)
    availableDevices << device.udi();

    _q_reloadBookmarks();
}

void VFilePlacesModelPrivate::_q_deviceAdded(const QString &udi)
{
    VDevice d(udi);

    if (predicate.matches(d)) {
        availableDevices << udi;
        _q_reloadBookmarks();
    }
}

void VFilePlacesModelPrivate::_q_deviceRemoved(const QString &udi)
{
    if (availableDevices.contains(udi)) {
        availableDevices.remove(udi);
        _q_reloadBookmarks();
    }
}

void VFilePlacesModelPrivate::_q_itemChanged(const QString &id)
{
    Q_Q(VFilePlacesModel);

    // Emit a signal for favorite items that had changed
    for (int row = 0; row < favoritesRootItem->childCount(); ++row) {
        if (favoritesRootItem->child(row)->id() == id) {
            QModelIndex index = q->index(row, 0);
            emit q->dataChanged(index, index);
        }
    }

    // Emit a signal for device items that had changed
    for (int row = 0; row < devicesRootItem->childCount(); ++row) {
        if (devicesRootItem->child(row)->id() == id) {
            QModelIndex index = q->index(row, 0);
            emit q->dataChanged(index, index);
        }
    }
}

void VFilePlacesModelPrivate::_q_reloadBookmarks()
{
    Q_Q(VFilePlacesModel);

    // Load favorites
    VBookmarkGroup root = bookmarkManager->root();
    VBookmark bookmark = root.first();

    while (!bookmark.isNull()) {
        QString udi = bookmark.metaDataItem("UDI");

        // TODO: Check if OnlyInApp metadata is available and matches with current application's name

        FilePlacesItem *item = 0;
        if (udi.isEmpty())
            item = new FilePlacesItem(bookmarkManager,
                                      bookmark.address(),
                                      favoritesRootItem);
        else
            item = new FilePlacesItem(bookmarkManager,
                                      bookmark.address(), udi,
                                      favoritesRootItem);
        q->connect(item, SIGNAL(itemChanged(const QString &)),
                   q, SLOT(_q_itemChanged(const QString &)));

        bookmark = root.next(bookmark);
    }

    // Load devices
    foreach(const QString & udi, availableDevices) {
        bookmark = FilePlacesItem::createDeviceBookmark(bookmarkManager, udi);
        if (!bookmark.isNull()) {
            FilePlacesItem *item =
                new FilePlacesItem(bookmarkManager,
                                   bookmark.address(),
                                   udi, devicesRootItem);
            q->connect(item, SIGNAL(itemChanged(QString)),
                       q, SLOT(_q_itemChanged(QString)));
        }
    }
}

/*
 * VFilePlacesModel
 */

VFilePlacesModel::VFilePlacesModel(QObject *parent) :
    QAbstractItemModel(parent),
    d_ptr(new VFilePlacesModelPrivate(this))
{
    // Reload bookmarks when something changes
    connect(d_ptr->bookmarkManager, SIGNAL(changed(QString, QString)),
            this, SLOT(_q_reloadBookmarks()));
    connect(d_ptr->bookmarkManager, SIGNAL(bookmarksChanged(QString)),
            this, SLOT(_q_reloadBookmarks()));

    // Initialize devices list
    QTimer::singleShot(0, this, SLOT(_q_initDeviceList()));
}

VFilePlacesModel::~VFilePlacesModel()
{
    delete d_ptr;
}

QVariant VFilePlacesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    FilePlacesItem *item = static_cast<FilePlacesItem *>(index.internalPointer());
    return item->data(role);
}

Qt::ItemFlags VFilePlacesModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex VFilePlacesModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_D(const VFilePlacesModel);

    if (!hasIndex(row, column, parent))
        return QModelIndex();

    FilePlacesItem *parentItem;

    if (!parent.isValid())
        parentItem = d->rootItem;
    else
        parentItem = static_cast<FilePlacesItem *>(parent.internalPointer());

    FilePlacesItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex VFilePlacesModel::parent(const QModelIndex &child) const
{
    Q_D(const VFilePlacesModel);

    if (!child.isValid())
        return QModelIndex();

    FilePlacesItem *childItem = static_cast<FilePlacesItem *>(child.internalPointer());
    FilePlacesItem *parentItem = childItem->parent();

    if (parentItem == d->rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int VFilePlacesModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const VFilePlacesModel);

    FilePlacesItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = d->rootItem;
    else
        parentItem = static_cast<FilePlacesItem *>(parent.internalPointer());

    return parentItem->childCount();
}

int VFilePlacesModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QIcon VFilePlacesModel::icon(const QModelIndex &index) const
{
    return QIcon(data(index, Qt::DecorationRole).value<QIcon>());
}

QString VFilePlacesModel::text(const QModelIndex &index) const
{
    return data(index, Qt::DisplayRole).toString();
}

QUrl VFilePlacesModel::url(const QModelIndex &index) const
{
    return QUrl(data(index, UrlRole).toUrl());
}

bool VFilePlacesModel::isHidden(const QModelIndex &index) const
{
    return data(index, HiddenRole).toBool();
}

bool VFilePlacesModel::isSetupNeeded(const QModelIndex &index) const
{
    return data(index, SetupNeededRole).toBool();
}

bool VFilePlacesModel::isDevice(const QModelIndex &index) const
{
    if (!index.isValid())
        return false;

    FilePlacesItem *item
    = static_cast<FilePlacesItem *>(index.internalPointer());
    return item->isDevice();
}

bool VFilePlacesModel::isCapacityBarReccomended(const QModelIndex &index) const
{
    return data(index, CapacityBarReccomendedRole).toBool();
}

VBookmark VFilePlacesModel::bookmarkForIndex(const QModelIndex &index) const
{
    if (!index.isValid())
        return VBookmark();

    FilePlacesItem *item =
        static_cast<FilePlacesItem *>(index.internalPointer());
    if (!item->isDevice() && !item->isTopLevel())
        return item->bookmark();
    return VBookmark();
}

VDevice VFilePlacesModel::deviceForIndex(const QModelIndex &index) const
{
    if (!index.isValid())
        return VDevice();

    FilePlacesItem *item =
        static_cast<FilePlacesItem *>(index.internalPointer());
    if (item->isDevice())
        return item->device();
    return VDevice();
}

#include "vfileplacesmodel.moc"
