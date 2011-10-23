/****************************************************************************
 * This file is part of Qube.
 *
 * Copyright (c) 2007 Kevin Ottens
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *	Kevin Ottens <ervin@kde.org>
 *
 * Qube is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Qube is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Qube.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QtCore/QDateTime>

#include <Qube/Hardware/Block>
#include <Qube/Hardware/OpticalDisc>
#include <Qube/Hardware/StorageAccess>
#include <Qube/Hardware/StorageDriver>

#include "fileplacesitem_p.h"
#include "fileplacesmodel.h"

namespace Qube
{
namespace Storage
{
    FilePlacesItem::FilePlacesItem(BookmarkManager *manager,
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
            m_access = m_device.as<Qube::Hardware::StorageAccess>();
            m_volume = m_device.as<Qube::Hardware::StorageVolume>();
            m_disc = m_device.as<Qube::Hardware::OpticalDisc>();
            if (m_access) {
                connect(m_access, SIGNAL(accessibilityChanged(bool, const QString &)),
                        this, SLOT(onAccessibilityChanged()));
            }
        }
    }

    FilePlacesItem::~FilePlacesItem()
    {
    }

    QString FilePlacesItem::id() const
    {
        if (isDevice()) {
            return bookmark().metaDataItem("UDI");
        } else {
            return bookmark().metaDataItem("ID");
        }
    }

    bool FilePlacesItem::isDevice() const
    {
        return !bookmark().metaDataItem("UDI").isEmpty();
    }

    Bookmark FilePlacesItem::bookmark() const
    {
        return m_bookmark;
    }

    void FilePlacesItem::setBookmark(const Bookmark &bookmark)
    {
        m_bookmark = bookmark;

        if (bookmark.metaDataItem("isSystemItem") == "true") {
            // This context must stay as it is - the translated system bookmark names
            // are created with 'KFile System Bookmarks' as their context, so this
            // ensures the right string is picked from the catalog.
            // (coles, 13th May 2009)

            m_text = i18nc("KFile System Bookmarks", bookmark.text().toUtf8().data());
        } else {
            m_text = bookmark.text();
        }
    }

    Qube::Hardware::Device FilePlacesItem::device() const
    {
        if (m_device.udi().isEmpty()) {
            m_device = Qube::Hardware::Device(bookmark().metaDataItem("UDI"));
            if (m_device.isValid()) {
                m_access = m_device.as<Qube::Hardware::StorageAccess>();
                m_volume = m_device.as<Qube::Hardware::StorageVolume>();
                m_disc = m_device.as<Qube::Hardware::OpticalDisc>();
            } else {
                m_access = 0;
                m_volume = 0;
                m_disc = 0;
            }
        }
        return m_device;
    }

    QVariant FilePlacesItem::data(int role) const
    {
        QVariant returnData;

        if (role!=FilePlacesModel::HiddenRole && role!=Qt::BackgroundRole && isDevice()) {
            returnData = deviceData(role);
        } else {
            returnData = bookmarkData(role);
        }

        return returnData;
    }

    QVariant FilePlacesItem::bookmarkData(int role) const
    {
        Bookmark b = bookmark();

        if (b.isNull()) return QVariant();

        switch (role) {
        case Qt::DisplayRole:
            return m_text;
        case Qt::DecorationRole:
            return KIcon(iconNameForBookmark(b));
        case Qt::BackgroundRole:
            if (b.metaDataItem("IsHidden")=="true") {
                return Qt::lightGray;
            } else {
                return QVariant();
            }
        case FilePlacesModel::UrlRole:
            return QUrl(b.url());
        case FilePlacesModel::SetupNeededRole:
            return false;
        case FilePlacesModel::HiddenRole:
            return b.metaDataItem("IsHidden")=="true";
        default:
            return QVariant();
        }
    }

    QVariant FilePlacesItem::deviceData(int role) const
    {
        Qube::Hardware::Device d = device();

        if (d.isValid()) {
            switch (role) {
            case Qt::DisplayRole:
                return d.description();
            case Qt::DecorationRole:
                return KIcon(d.icon(), 0, d.emblems());
            case FilePlacesModel::UrlRole:
                if (m_access) {
                    return QUrl(KUrl(m_access->filePath()));
                } else if (m_disc && (m_disc->availableContent() & Qube::Hardware::OpticalDisc::Audio)!=0) {
                    QString device = d.as<Qube::Hardware::Block>()->device();
                    return QUrl(QString("audiocd:/?device=%1").arg(device));
                } else {
                    return QVariant();
                }
            case FilePlacesModel::SetupNeededRole:
                if (m_access) {
                    return !m_access->isAccessible();
                } else {
                    return QVariant();
                }

            case FilePlacesModel::FixedDeviceRole: {
                Qube::Hardware::StorageDrive *drive = 0;
                Qube::Hardware::Device parentDevice = m_device;
                while (parentDevice.isValid() && !drive) {
                    drive = parentDevice.as<Qube::Hardware::StorageDrive>();
                    parentDevice = parentDevice.parent();
                }
                if (drive!=0) {
                    return !drive->isHotpluggable() && !drive->isRemovable();
                }
                return true;
            }

            case FilePlacesModel::CapacityBarRecommendedRole: {
                bool accessible = m_access && m_access->isAccessible();
                bool isCdrom =
                    ((m_device.is<Qube::Hardware::StorageDrive>()
                      && m_device.as<Qube::Hardware::StorageDrive>()->driveType() == Qube::Hardware::StorageDrive::CdromDrive)
                     || (m_device.parent().is<Qube::Hardware::StorageDrive>()
                         && m_device.parent().as<Qube::Hardware::StorageDrive>()->driveType() == Qube::Hardware::StorageDrive::CdromDrive));

                return accessible && !isCdrom;
            }

            default:
                return QVariant();
            }
        } else {
            return QVariant();
        }
    }

    Bookmark FilePlacesItem::createBookmark(BookmarkManager *manager,
                                            const QString &label,
                                            const KUrl &url,
                                            const QString &iconName,
                                            FilePlacesItem *after)
    {
        BookmarkGroup root = manager->root();
        if (root.isNull())
            return Bookmark();
        QString empty_icon = iconName;
        if (url==KUrl("trash:/")) {
            if (empty_icon.endsWith(QLatin1String("-full"))) {
                empty_icon.chop(5);
            } else if (empty_icon.isEmpty()) {
                empty_icon = "user-trash";
            }
        }
        Bookmark bookmark = root.addBookmark(label, url, empty_icon);
        bookmark.setMetaDataItem("ID", generateNewId());

        if (after) {
            root.moveBookmark(bookmark, after->bookmark());
        }

        return bookmark;
    }

    Bookmark FilePlacesItem::createSystemBookmark(BookmarkManager *manager,
            const QString &untranslatedLabel,
            const QString &translatedLabel,
            const KUrl &url,
            const QString &iconName)
    {
        Q_UNUSED(translatedLabel); // parameter is only necessary to force the caller
        // providing a translated string for the label

        Bookmark bookmark = createBookmark(manager, untranslatedLabel, url, iconName);
        if (!bookmark.isNull())
            bookmark.setMetaDataItem("isSystemItem", "true");
        return bookmark;
    }


    Bookmark FilePlacesItem::createDeviceBookmark(BookmarkManager *manager,
            const QString &udi)
    {
        BookmarkGroup root = manager->root();
        if (root.isNull())
            return Bookmark();
        Bookmark bookmark = root.createNewSeparator();
        bookmark.setMetaDataItem("UDI", udi);
        bookmark.setMetaDataItem("isSystemItem", "true");
        return bookmark;
    }

    QString FilePlacesItem::generateNewId()
    {
        static int count = 0;

//    return QString::number(count++);

        return QString::number(QDateTime::currentDateTime().toTime_t())
               + '/' + QString::number(count++);


//    return QString::number(QDateTime::currentDateTime().toTime_t())
//         + '/' + QString::number(qrand());
    }

    void FilePlacesItem::onAccessibilityChanged()
    {
        emit itemChanged(id());
    }

    bool FilePlacesItem::hasFullIcon(const Bookmark &bookmark) const
    {
        return bookmark.url() == QUrl("trash:///");
    }

    QString FilePlacesItem::iconNameForBookmark(const Bookmark &bookmark) const
    {
        if (!m_folderIsEmpty && hasFullIcon(bookmark))
            return bookmark.icon() + "-full";
        else
            return bookmark.icon();
    }

    void FilePlacesItem::onListerCompleted()
    {
#ifdef GIGI
        m_folderIsEmpty = m_lister->items().isEmpty();
        emit itemChanged(id());
#endif
    }
}
}
