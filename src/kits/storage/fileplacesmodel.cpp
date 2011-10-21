/*  This file is part of the KDE project
    Copyright (C) 2007 Kevin Ottens <ervin@kde.org>
    Copyright (C) 2007 David Faure <faure@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License version 2 as published by the Free Software Foundation.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.

*/

#include <QtCore/QMimeData>
#include <QtCore/QTimer>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QtGui/QAction>

#include <QubeHardware/DeviceNotifier>
#include <QubeHardware/StorageAccess>
#include <QubeHardware/StorageDrive>
#include <QubeHardware/StorageVolume>
#include <QubeHardware/OpticalDrive>
#include <QubeHardware/OpticalDisc>
#include <QubeHardware/Predicate>

#include "fileplacesmodel.h"
#include "fileplacesitem_p.h"
#include "fileplacessharedbookmarks_p.h"

#define I18N_NOOP2(x, y) QString(y)

namespace QubeStorage
{
    class FilePlacesModel::Private : public QObject
    {
        Q_OBJECT
    public:
        Private(FilePlacesModel *self) :
            q(self),
            bookmarkManager(0),
            sharedBookmarks(0) {
        }

        ~Private() {
            delete sharedBookmarks;
            qDeleteAll(items);
        }

        FilePlacesModel *q;

        QList<FilePlacesItem*> items;
        QSet<QString> availableDevices;
        QMap<QObject*, QPersistentModelIndex> setupInProgress;

        QubeHardware::Predicate predicate;
        BookmarkManager *bookmarkManager;
        FilePlacesSharedBookmarks * sharedBookmarks;

        void reloadAndSignal() {
            bookmarkManager->emitChanged(bookmarkManager->root()); // ... we'll get relisted anyway
        }

        QList<FilePlacesItem *> loadBookmarkList() {
            QList<FilePlacesItem*> items;

            BookmarkGroup root = bookmarkManager->root();
            Bookmark bookmark = root.first();
            QSet<QString> devices = availableDevices;

            while (!bookmark.isNull()) {
                QString udi = bookmark.metaDataItem("UDI");
                QString appName = bookmark.metaDataItem("OnlyInApp");
                bool deviceAvailable = devices.remove(udi);

                bool allowedHere = appName.isEmpty();

                if ((udi.isEmpty() && allowedHere) || deviceAvailable) {
                    FilePlacesItem *item;
                    if (deviceAvailable)
                        // TODO: Update bookmark internal element
                        item = new FilePlacesItem(bookmarkManager, bookmark.address(), udi);
                    else
                        item = new FilePlacesItem(bookmarkManager, bookmark.address());
                    connect(item, SIGNAL(itemChanged(const QString&)),
                            q, SLOT(_q_itemChanged(const QString&)));
                    items << item;
                }

                bookmark = root.next(bookmark);
            }

            // Add bookmarks for the remaining devices, they were previously unknown
            foreach (const QString &udi, devices) {
                bookmark = FilePlacesItem::createDeviceBookmark(bookmarkManager, udi);
                if (!bookmark.isNull()) {
                    FilePlacesItem *item = new FilePlacesItem(bookmarkManager,
                            bookmark.address(), udi);
                    connect(item, SIGNAL(itemChanged(const QString&)),
                            q, SLOT(_q_itemChanged(const QString&)));
                    // TODO: Update bookmark internal element
                    items << item;
                }
            }

            return items;
        }

        void _q_initDeviceList() {
            QubeHardware::DeviceNotifier *notifier = QubeHardware::DeviceNotifier::instance();

            connect(notifier, SIGNAL(deviceAdded(const QString&)),
                    q, SLOT(_q_deviceAdded(const QString&)));
            connect(notifier, SIGNAL(deviceRemoved(const QString&)),
                    q, SLOT(_q_deviceRemoved(const QString&)));

            const QList<QubeHardware::Device> &deviceList = QubeHardware::Device::listFromQuery(predicate);

            foreach(const QubeHardware::Device &device, deviceList) {
                availableDevices << device.udi();
            }

            _q_reloadBookmarks();
        }

        void _q_deviceAdded(const QString &udi) {
            QubeHardware::Device d(udi);

            if (predicate.matches(d)) {
                availableDevices << udi;
                _q_reloadBookmarks();
            }
        }

        void _q_deviceRemoved(const QString &udi) {
            if (availableDevices.contains(udi)) {
                availableDevices.remove(udi);
                _q_reloadBookmarks();
            }
        }

        void _q_itemChanged(const QString &id) {
            for (int row = 0; row<items.size(); ++row) {
                if (items.at(row)->id()==id) {
                    QModelIndex index = q->index(row, 0);
                    emit q->dataChanged(index, index);
                }
            }
        }

        void _q_reloadBookmarks() {
            QList<FilePlacesItem*> currentItems = loadBookmarkList();

            QList<FilePlacesItem*>::Iterator it_i = items.begin();
            QList<FilePlacesItem*>::Iterator it_c = currentItems.begin();

            QList<FilePlacesItem*>::Iterator end_i = items.end();
            QList<FilePlacesItem*>::Iterator end_c = currentItems.end();

            while (it_i!=end_i || it_c!=end_c) {
                if (it_i==end_i && it_c!=end_c) {
                    int row = items.count();

                    q->beginInsertRows(QModelIndex(), row, row);
                    it_i = items.insert(it_i, *it_c);
                    ++it_i;
                    it_c = currentItems.erase(it_c);

                    end_i = items.end();
                    end_c = currentItems.end();
                    q->endInsertRows();

                } else if (it_i!=end_i && it_c==end_c) {
                    int row = items.indexOf(*it_i);

                    q->beginRemoveRows(QModelIndex(), row, row);
                    delete *it_i;
                    it_i = items.erase(it_i);

                    end_i = items.end();
                    end_c = currentItems.end();
                    q->endRemoveRows();

                } else if ((*it_i)->id()==(*it_c)->id()) {
                    bool shouldEmit = !((*it_i)->bookmark()==(*it_c)->bookmark());
                    (*it_i)->setBookmark((*it_c)->bookmark());
                    if (shouldEmit) {
                        int row = items.indexOf(*it_i);
                        QModelIndex idx = q->index(row, 0);
                        emit q->dataChanged(idx, idx);
                    }
                    ++it_i;
                    ++it_c;
                } else if ((*it_i)->id()!=(*it_c)->id()) {
                    int row = items.indexOf(*it_i);

                    if (it_i+1!=end_i && (*(it_i+1))->id()==(*it_c)->id()) { // if the next one matches, it's a remove
                        q->beginRemoveRows(QModelIndex(), row, row);
                        delete *it_i;
                        it_i = items.erase(it_i);

                        end_i = items.end();
                        end_c = currentItems.end();
                        q->endRemoveRows();
                    } else {
                        q->beginInsertRows(QModelIndex(), row, row);
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

        void _q_storageSetupDone(QubeHardware::ErrorType error, QVariant errorData) {
            QPersistentModelIndex index = setupInProgress.take(q->sender());

            if (!index.isValid()) {
                return;
            }

            if (!error) {
                emit q->setupDone(index, true);
            } else {
                if (errorData.isValid()) {
                    emit q->errorMessage(tr("An error occurred while accessing '%1', the system responded: %2")
                                         .arg(q->text(index)).arg(errorData.toString()));
                } else {
                    emit q->errorMessage(tr("An error occurred while accessing '%1'")
                                         .arg(q->text(index)));
                }
                emit q->setupDone(index, false);
            }

        }

        void _q_storageTeardownDone(QubeHardware::ErrorType error, QVariant errorData) {
            if (error && errorData.isValid())
                emit q->errorMessage(errorData.toString());
        }
    };

    FilePlacesModel::FilePlacesModel(QObject *parent) :
        QAbstractItemModel(parent),
        d(new Private(this))
    {
#if GIGI
        const QString file = KStandardDirs::locateLocal("data", "kfileplaces/bookmarks.xml");
#else
        const QString file("~/.local/share/tracker/bookmarks.xml");
#endif
        d->bookmarkManager = BookmarkManager::managerForFile(file, "tracker");

        // Let's put some places in there if it's empty. We have a corner case here:
        // Given you have bookmarked some folders (which have been saved on
        // ~/.local/share/user-places.xbel (according to freedesktop bookmarks spec), and
        // deleted the home directory ~/.kde, the call managerForFile() will return the
        // bookmark manager for the fallback "kfilePlaces", making root.first().isNull() being
        // false (you have your own items bookmarked), resulting on only being added your own
        // bookmarks, and not the default ones too. So, we also check if kfileplaces/bookmarks.xml
        // file exists, and if it doesn't, we also add the default places. (ereslibre)
        BookmarkGroup root = d->bookmarkManager->root();
        if (root.first().isNull() || !QFile::exists(file)) {
            FilePlacesItem::createSystemBookmark(d->bookmarkManager,
                                                 "Home", I18N_NOOP2("KFile System Bookmarks", "Home"),
                                                 QUrl(QDir::homePath()), "user-home");
            FilePlacesItem::createSystemBookmark(d->bookmarkManager,
                                                 "Network", I18N_NOOP2("KFile System Bookmarks", "Network"),
                                                 QUrl("remote:/"), "network-workgroup");
            FilePlacesItem::createSystemBookmark(d->bookmarkManager,
                                                 "Root", I18N_NOOP2("KFile System Bookmarks", "Root"),
                                                 QUrl("/"), "folder-red");
            FilePlacesItem::createSystemBookmark(d->bookmarkManager,
                                                 "Trash", I18N_NOOP2("KFile System Bookmarks", "Trash"),
                                                 QUrl("trash:/"), "user-trash");

            // Force bookmarks to be saved. If on open/save dialog and the bookmarks are not saved, QFile::exists
            // will always return false, which opening/closing all the time the open/save dialog would case the
            // bookmarks to be added once each time, having lots of times each bookmark. This forces the defaults
            // to be saved on the bookmarks.xml file. Of course, the complete list of bookmarks (those that come from
            // user-places.xbel will be filled later). (ereslibre)
            d->bookmarkManager->saveAs(file);
        }

        // create after, so if we have own places, they are added afterwards, in case of equal priorities
        d->sharedBookmarks = new FilePlacesSharedBookmarks(d->bookmarkManager);

        d->predicate = QubeHardware::Predicate::fromString(
                           "[[[[ StorageVolume.ignored == false AND [ StorageVolume.usage == 'FileSystem' OR StorageVolume.usage == 'Encrypted' ]]"
                           " OR "
                           "[ IS StorageAccess AND StorageDrive.driveType == 'Floppy' ]]"
                           " OR "
                           "OpticalDisc.availableContent & 'Audio' ]"
                           " OR "
                           "StorageAccess.ignored == false ]");
        Q_ASSERT(d->predicate.isValid());

        connect(d->bookmarkManager, SIGNAL(changed(const QString&, const QString&)),
                this, SLOT(_q_reloadBookmarks()));
        connect(d->bookmarkManager, SIGNAL(bookmarksChanged(const QString&)),
                this, SLOT(_q_reloadBookmarks()));

        d->_q_reloadBookmarks();
        QTimer::singleShot(0, this, SLOT(_q_initDeviceList()));
    }

    FilePlacesModel::~FilePlacesModel()
    {
        delete d;
    }

    QUrl FilePlacesModel::url(const QModelIndex &index) const
    {
        return QUrl(data(index, UrlRole).toUrl());
    }

    bool FilePlacesModel::setupNeeded(const QModelIndex &index) const
    {
        return data(index, SetupNeededRole).toBool();
    }

    QIcon FilePlacesModel::icon(const QModelIndex &index) const
    {
        return QIcon(data(index, Qt::DecorationRole).value<QIcon>());
    }

    QString FilePlacesModel::text(const QModelIndex &index) const
    {
        return data(index, Qt::DisplayRole).toString();
    }

    bool FilePlacesModel::isHidden(const QModelIndex &index) const
    {
        return data(index, HiddenRole).toBool();
    }

    bool FilePlacesModel::isDevice(const QModelIndex &index) const
    {
        if (!index.isValid())
            return false;

        FilePlacesItem *item = static_cast<FilePlacesItem*>(index.internalPointer());

        return item->isDevice();
    }

    QubeHardware::Device FilePlacesModel::deviceForIndex(const QModelIndex &index) const
    {
        if (!index.isValid())
            return QubeHardware::Device();

        FilePlacesItem *item = static_cast<FilePlacesItem*>(index.internalPointer());

        if (item->isDevice()) {
            return item->device();
        } else {
            return QubeHardware::Device();
        }
    }

    Bookmark FilePlacesModel::bookmarkForIndex(const QModelIndex &index) const
    {
        if (!index.isValid())
            return Bookmark();

        FilePlacesItem *item = static_cast<FilePlacesItem*>(index.internalPointer());

        if (!item->isDevice()) {
            return item->bookmark();
        } else {
            return Bookmark();
        }
    }

    QVariant FilePlacesModel::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid())
            return QVariant();

        FilePlacesItem *item = static_cast<FilePlacesItem*>(index.internalPointer());
        return item->data(role);
    }

    QModelIndex FilePlacesModel::index(int row, int column, const QModelIndex &parent) const
    {
        if (row<0 || column!=0 || row>=d->items.size())
            return QModelIndex();

        if (parent.isValid())
            return QModelIndex();

        return createIndex(row, column, d->items.at(row));
    }

    QModelIndex FilePlacesModel::parent(const QModelIndex &child) const
    {
        Q_UNUSED(child);
        return QModelIndex();
    }

    int FilePlacesModel::rowCount(const QModelIndex &parent) const
    {
        if (parent.isValid())
            return 0;
        else
            return d->items.size();
    }

    int FilePlacesModel::columnCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent)
        // We only know 1 piece of information for a particular entry
        return 1;
    }

    QModelIndex FilePlacesModel::closestItem(const QUrl &url) const
    {
        int foundRow = -1;
        int maxLength = 0;

        // Search the item which is equal to the URL or at least is a parent URL.
        // If there are more than one possible item URL candidates, choose the item
        // which covers the bigger range of the URL.
        for (int row = 0; row<d->items.size(); ++row) {
            FilePlacesItem *item = d->items[row];
            QUrl itemUrl = QUrl(item->data(UrlRole).toUrl());

            if (itemUrl.isParentOf(url)) {
                const int length = itemUrl.toString().length();
                if (length > maxLength) {
                    foundRow = row;
                    maxLength = length;
                }
            }
        }

        if (foundRow==-1)
            return QModelIndex();
        else
            return createIndex(foundRow, 0, d->items[foundRow]);
    }

    Qt::DropActions FilePlacesModel::supportedDropActions() const
    {
        return Qt::ActionMask;
    }

    Qt::ItemFlags FilePlacesModel::flags(const QModelIndex &index) const
    {
        Qt::ItemFlags res = Qt::ItemIsSelectable|Qt::ItemIsEnabled;

        if (index.isValid())
            res|= Qt::ItemIsDragEnabled;

        if (!index.isValid())
            res|= Qt::ItemIsDropEnabled;

        return res;
    }

    static QString _q_internalMimetype(const FilePlacesModel * const self)
    {
        return QString("application/x-kfileplacesmodel-")+QString::number((qptrdiff)self);
    }

    QStringList FilePlacesModel::mimeTypes() const
    {
        QStringList types;

        types << _q_internalMimetype(this) << "text/uri-list";

        return types;
    }

    QMimeData *FilePlacesModel::mimeData(const QModelIndexList &indexes) const
    {
        QList<QUrl> urls;
        QByteArray itemData;

        QDataStream stream(&itemData, QIODevice::WriteOnly);

        foreach (const QModelIndex &index, indexes) {
            QUrl itemUrl = url(index);
            if (itemUrl.isValid())
                urls << itemUrl;
            stream << index.row();
        }

        QMimeData *mimeData = new QMimeData();

#ifdef GIGI
        if (!urls.isEmpty())
            urls.populateMimeData(mimeData);
#endif

        mimeData->setData(_q_internalMimetype(this), itemData);

        return mimeData;
    }

    bool FilePlacesModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
                                       int row, int column, const QModelIndex &parent)
    {
        if (action == Qt::IgnoreAction)
            return true;

        if (column > 0)
            return false;

        if (row==-1 && parent.isValid()) {
            return false; // Don't allow to move an item onto another one,
            // too easy for the user to mess something up
            // If we really really want to allow copying files this way,
            // let's do it in the views to get the good old drop menu
        }


        Bookmark afterBookmark;

        if (row==-1) {
            // The dropped item is moved or added to the last position

            FilePlacesItem *lastItem = d->items.last();
            afterBookmark = lastItem->bookmark();

        } else {
            // The dropped item is moved or added before position 'row', ie after position 'row-1'

            if (row>0) {
                FilePlacesItem *afterItem = d->items[row-1];
                afterBookmark = afterItem->bookmark();
            }
        }

        if (data->hasFormat(_q_internalMimetype(this))) {
            // The operation is an internal move
            QByteArray itemData = data->data(_q_internalMimetype(this));
            QDataStream stream(&itemData, QIODevice::ReadOnly);
            int itemRow;

            stream >> itemRow;

            FilePlacesItem *item = d->items[itemRow];
            Bookmark bookmark = item->bookmark();

            d->bookmarkManager->root().moveBookmark(bookmark, afterBookmark);
#ifdef GIGI
        } else if (data->hasFormat("text/uri-list")) {
            // The operation is an add
            QUrl::List urls = QUrl::List::fromMimeData(data);

            BookmarkGroup group = d->bookmarkManager->root();

            foreach (const QUrl &url, urls) {
                // TODO: use KIO::stat in order to get the UDS_DISPLAY_NAME too
                KMimeType::Ptr mimetype = KMimeType::mimeType(KIO::NetAccess::mimetype(url, 0));

                if (!mimetype) {
                    qWarning() << "URL not added to Places as mimetype could not be determined!";
                    continue;
                }

                if (!mimetype->is("inode/directory")) {
                    // Only directories are allowed
                    continue;
                }

                Bookmark bookmark = FilePlacesItem::createBookmark(d->bookmarkManager,
                                    url.fileName(), url,
                                    mimetype->iconName(url));
                group.moveBookmark(bookmark, afterBookmark);
                afterBookmark = bookmark;
            }
#endif
        } else {
            // Oops, shouldn't happen thanks to mimeTypes()
            qWarning() << ": received wrong mimedata, " << data->formats();
            return false;
        }

        d->reloadAndSignal();

        return true;
    }

    void FilePlacesModel::addPlace(const QString &text, const QUrl &url,
                                   const QString &iconName, const QString &appName)
    {
        addPlace(text, url, iconName, appName, QModelIndex());
    }

    void FilePlacesModel::addPlace(const QString &text, const QUrl &url,
                                   const QString &iconName, const QString &appName,
                                   const QModelIndex &after)
    {
        Bookmark bookmark = FilePlacesItem::createBookmark(d->bookmarkManager,
                            text, url, iconName);

        if (!appName.isEmpty()) {
            bookmark.setMetaDataItem("OnlyInApp", appName);
        }

        if (after.isValid()) {
            FilePlacesItem *item = static_cast<FilePlacesItem*>(after.internalPointer());
            d->bookmarkManager->root().moveBookmark(bookmark, item->bookmark());
        }

        d->reloadAndSignal();
    }

    void FilePlacesModel::editPlace(const QModelIndex &index, const QString &text, const QUrl &url,
                                    const QString &iconName, const QString &appName)
    {
        if (!index.isValid()) return;

        FilePlacesItem *item = static_cast<FilePlacesItem*>(index.internalPointer());

        if (item->isDevice()) return;

        Bookmark bookmark = item->bookmark();

        if (bookmark.isNull()) return;

        bookmark.setFullText(text);
        bookmark.setUrl(url);
        bookmark.setIcon(iconName);
        bookmark.setMetaDataItem("OnlyInApp", appName);

        d->reloadAndSignal();
        emit dataChanged(index, index);
    }

    void FilePlacesModel::removePlace(const QModelIndex &index) const
    {
        if (!index.isValid()) return;

        FilePlacesItem *item = static_cast<FilePlacesItem*>(index.internalPointer());

        if (item->isDevice()) return;

        Bookmark bookmark = item->bookmark();

        if (bookmark.isNull()) return;

        d->bookmarkManager->root().deleteBookmark(bookmark);
        d->reloadAndSignal();
    }

    void FilePlacesModel::setPlaceHidden(const QModelIndex &index, bool hidden)
    {
        if (!index.isValid()) return;

        FilePlacesItem *item = static_cast<FilePlacesItem*>(index.internalPointer());

        Bookmark bookmark = item->bookmark();

        if (bookmark.isNull()) return;

        bookmark.setMetaDataItem("IsHidden", (hidden ? "true" : "false"));

        d->reloadAndSignal();
        emit dataChanged(index, index);
    }

    int FilePlacesModel::hiddenCount() const
    {
        int rows = rowCount();
        int hidden = 0;

        for (int i=0; i<rows; ++i) {
            if (isHidden(index(i, 0)))
                hidden++;
        }

        return hidden;
    }

    QAction *FilePlacesModel::teardownActionForIndex(const QModelIndex &index) const
    {
        QubeHardware::Device device = deviceForIndex(index);

        if (device.is<QubeHardware::StorageAccess>() && device.as<QubeHardware::StorageAccess>()->isAccessible()) {

            QubeHardware::StorageDrive *drive = device.as<QubeHardware::StorageDrive>();

            if (drive==0) {
                drive = device.parent().as<QubeHardware::StorageDrive>();
            }

            bool hotpluggable = false;
            bool removable = false;

            if (drive!=0) {
                hotpluggable = drive->isHotpluggable();
                removable = drive->isRemovable();
            }

            QString iconName;
            QString text;
            QString label = data(index, Qt::DisplayRole).toString().replace('&',"&&");

            if (device.is<QubeHardware::OpticalDisc>()) {
                text = tr("&Release '%1'").arg(label);
            } else if (removable || hotpluggable) {
                text = tr("&Safely Remove '%1'").arg(label);
                iconName = "media-eject";
            } else {
                text = tr("&Unmount '%1'").arg(label);
                iconName = "media-eject";
            }

            if (!iconName.isEmpty())
                return new QAction(QIcon(iconName), text, 0);
            else
                return new QAction(text, 0);
        }

        return 0;
    }

    QAction *FilePlacesModel::ejectActionForIndex(const QModelIndex &index) const
    {
        QubeHardware::Device device = deviceForIndex(index);

        if (device.is<QubeHardware::OpticalDisc>()) {
            QString label = data(index, Qt::DisplayRole).toString().replace('&',"&&");
            QString text = tr("&Eject '%1'").arg(label);

            return new QAction(QIcon("media-eject"), text, 0);
        }

        return 0;
    }

    void FilePlacesModel::requestTeardown(const QModelIndex &index)
    {
        QubeHardware::Device device = deviceForIndex(index);
        QubeHardware::StorageAccess *access = device.as<QubeHardware::StorageAccess>();

        if (access!=0) {
            connect(access, SIGNAL(teardownDone(QubeHardware::ErrorType, QVariant, const QString &)),
                    this, SLOT(_q_storageTeardownDone(QubeHardware::ErrorType, QVariant)));

            access->teardown();
        }
    }

    void FilePlacesModel::requestEject(const QModelIndex &index)
    {
        QubeHardware::Device device = deviceForIndex(index);

        QubeHardware::OpticalDrive *drive = device.parent().as<QubeHardware::OpticalDrive>();

        if (drive!=0) {
            connect(drive, SIGNAL(ejectDone(QubeHardware::ErrorType, QVariant, const QString &)),
                    this, SLOT(_q_storageTeardownDone(QubeHardware::ErrorType, QVariant)));

            drive->eject();
        } else {
            QString label = data(index, Qt::DisplayRole).toString().replace('&',"&&");
            QString message = tr("The device '%1' is not a disk and cannot be ejected.").arg(label);
            emit errorMessage(message);
        }
    }

    void FilePlacesModel::requestSetup(const QModelIndex &index)
    {
        QubeHardware::Device device = deviceForIndex(index);

        if (device.is<QubeHardware::StorageAccess>()
            && !d->setupInProgress.contains(device.as<QubeHardware::StorageAccess>())
            && !device.as<QubeHardware::StorageAccess>()->isAccessible()) {

            QubeHardware::StorageAccess *access = device.as<QubeHardware::StorageAccess>();

            d->setupInProgress[access] = index;

            connect(access, SIGNAL(setupDone(QubeHardware::ErrorType, QVariant, const QString &)),
                    this, SLOT(_q_storageSetupDone(QubeHardware::ErrorType, QVariant)));

            access->setup();
        }
    }
}
