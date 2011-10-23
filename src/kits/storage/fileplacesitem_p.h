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

#ifndef QUBE_FILEPLACESITEM_P_H
#define QUBE_FILEPLACESITEM_P_H

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QModelIndex>

#include <Qube/Hardware/Device>

#include "bookmark.h"

namespace Qube
{
    namespace Hardware
    {
        class StorageAccess;
        class StorageVolume;
        class OpticalDisc;
    }

    namespace Storage
    {
        class BookmarkManager;
        class FilePlacesItem : public QObject
        {
            Q_OBJECT
        public:
            FilePlacesItem(BookmarkManager *manager,
                           const QString &address,
                           const QString &udi = QString());
            ~FilePlacesItem();

            QString id() const;

            bool isDevice() const;
            Bookmark bookmark() const;
            void setBookmark(const Bookmark &bookmark);
            Qube::Hardware::Device device() const;
            QVariant data(int role) const;

            static Bookmark createBookmark(BookmarkManager *manager,
                                           const QString &label,
                                           const QUrl &url,
                                           const QString &iconName,
                                           FilePlacesItem *after = 0);
            static Bookmark createSystemBookmark(BookmarkManager *manager,
                                                 const QString &untranslatedLabel,
                                                 const QString &translatedLabel,
                                                 const QUrl &url,
                                                 const QString &iconName);
            static Bookmark createDeviceBookmark(BookmarkManager *manager,
                                                 const QString &udi);

        Q_SIGNALS:
            void itemChanged(const QString &id);

        private Q_SLOTS:
            void onAccessibilityChanged();
            void onListerCompleted();

        private:
            QVariant bookmarkData(int role) const;
            QVariant deviceData(int role) const;

            bool hasFullIcon(const Bookmark &bookmark) const;
            QString iconNameForBookmark(const Bookmark &bookmark) const;

            static QString generateNewId();

            BookmarkManager *m_manager;
            Bookmark m_bookmark;
#ifdef GIGI
            KDirLister *m_lister;
#endif
            bool m_folderIsEmpty;
            QString m_text;
            mutable Qube::Hardware::Device m_device;
            mutable QPointer<Qube::Hardware::StorageAccess> m_access;
            mutable QPointer<Qube::Hardware::StorageVolume> m_volume;
            mutable QPointer<Qube::Hardware::OpticalDisc> m_disc;
        };
    }
}

#endif // QUBE_FILEPLACESITEM_P_H
