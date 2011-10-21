/*  This file is part of the KDE project
    Copyright (C) 2007 Kevin Ottens <ervin@kde.org>

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

#ifndef FILEPLACESITEM_P_H
#define FILEPLACESITEM_P_H

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QModelIndex>

#include <QubeHardware/Device>

#include "bookmark.h"

namespace QubeHardware
{
    class StorageAccess;
    class StorageVolume;
    class OpticalDisc;
}

namespace QubeStorage
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
        QubeHardware::Device device() const;
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
        mutable QubeHardware::Device m_device;
        mutable QPointer<QubeHardware::StorageAccess> m_access;
        mutable QPointer<QubeHardware::StorageVolume> m_volume;
        mutable QPointer<QubeHardware::OpticalDisc> m_disc;
    };
}

#endif // FILEPLACESITEM_P_H
