/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPL-ONLY$
 *
 * This file may be used under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation and
 * appearing in the file LICENSE.LGPL included in the packaging of
 * this file, either version 2.1 of the License, or (at your option) any
 * later version.  Please review the following information to ensure the
 * GNU Lesser General Public License version 2.1 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 * If you have questions regarding the use of this file, please contact
 * us via http://www.maui-project.org/.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QIcon>
#include <QTimer>
#include <QAction>
#include <QStandardPaths>

#include <solid/devicenotifier.h>
#include <solid/device.h>
#include <solid/storageaccess.h>
#include <solid/storagedrive.h>
#include <solid/opticaldisc.h>
#include <solid/opticaldrive.h>

#include <VibeCore/VBookmarkManager>

#include "vfileplacesmodel.h"
#include "vfileplacesmodel_p.h"
#include "vfileplacesitem.h"

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
    predicate = Solid::Predicate::fromString(
                    "[[[[ StorageVolume.ignored == false AND [ StorageVolume.usage == 'FileSystem' OR StorageVolume.usage == 'Encrypted' ]]"
                    " OR "
                    "[ IS StorageAccess AND StorageDrive.driveType == 'Floppy' ]]"
                    " OR "
                    "OpticalDisc.availableContent & 'Audio' ]"
                    " OR "
                    "StorageAccess.ignored == false ]");
    Q_ASSERT(predicate.isValid());

    // Standard places bookmark manager
    QString fileName = QStringLiteral("%1/tracker/bookmarks.xbel").arg(
        QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));
    bookmarkManager = VBookmarkManager::managerForFile(fileName, "filemanager");

    // If the bookmarks file doesn't exist, create initial items
    VBookmarkGroup root = bookmarkManager->root();
    if (root.first().isNull() || !QFile::exists(fileName)) {
        FilePlacesItem::createSystemBookmark(bookmarkManager,
                                             "Desktop", QT_TRANSLATE_NOOP("Places Bookmarks", "Desktop"),
                                             QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)),
                                             "user-desktop");
        FilePlacesItem::createSystemBookmark(bookmarkManager,
                                             "Home", QT_TRANSLATE_NOOP("Places Bookmarks", "Home"),
                                             QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)),
                                             "user-home");
        FilePlacesItem::createSystemBookmark(bookmarkManager,
                                             "Applications", QT_TRANSLATE_NOOP("Places Bookmarks", "Applications"),
                                             QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation)),
                                             "folder-applications");
        FilePlacesItem::createSystemBookmark(bookmarkManager,
                                             "Documents", QT_TRANSLATE_NOOP("Places Bookmarks", "Documents"),
                                             QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)),
                                             "folder-documents");
        FilePlacesItem::createSystemBookmark(bookmarkManager,
                                             "Music", QT_TRANSLATE_NOOP("Places Bookmarks", "Music"),
                                             QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::MusicLocation)),
                                             "folder-sound");
        FilePlacesItem::createSystemBookmark(bookmarkManager,
                                             "Movies", QT_TRANSLATE_NOOP("Places Bookmarks", "Movies"),
                                             QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::MoviesLocation)),
                                             "folder-video");
        FilePlacesItem::createSystemBookmark(bookmarkManager,
                                             "Pictures", QT_TRANSLATE_NOOP("Places Bookmarks", "Pictures"),
                                             QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)),
                                             "folder-image");
        FilePlacesItem::createSystemBookmark(bookmarkManager,
                                             "Downloads", QT_TRANSLATE_NOOP("Places Bookmarks", "Downloads"),
                                             QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)),
                                             "folder-download");

        // Force bookmarks to be saved so that open/save dialogs or applications
        // using these bookmarks won't have to create the items above
        bookmarkManager->saveAs(fileName, false);
    }
}

VFilePlacesModelPrivate::~VFilePlacesModelPrivate()
{
    qDeleteAll(favoriteItems);
    qDeleteAll(deviceItems);
    delete rootItem;
    delete favoritesRootItem;
    delete devicesRootItem;
}

QList<FilePlacesItem *> VFilePlacesModelPrivate::loadFavoritesList()
{
    Q_Q(VFilePlacesModel);

    QList<FilePlacesItem *> list;

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
        QObject::connect(item, SIGNAL(itemChanged(const QString &)),
                         q, SLOT(_q_itemChanged(const QString &)));
        list << item;

        bookmark = root.next(bookmark);
    }

    return list;
}

QList<FilePlacesItem *> VFilePlacesModelPrivate::loadDevicesList()
{
    Q_Q(VFilePlacesModel);

    QList<FilePlacesItem *> list;

    VBookmarkGroup root = bookmarkManager->root();
    VBookmark bookmark = root.first();

    foreach(const QString & udi, availableDevices) {
        bookmark = FilePlacesItem::createDeviceBookmark(bookmarkManager, udi);
        if (!bookmark.isNull()) {
            FilePlacesItem *item =
                new FilePlacesItem(bookmarkManager,
                                   bookmark.address(),
                                   udi, devicesRootItem);
            QObject::connect(item, SIGNAL(itemChanged(QString)),
                             q, SLOT(_q_itemChanged(QString)));
            list << item;
        }
    }

    return list;
}

void VFilePlacesModelPrivate::reloadList(const QModelIndex &parent,
                                         QList<FilePlacesItem *> currentItems,
                                         QList<FilePlacesItem *> &items)
{
    Q_Q(VFilePlacesModel);

    qDebug() << "-----" << parent.data(Qt::DisplayRole).toString();
    QList<FilePlacesItem *>::Iterator it_i = items.begin();
    QList<FilePlacesItem *>::Iterator it_c = currentItems.begin();

    QList<FilePlacesItem *>::Iterator end_i = items.end();
    QList<FilePlacesItem *>::Iterator end_c = currentItems.end();

    while (it_i != end_i || it_c != end_c) {
        if (it_i == end_i && it_c != end_c) {
            int row = items.count();

            qDebug() << "inserting row" << row;
            q->beginInsertRows(parent, row, row);
            it_i = items.insert(it_i, *it_c);
            ++it_i;
            it_c = currentItems.erase(it_c);

            end_i = items.end();
            end_c = currentItems.end();
            q->endInsertRows();
        } else if (it_i != end_i && it_c == end_c) {
            int row = items.indexOf(*it_i);

            qDebug() << "removing row" << row;
            q->beginRemoveRows(parent, row, row);
            delete *it_i;
            it_i = items.erase(it_i);

            end_i = items.end();
            end_c = currentItems.end();
            q->endRemoveRows();
        } else if ((*it_i)->id() == (*it_c)->id()) {
            bool shouldEmit = !((*it_i)->bookmark() == (*it_c)->bookmark());
            (*it_i)->setBookmark((*it_c)->bookmark());
            if (shouldEmit) {
                int row = items.indexOf(*it_i);
                qDebug() << "changing row" << row;
                QModelIndex idx = q->index(row, 0, parent);
                emit q->dataChanged(idx, idx);
            }
            ++it_i;
            ++it_c;
        } else if ((*it_i)->id() != (*it_c)->id()) {
            int row = items.indexOf(*it_i);

            // If the next one matches, it's a remove
            if (it_i + 1 != end_i && (*(it_i + 1))->id() == (*it_c)->id()) {
                qDebug() << "2 removing row" << row;
                q->beginRemoveRows(parent, row, row);
                delete *it_i;
                it_i = items.erase(it_i);

                end_i = items.end();
                end_c = currentItems.end();
                q->endRemoveRows();
            } else {
                qDebug() << "2 inserting row" << row;
                q->beginInsertRows(parent, row, row);
                it_i = items.insert(it_i, *it_c);
                ++it_i;
                it_c = currentItems.erase(it_c);

                end_i = items.end();
                end_c = currentItems.end();
                q->endInsertRows();
            }
        }
    }

    qDeleteAll(currentItems);
    currentItems.clear();
}

void VFilePlacesModelPrivate::_q_initDeviceList()
{
    Q_Q(VFilePlacesModel);

    Solid::DeviceNotifier *notifier = Solid::DeviceNotifier::instance();

    q->connect(notifier, SIGNAL(deviceAdded(const QString &)),
               q, SLOT(_q_deviceAdded(const QString &)));
    q->connect(notifier, SIGNAL(deviceRemoved(const QString &)),
               q, SLOT(_q_deviceRemoved(const QString &)));

    const QList<Solid::Device> &deviceList = Solid::Device::listFromQuery(predicate);
    foreach(const Solid::Device & device, deviceList)
    availableDevices << device.udi();

    _q_reloadDevices();
}

void VFilePlacesModelPrivate::_q_deviceAdded(const QString &udi)
{
    Solid::Device d(udi);

    if (predicate.matches(d)) {
        availableDevices << udi;
        _q_reloadDevices();
    }
}

void VFilePlacesModelPrivate::_q_deviceRemoved(const QString &udi)
{
    if (availableDevices.contains(udi)) {
        availableDevices.remove(udi);
        _q_reloadDevices();
    }
}

void VFilePlacesModelPrivate::_q_itemChanged(const QString &id)
{
    Q_Q(VFilePlacesModel);

    // Emit a signal for favorite items that had changed
    for (int row = 0; row < favoritesRootItem->childCount(); ++row) {
        if (favoritesRootItem->childAt(row)->id() == id) {
            QModelIndex parent = q->index(0, 0, QModelIndex());
            QModelIndex index = q->index(row, 0, parent);
            emit q->dataChanged(index, index);
        }
    }

    // Emit a signal for device items that had changed
    for (int row = 0; row < devicesRootItem->childCount(); ++row) {
        if (devicesRootItem->childAt(row)->id() == id) {
            QModelIndex parent = q->index(1, 0, QModelIndex());
            QModelIndex index = q->index(row, 0, parent);
            emit q->dataChanged(index, index);
        }
    }
}

void VFilePlacesModelPrivate::_q_reloadFavorites()
{
    Q_Q(VFilePlacesModel);

    QList<FilePlacesItem *> favorites = loadFavoritesList();
    reloadList(q->index(0, 0, QModelIndex()), favorites, favoriteItems);
}

void VFilePlacesModelPrivate::_q_reloadDevices()
{
    Q_Q(VFilePlacesModel);

    QList<FilePlacesItem *> devices = loadDevicesList();
    reloadList(q->index(1, 0, QModelIndex()), devices, deviceItems);
}

void VFilePlacesModelPrivate::_q_storageTeardownDone(Solid::ErrorType error, QVariant errorData)
{
    Q_Q(VFilePlacesModel);

    if (error && errorData.isValid())
        emit q->errorMessage(errorData.toString());
}

void VFilePlacesModelPrivate::_q_storageSetupDone(Solid::ErrorType error, QVariant errorData)
{
    Q_Q(VFilePlacesModel);

    QPersistentModelIndex index = setupInProgress.take(q->sender());
    if (!index.isValid())
        return;

    if (!error) {
        emit q->setupDone(index, true);
    } else {
        if (errorData.isValid())
            emit q->errorMessage(q->tr("An error occurred while accessing '%1', the system responded: %2")
                                 .arg(q->text(index))
                                 .arg(errorData.toString()));
        else
            emit q->errorMessage(q->tr("An error occurred while accessing '%1'")
                                 .arg(q->text(index)));
        emit q->setupDone(index, false);
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
            this, SLOT(_q_reloadFavorites()));
    connect(d_ptr->bookmarkManager, SIGNAL(bookmarksChanged(QString)),
            this, SLOT(_q_reloadFavorites()));

    // Load bookmarks and initialize devices list
    QTimer::singleShot(0, this, SLOT(_q_reloadFavorites()));
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

    FilePlacesItem *childItem = parentItem->childAt(row);
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

    // This model has only one column
    if (parent.column() > 0)
        return 0;

    FilePlacesItem *parentItem = 0;
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

bool VFilePlacesModel::isTopLevel(const QModelIndex &index) const
{
    if (!index.isValid())
        return false;

    FilePlacesItem *item =
        static_cast<FilePlacesItem *>(index.internalPointer());
    return item->isTopLevel();
}

bool VFilePlacesModel::isDevice(const QModelIndex &index) const
{
    if (!index.isValid())
        return false;

    FilePlacesItem *item =
        static_cast<FilePlacesItem *>(index.internalPointer());
    return item->isDevice();
}

bool VFilePlacesModel::isCapacityBarReccomended(const QModelIndex &index) const
{
    return data(index, CapacityBarReccomendedRole).toBool();
}

int VFilePlacesModel::hiddenCount(const QModelIndex &parent) const
{
    Q_D(const VFilePlacesModel);

    int hidden = 0;

    for (int i = 0; i < rowCount(parent); ++i) {
        if (isHidden(index(i, 0, parent)))
            hidden++;
    }

    return hidden;
}

QModelIndex VFilePlacesModel::closestItem(const QUrl &url) const
{
    Q_D(const VFilePlacesModel);

    int foundRow = -1;
    int maxLength = 0;

    /*
     * Search the item which is equal to the URL passed as argument or at least
     * is a parent.  If there are more than one possible candidates, choose the item
     * which covers the bigger range.
     */
    for (int row = 0; row < d->favoritesRootItem->childCount(); ++row) {
        FilePlacesItem *item = d->favoritesRootItem->childAt(row);

        QUrl itemUrl = item->data(UrlRole).toUrl();
        if (itemUrl.isParentOf(url)) {
            const int length = itemUrl.toEncoded(QUrl::RemovePassword).length();
            if (length > maxLength) {
                foundRow = row;
                maxLength = length;
            }
        }
    }

    if (foundRow == -1)
        return QModelIndex();
    return createIndex(foundRow, 0, d->favoritesRootItem->childAt(foundRow));
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

Solid::Device VFilePlacesModel::deviceForIndex(const QModelIndex &index) const
{
    if (!index.isValid())
        return Solid::Device();

    FilePlacesItem *item =
        static_cast<FilePlacesItem *>(index.internalPointer());
    if (item->isDevice())
        return item->device();
    return Solid::Device();
}

QAction *VFilePlacesModel::teardownActionForIndex(const QModelIndex &index) const
{
    Solid::Device device = deviceForIndex(index);

    if (device.is<Solid::StorageAccess>() && device.as<Solid::StorageAccess>()->isAccessible()) {
        Solid::StorageDrive *drive = device.as<Solid::StorageDrive>();
        if (!drive)
            drive = device.parent().as<Solid::StorageDrive>();

        bool isHotpluggable = false;
        bool isRemovable = false;

        if (drive) {
            isHotpluggable = drive->isHotpluggable();
            isRemovable = drive->isRemovable();
        }

        QString label = text(index).replace('&', "&&");
        QString iconName, text;

        if (device.is<Solid::OpticalDisc>())
            text = tr("&Release '%1'").arg(label);
        else if (isHotpluggable || isRemovable) {
            text = tr("&Safely Remove '%1'").arg(label);
            iconName = "media-eject";
        } else {
            text = tr("&Unmount '%1'").arg(label);
            iconName = "media-eject";
        }

        if (!iconName.isEmpty())
            return new QAction(QIcon::fromTheme(iconName), text, 0);
        return new QAction(text, 0);
    }

    return 0;
}

QAction *VFilePlacesModel::ejectActionForIndex(const QModelIndex &index) const
{
    Solid::Device device = deviceForIndex(index);

    if (device.is<Solid::OpticalDisc>()) {
        QString label = text(index).replace('&', "&&");
        QString text = tr("&Eject '%1'").arg(label);

        return new QAction(QIcon::fromTheme("media-eject"), text, 0);
    }

    return 0;
}

void VFilePlacesModel::requestTeardown(const QModelIndex &index)
{
    Solid::Device device = deviceForIndex(index);
    Solid::StorageAccess *access = device.as<Solid::StorageAccess>();

    if (access) {
        connect(access, SIGNAL(teardownDone(Solid::ErrorType, QVariant, QString)),
                this, SLOT(_q_storageTeardownDone(Solid::ErrorType, QVariant)));
        access->teardown();
    }
}

void VFilePlacesModel::requestEject(const QModelIndex &index)
{
    Solid::Device device = deviceForIndex(index);
    Solid::OpticalDrive *drive = device.parent().as<Solid::OpticalDrive>();

    if (drive) {
        connect(drive, SIGNAL(ejectDone(Solid::ErrorType, QVariant, QString)),
                this, SLOT(_q_storageTeardownDone(Solid::ErrorType, QVariant)));
        drive->eject();
    } else {
        QString label = text(index).replace('&', "&&");
        QString message = tr("The device '%1' is not an optical drive and cannot be ejected.").arg(label);
        emit errorMessage(message);
    }
}

void VFilePlacesModel::requestSetup(const QModelIndex &index)
{
    Q_D(VFilePlacesModel);

    Solid::Device device = deviceForIndex(index);

    if (device.is<Solid::StorageAccess>() &&
            !d->setupInProgress.contains(device.as<Solid::StorageAccess>()) &&
            !device.as<Solid::StorageAccess>()->isAccessible()) {
        Solid::StorageAccess *access = device.as<Solid::StorageAccess>();
        d->setupInProgress[access] = index;

        connect(access, SIGNAL(setupDone(Solid::ErrorType, QVariant, QString)),
                this, SLOT(_q_storageSetupDone(Solid::ErrorType, QVariant)));
        access->setup();
    }
}

#include "moc_vfileplacesmodel.cpp"
