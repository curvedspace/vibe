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

#include "vfilemanagernavigationmodel.h"
#include "vfilemanagernavigationmodel_p.h"
#include "filemanagernavigationitem.h"

/*
 * VFileManagerNavigationModelPrivate
 */

VFileManagerNavigationModelPrivate::VFileManagerNavigationModelPrivate(VFileManagerNavigationModel *parent) :
    q_ptr(parent),
    favoritesRootItem(0),
    devicesRootItem(0)
{
    // Root items
    rootItem = new FileManagerNavigationItem(QIcon(), QString());
    favoritesRootItem = new FileManagerNavigationItem(QIcon(),
                                                      QT_TRANSLATE_NOOP("File manager navigation top-level", "Favorites"),
                                                      rootItem);
    devicesRootItem = new FileManagerNavigationItem(QIcon(),
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
        FileManagerNavigationItem::createSystemBookmark(bookmarkManager,
                                                        "Desktop", QT_TRANSLATE_NOOP("Places Bookmarks", "Desktop"),
                                                        QUrl::fromLocalFile(VStandardDirectories::findDirectory(VStandardDirectories::UserDesktopDirectory)),
                                                        "user-desktop");
        FileManagerNavigationItem::createSystemBookmark(bookmarkManager,
                                                        "Home", QT_TRANSLATE_NOOP("Places Bookmarks", "Home"),
                                                        QUrl::fromLocalFile(VStandardDirectories::findDirectory(VStandardDirectories::UserHomeDirectory)),
                                                        "user-home");
        FileManagerNavigationItem::createSystemBookmark(bookmarkManager,
                                                        "Applications", QT_TRANSLATE_NOOP("Places Bookmarks", "Applications"),
                                                        QUrl::fromLocalFile(VStandardDirectories::findDirectory(VStandardDirectories::UserApplicationsDirectory)),
                                                        "folder-applications");
        FileManagerNavigationItem::createSystemBookmark(bookmarkManager,
                                                        "Documents", QT_TRANSLATE_NOOP("Places Bookmarks", "Documents"),
                                                        QUrl::fromLocalFile(VStandardDirectories::findDirectory(VStandardDirectories::UserDocumentsDirectory)),
                                                        "folder-documents");
        FileManagerNavigationItem::createSystemBookmark(bookmarkManager,
                                                        "Music", QT_TRANSLATE_NOOP("Places Bookmarks", "Music"),
                                                        QUrl::fromLocalFile(VStandardDirectories::findDirectory(VStandardDirectories::UserMusicDirectory)),
                                                        "folder-sound");
        FileManagerNavigationItem::createSystemBookmark(bookmarkManager,
                                                        "Movies", QT_TRANSLATE_NOOP("Places Bookmarks", "Movies"),
                                                        QUrl::fromLocalFile(VStandardDirectories::findDirectory(VStandardDirectories::UserMoviesDirectory)),
                                                        "folder-video");
        FileManagerNavigationItem::createSystemBookmark(bookmarkManager,
                                                        "Pictures", QT_TRANSLATE_NOOP("Places Bookmarks", "Pictures"),
                                                        QUrl::fromLocalFile(VStandardDirectories::findDirectory(VStandardDirectories::UserPicturesDirectory)),
                                                        "folder-image");
        FileManagerNavigationItem::createSystemBookmark(bookmarkManager,
                                                        "Downloads", QT_TRANSLATE_NOOP("Places Bookmarks", "Downloads"),
                                                        QUrl::fromLocalFile(VStandardDirectories::findDirectory(VStandardDirectories::UserDownloadsDirectory)),
                                                        "folder-download");

        // Force bookmarks to be saved so that open/save dialogs or applications
        // using these bookmarks won't have to create the items above
        bookmarkManager->saveAs(fileName, false);
    }
}

VFileManagerNavigationModelPrivate::~VFileManagerNavigationModelPrivate()
{
    delete rootItem;
    delete favoritesRootItem;
    delete devicesRootItem;
}

void VFileManagerNavigationModelPrivate::_q_initDeviceList()
{
    Q_Q(VFileManagerNavigationModel);

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

void VFileManagerNavigationModelPrivate::_q_deviceAdded(const QString &udi)
{
    VDevice d(udi);

    if (predicate.matches(d)) {
        availableDevices << udi;
        _q_reloadBookmarks();
    }
}

void VFileManagerNavigationModelPrivate::_q_deviceRemoved(const QString &udi)
{
    if (availableDevices.contains(udi)) {
        availableDevices.remove(udi);
        _q_reloadBookmarks();
    }
}

void VFileManagerNavigationModelPrivate::_q_itemChanged(const QString &id)
{
    Q_Q(VFileManagerNavigationModel);

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

void VFileManagerNavigationModelPrivate::_q_reloadBookmarks()
{
    Q_Q(VFileManagerNavigationModel);

    // Load favorites
    VBookmarkGroup root = bookmarkManager->root();
    VBookmark bookmark = root.first();

    while (!bookmark.isNull()) {
        QString udi = bookmark.metaDataItem("UDI");

        // TODO: Check if OnlyInApp metadata is available and matches with current application's name

        FileManagerNavigationItem *item = 0;
        if (udi.isEmpty())
            item = new FileManagerNavigationItem(bookmarkManager,
                                                 bookmark.address(),
                                                 favoritesRootItem);
        else
            item = new FileManagerNavigationItem(bookmarkManager,
                                                 bookmark.address(), udi,
                                                 favoritesRootItem);
        q->connect(item, SIGNAL(itemChanged(const QString &)),
                   q, SLOT(_q_itemChanged(const QString &)));

        bookmark = root.next(bookmark);
    }

    // Load devices
    foreach(const QString & udi, availableDevices) {
        bookmark = FileManagerNavigationItem::createDeviceBookmark(bookmarkManager, udi);
        if (!bookmark.isNull()) {
            FileManagerNavigationItem *item =
                new FileManagerNavigationItem(bookmarkManager,
                                              bookmark.address(),
                                              udi, devicesRootItem);
            q->connect(item, SIGNAL(itemChanged(QString)),
                       q, SLOT(_q_itemChanged(QString)));
        }
    }
}

/*
 * VFileManagerNavigationModel
 */

VFileManagerNavigationModel::VFileManagerNavigationModel(QObject *parent) :
    QAbstractItemModel(parent),
    d_ptr(new VFileManagerNavigationModelPrivate(this))
{
    // Reload bookmarks when something changes
    connect(d_ptr->bookmarkManager, SIGNAL(changed(QString, QString)),
            this, SLOT(_q_reloadBookmarks()));
    connect(d_ptr->bookmarkManager, SIGNAL(bookmarksChanged(QString)),
            this, SLOT(_q_reloadBookmarks()));

    // Initialize devices list
    QTimer::singleShot(0, this, SLOT(_q_initDeviceList()));
}

VFileManagerNavigationModel::~VFileManagerNavigationModel()
{
    delete d_ptr;
}

QVariant VFileManagerNavigationModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    FileManagerNavigationItem *item = static_cast<FileManagerNavigationItem *>(index.internalPointer());
    return item->data(role);
}

Qt::ItemFlags VFileManagerNavigationModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex VFileManagerNavigationModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_D(const VFileManagerNavigationModel);

    if (!hasIndex(row, column, parent))
        return QModelIndex();

    FileManagerNavigationItem *parentItem;

    if (!parent.isValid())
        parentItem = d->rootItem;
    else
        parentItem = static_cast<FileManagerNavigationItem *>(parent.internalPointer());

    FileManagerNavigationItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex VFileManagerNavigationModel::parent(const QModelIndex &child) const
{
    Q_D(const VFileManagerNavigationModel);

    if (!child.isValid())
        return QModelIndex();

    FileManagerNavigationItem *childItem = static_cast<FileManagerNavigationItem *>(child.internalPointer());
    FileManagerNavigationItem *parentItem = childItem->parent();

    if (parentItem == d->rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int VFileManagerNavigationModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const VFileManagerNavigationModel);

    FileManagerNavigationItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = d->rootItem;
    else
        parentItem = static_cast<FileManagerNavigationItem *>(parent.internalPointer());

    return parentItem->childCount();
}

int VFileManagerNavigationModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

#include "vfilemanagernavigationmodel.moc"
