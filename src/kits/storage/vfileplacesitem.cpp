/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2007 Kevin Ottens
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *	Kevin Ottens <ervin@kde.org>
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

#include <QtCore/QDateTime>
#include <QtGui/QIcon>

#include <VibeHardware/VBlock>
#include <VibeHardware/VOpticalDisc>
#include <VibeHardware/VStorageAccess>
#include <VibeHardware/VStorageDrive>

#include "vfileplacesitem_p.h"
#include "vfileplacesmodel.h"
#include "vbookmarkmanager.h"

VFilePlacesItem::VFilePlacesItem(VBookmarkManager *manager,
                                 const QString &address,
                                 const QString &udi)
#ifdef GIGI
    : m_manager(manager), m_lister(0), m_folderIsEmpty(true), m_device(udi)
#else
    : m_manager(manager), m_folderIsEmpty(true), m_device(udi)
#endif
{
    setBookmark(m_manager->findByAddress(address));

    if (udi.isEmpty() && m_bookmark.metaDataItem("ID").isEmpty()) {
        m_bookmark.setMetaDataItem("ID", generateNewId());
    } else if (udi.isEmpty()) {
        if (hasFullIcon(m_bookmark)) {
            // FIXME: GIGI
#ifdef GIGI
            // TODO if this is only for the trash, it would be much faster to just read trashrc
            m_lister = new KDirLister(this);
            m_lister->setAutoErrorHandlingEnabled(false, 0); // don't bother the user if trash:/ doesn't exist
            m_lister->setDelayedMimeTypes(true); // we don't need the mimetypes, so don't penalize other KDirLister users
            connect(m_lister, SIGNAL(completed()),
                    this, SLOT(onListerCompleted()));
            m_lister->openUrl(m_bookmark.url());
#endif
        }
    } else if (!udi.isEmpty() && m_device.isValid()) {
        m_access = m_device.as<VStorageAccess>();
        m_volume = m_device.as<VStorageVolume>();
        m_disc = m_device.as<VOpticalDisc>();
        if (m_access) {
            connect(m_access, SIGNAL(accessibilityChanged(bool, const QString &)),
                    this, SLOT(onAccessibilityChanged()));
        }
    }
}

VFilePlacesItem::~VFilePlacesItem()
{
}

QString VFilePlacesItem::id() const
{
    if (isDevice()) {
        return bookmark().metaDataItem("UDI");
    } else {
        return bookmark().metaDataItem("ID");
    }
}

bool VFilePlacesItem::isDevice() const
{
    return !bookmark().metaDataItem("UDI").isEmpty();
}

VBookmark VFilePlacesItem::bookmark() const
{
    return m_bookmark;
}

void VFilePlacesItem::setBookmark(const VBookmark &bookmark)
{
    m_bookmark = bookmark;

    if (bookmark.metaDataItem("isSystemItem") == "true") {
        // This context must stay as it is - the translated system bookmark names
        // are created with 'Places Bookmarks' as their context, so this
        // ensures the right string is picked from the catalog.
        m_text = QT_TRANSLATE_NOOP_UTF8("Places Bookmarks", bookmark.text().toUtf8().data());
    } else {
        m_text = bookmark.text();
    }
}

VDevice VFilePlacesItem::device() const
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

QVariant VFilePlacesItem::data(int role) const
{
    QVariant returnData;

    if (role != VFilePlacesModel::HiddenRole && role != Qt::BackgroundRole && isDevice()) {
        returnData = deviceData(role);
    } else {
        returnData = bookmarkData(role);
    }

    return returnData;
}

QVariant VFilePlacesItem::bookmarkData(int role) const
{
    VBookmark b = bookmark();

    if (b.isNull()) return QVariant();

    switch (role) {
        case Qt::DisplayRole:
            return m_text;
        case Qt::DecorationRole:
            return QIcon::fromTheme(iconNameForBookmark(b));
        case Qt::BackgroundRole:
            if (b.metaDataItem("IsHidden") == "true") {
                return Qt::lightGray;
            } else {
                return QVariant();
            }
        case VFilePlacesModel::UrlRole:
            return QUrl(b.url());
        case VFilePlacesModel::SetupNeededRole:
            return false;
        case VFilePlacesModel::HiddenRole:
            return b.metaDataItem("IsHidden") == "true";
        default:
            return QVariant();
    }
}

QVariant VFilePlacesItem::deviceData(int role) const
{
    VDevice d = device();

    if (d.isValid()) {
        switch (role) {
            case Qt::DisplayRole:
                return d.description();
            case Qt::DecorationRole:
#ifdef GIGI
                return QIcon(d.icon(), 0, d.emblems());
#else
                return QIcon();
#endif
            case VFilePlacesModel::UrlRole:
                if (m_access) {
                    return QUrl(m_access->filePath());
                } else if (m_disc && (m_disc->availableContent() & VOpticalDisc::Audio) != 0) {
                    QString device = d.as<VBlock>()->device();
                    return QUrl(QString("audiocd:/?device=%1").arg(device));
                } else {
                    return QVariant();
                }
            case VFilePlacesModel::SetupNeededRole:
                if (m_access) {
                    return !m_access->isAccessible();
                } else {
                    return QVariant();
                }

            case VFilePlacesModel::FixedDeviceRole: {
                VStorageDrive *drive = 0;
                VDevice parentDevice = m_device;
                while (parentDevice.isValid() && !drive) {
                    drive = parentDevice.as<VStorageDrive>();
                    parentDevice = parentDevice.parent();
                }
                if (drive != 0) {
                    return !drive->isHotpluggable() && !drive->isRemovable();
                }
                return true;
            }

            case VFilePlacesModel::CapacityBarRecommendedRole: {
                bool accessible = m_access && m_access->isAccessible();
                bool isCdrom =
                    ((m_device.is<VStorageDrive>()
                      && m_device.as<VStorageDrive>()->driveType() == VStorageDrive::CdromDrive)
                     || (m_device.parent().is<VStorageDrive>()
                         && m_device.parent().as<VStorageDrive>()->driveType() == VStorageDrive::CdromDrive));

                return accessible && !isCdrom;
            }

            default:
                return QVariant();
        }
    } else {
        return QVariant();
    }
}

VBookmark VFilePlacesItem::createBookmark(VBookmarkManager *manager,
                                          const QString &label,
                                          const QUrl &url,
                                          const QString &iconName,
                                          VFilePlacesItem *after)
{
    VBookmarkGroup root = manager->root();
    if (root.isNull())
        return VBookmark();
    QString empty_icon = iconName;
    if (url == QUrl("trash:/")) {
        if (empty_icon.endsWith(QLatin1String("-full"))) {
            empty_icon.chop(5);
        } else if (empty_icon.isEmpty()) {
            empty_icon = "user-trash";
        }
    }
    VBookmark bookmark = root.addBookmark(label, url, empty_icon);
    bookmark.setMetaDataItem("ID", generateNewId());

    if (after)
        root.moveBookmark(bookmark, after->bookmark());

    return bookmark;
}

VBookmark VFilePlacesItem::createSystemBookmark(VBookmarkManager *manager,
                                                const QString &untranslatedLabel,
                                                const QString &translatedLabel,
                                                const QUrl &url,
                                                const QString &iconName)
{
    Q_UNUSED(translatedLabel); // parameter is only necessary to force the caller
    // providing a translated string for the label

    VBookmark bookmark = createBookmark(manager, untranslatedLabel, url, iconName);
    if (!bookmark.isNull())
        bookmark.setMetaDataItem("isSystemItem", "true");
    return bookmark;
}


VBookmark VFilePlacesItem::createDeviceBookmark(VBookmarkManager *manager,
                                                const QString &udi)
{
    VBookmarkGroup root = manager->root();
    if (root.isNull())
        return VBookmark();
    VBookmark bookmark = root.createNewSeparator();
    bookmark.setMetaDataItem("UDI", udi);
    bookmark.setMetaDataItem("isSystemItem", "true");
    return bookmark;
}

QString VFilePlacesItem::generateNewId()
{
    static int count = 0;

    //    return QString::number(count++);

    return QString::number(QDateTime::currentDateTime().toTime_t())
           + '/' + QString::number(count++);


    //    return QString::number(QDateTime::currentDateTime().toTime_t())
    //         + '/' + QString::number(qrand());
}

void VFilePlacesItem::onAccessibilityChanged()
{
    emit itemChanged(id());
}

bool VFilePlacesItem::hasFullIcon(const VBookmark &bookmark) const
{
    return bookmark.url() == QUrl("trash:///");
}

QString VFilePlacesItem::iconNameForBookmark(const VBookmark &bookmark) const
{
    if (!m_folderIsEmpty && hasFullIcon(bookmark))
        return bookmark.icon() + "-full";
    else
        return bookmark.icon();
}

void VFilePlacesItem::onListerCompleted()
{
#ifdef GIGI
    m_folderIsEmpty = m_lister->items().isEmpty();
    emit itemChanged(id());
#endif
}

#include "vfileplacesitem_p.moc"
