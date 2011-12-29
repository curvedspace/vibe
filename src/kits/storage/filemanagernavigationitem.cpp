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

#include <QIcon>
#include <QUrl>
#include <QDateTime>

#include <VibeHardware/VDevice>
#include <VibeHardware/VStorageAccess>
#include <VibeHardware/VStorageVolume>
#include <VibeHardware/VStorageDrive>
#include <VibeHardware/VBlock>
#include <VibeHardware/VOpticalDisc>

#include "filemanagernavigationitem.h"

FileManagerNavigationItem::FileManagerNavigationItem(const QIcon &icon,
                                                     const QString &text,
                                                     FileManagerNavigationItem *parent) :
    QObject(parent),
    m_manager(0),
    m_icon(icon),
    m_text(text),
    m_isTopLevel(true),
    m_folderIsEmpty(true),
    m_isDevice(false),
    m_parentItem(parent)
{
    // Automatically append this item to its parent
    if (m_parentItem)
        m_parentItem->appendChild(this);
}

FileManagerNavigationItem::FileManagerNavigationItem(VBookmarkManager *manager,
                                                     const QString &address,
                                                     FileManagerNavigationItem *parent) :
    QObject(parent),
    m_manager(manager),
    m_address(address),
    m_isTopLevel(false),
    m_folderIsEmpty(true),
    m_isDevice(false),
    m_parentItem(parent)
{
    setBookmark(m_manager->findByAddress(address));

    if (m_bookmark.metaDataItem("ID").isEmpty())
        m_bookmark.setMetaDataItem("ID", generateNewId());

    // Automatically append this item to its parent
    if (m_parentItem)
        m_parentItem->appendChild(this);
}

FileManagerNavigationItem::FileManagerNavigationItem(VBookmarkManager *manager,
                                                     const QString &address,
                                                     const QString &udi,
                                                     FileManagerNavigationItem *parent) :
    QObject(parent),
    m_manager(manager),
    m_address(address),
    m_isTopLevel(false),
    m_folderIsEmpty(true),
    m_isDevice(true),
    m_device(udi),
    m_parentItem(parent)
{
    setBookmark(m_manager->findByAddress(address));

    if (udi.isEmpty()) {
        if (hasFullIcon(m_bookmark)) {
            // xx
        }
    } else if (!udi.isEmpty() && m_device.isValid()) {
        m_access = m_device.as<VStorageAccess>();
        m_volume = m_device.as<VStorageVolume>();
        m_disc = m_device.as<VOpticalDisc>();

        if (m_access)
            connect(m_access, SIGNAL(accessibilityChanged(bool, const QString &)),
                    this, SLOT(slotAccessibilityChanged()));
    }

    // Automatically append this item to its parent
    if (m_parentItem)
        m_parentItem->appendChild(this);
}

FileManagerNavigationItem::~FileManagerNavigationItem()
{
    qDeleteAll(m_childItems);
}

VBookmark FileManagerNavigationItem::bookmark() const
{
    return m_bookmark;
}

void FileManagerNavigationItem::setBookmark(const VBookmark &bookmark)
{
    m_bookmark = bookmark;

    if (bookmark.metaDataItem("IsSystemItem") == "true") {
        // Ensure the right string is picked from the catalog every time
        m_text = QT_TRANSLATE_NOOP_UTF8("Places Bookmarks", bookmark.text().toUtf8().data());
    } else {
        // Just use bookmark's text
        m_text = bookmark.text();
    }
}

bool FileManagerNavigationItem::isTopLevel() const
{
    return m_isTopLevel;
}

bool FileManagerNavigationItem::isDevice() const
{
    return m_isDevice;
}

VDevice FileManagerNavigationItem::device() const
{
    if (m_device.udi().isEmpty()) {
        m_device = VDevice(bookmark().metaDataItem("UDI"));
        if (m_device.isValid()) {
            m_access = m_device.as<VStorageAccess>();
            m_volume = m_device.as<VStorageVolume>();
            m_disc = m_device.as<VOpticalDisc>();
        } else {
            m_access = 0;
            m_volume = 0;
            m_disc = 0;
        }
    }

    return m_device;
}

QString FileManagerNavigationItem::id() const
{
    if (isDevice())
        return m_bookmark.metaDataItem("UDI");
    return m_bookmark.metaDataItem("ID");
}

void FileManagerNavigationItem::appendChild(FileManagerNavigationItem *item)
{
    m_childItems.append(item);
}

FileManagerNavigationItem *FileManagerNavigationItem::child(int row)
{
    return m_childItems.value(row);
}

int FileManagerNavigationItem::childCount() const
{
    return m_childItems.size();
}

QVariant FileManagerNavigationItem::data(int role) const
{
    if (isTopLevel()) {
        switch (role) {
            case Qt::DecorationRole:
                return m_icon;
            case Qt::DisplayRole:
                return m_text;
            default:
                return QVariant();
        }
    }

    if (isDevice())
        return deviceData(role);

    return bookmarkData(role);
}

int FileManagerNavigationItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<FileManagerNavigationItem *>(this));
    return 0;
}

FileManagerNavigationItem *FileManagerNavigationItem::parent()
{
    return m_parentItem;
}

VBookmark FileManagerNavigationItem::createBookmark(VBookmarkManager *manager,
                                                    const QString &label,
                                                    const QUrl &url,
                                                    const QString &iconName,
                                                    FileManagerNavigationItem *after)
{
    Q_ASSERT(manager);

    VBookmarkGroup root = manager->root();
    if (root.isNull())
        return VBookmark();

    QString emptyIcon = iconName;

    if (url == QUrl("trash:///")) {
        if (emptyIcon.endsWith(QLatin1String("-full")))
            emptyIcon.chop(5);
        else if (emptyIcon.isEmpty())
            emptyIcon = "user-trash";
    }

    VBookmark bookmark = root.addBookmark(label, url, emptyIcon);
    bookmark.setMetaDataItem("ID", generateNewId());

    if (after)
        root.moveBookmark(bookmark, after->bookmark());

    return bookmark;
}

VBookmark FileManagerNavigationItem::createSystemBookmark(VBookmarkManager *manager,
                                                          const QString &untranslatedLabel,
                                                          const QString &translatedLabel,
                                                          const QUrl &url,
                                                          const QString &iconName)
{
    // System boomarks created with this method are special locations
    // such as Desktop, Home, Documents, Music, ...
    // We always create these bookmarks with the untranslated label, the
    // translatedLabel parameter is needed to force the caller providing a
    // translation which will be picked when the item is shown while we store
    // the untranslated label on disk
    Q_UNUSED(translatedLabel);

    VBookmark bookmark = createBookmark(manager, untranslatedLabel, url, iconName);
    if (!bookmark.isNull())
        bookmark.setMetaDataItem("IsSystemItem", "true");
    return bookmark;
}

VBookmark FileManagerNavigationItem::createDeviceBookmark(VBookmarkManager *manager,
                                                          const QString &udi)
{
    Q_ASSERT(manager);

    VBookmarkGroup root = manager->root();
    if (root.isNull())
        return VBookmark();

    VBookmark bookmark = root.createNewSeparator();
    bookmark.setMetaDataItem("UDI", udi);
    bookmark.setMetaDataItem("IsSystemItem", "true");
    return bookmark;
}

QString FileManagerNavigationItem::generateNewId()
{
    static int count = 0;

    return QString::number(QDateTime::currentDateTime().toTime_t())
           + '/' + QString::number(count++);
}

QVariant FileManagerNavigationItem::deviceData(int role) const
{
    VDevice d = device();

    if (!d.isValid())
        return QVariant();

    switch (role) {
        case Qt::DecorationRole:
            return QIcon::fromTheme(d.icon());
        case Qt::DisplayRole:
            return d.description();
        case UrlRole:
            if (m_access)
                return QUrl::fromLocalFile(m_access->filePath());
            else if (m_disc && (m_disc->availableContent() && VOpticalDisc::Audio) != 0) {
                QString device = d.as<VBlock>()->device();
                return QUrl(QString("audiocd:///?device=%1").arg(device));
            }
        case FixedDeviceRole: {
            VStorageDrive *drive = 0;
            VDevice parentDevice = d;

            while (parentDevice.isValid() && !drive) {
                drive = parentDevice.as<VStorageDrive>();
                parentDevice = parentDevice.parent();
            }

            if (drive)
                return !drive->isHotpluggable() && !drive->isRemovable();
            return true;
        }
        break;
        default:
            break;
    }

    return QVariant();
}

QVariant FileManagerNavigationItem::bookmarkData(int role) const
{
    if (m_bookmark.isNull())
        return QVariant();

    switch (role) {
        case Qt::DecorationRole:
            return QIcon::fromTheme(iconNameForBookmark(m_bookmark));
        case Qt::DisplayRole:
            return m_text;
        case Qt::BackgroundRole:
            if (m_bookmark.metaDataItem("IsHidden") == "true")
                return Qt::lightGray;
        case UrlRole:
            return m_bookmark.url();
        case HiddenRole:
            return m_bookmark.metaDataItem("IsHidden") == "true";
        case SetupNeededRole:
            return false;
        default:
            break;
    }

    return QVariant();
}

bool FileManagerNavigationItem::hasFullIcon(const VBookmark &bookmark) const
{
    return bookmark.url() == QUrl("trash:///");
}

QString FileManagerNavigationItem::iconNameForBookmark(const VBookmark &bookmark) const
{
    if (!m_folderIsEmpty && hasFullIcon(bookmark))
        return bookmark.icon() + "-full";
    return bookmark.icon();
}

void FileManagerNavigationItem::slotAccessibilityChanged()
{
    emit itemChanged(id());
}

#include "filemanagernavigationitem.moc"
