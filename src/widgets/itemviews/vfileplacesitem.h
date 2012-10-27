/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2007 Kevin Ottens
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Kevin Ottens <ervin@kde.org>
 *
 * $BEGIN_LICENSE:LGPL2$
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef VFILEPLACESITEM_P_H
#define VFILEPLACESITEM_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Vibe API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QPointer>

#include <solid/device.h>

#include <VibeCore/VBookmarkManager>

class QUrl;
class QIcon;

namespace Solid {
    class StorageAccess;
    class StorageVolume;
    class OpticalDisc;
}

namespace VPrivate
{
    class FilePlacesItem : public QObject
    {
        Q_OBJECT
    public:
        // Constructor for top-level items
        explicit FilePlacesItem(const QIcon &icon, const QString &text,
                                FilePlacesItem *parent = 0);

        // Constructor for items
        explicit FilePlacesItem(VBookmarkManager *manager,
                                const QString &address,
                                FilePlacesItem *parent = 0);

        // Constructor for devices items
        explicit FilePlacesItem(VBookmarkManager *manager,
                                const QString &address,
                                const QString &udi = QString(),
                                FilePlacesItem *parent = 0);

        ~FilePlacesItem();

        VBookmark bookmark() const;
        void setBookmark(const VBookmark &bookmark);

        bool isTopLevel() const;

        bool isDevice() const;
        Solid::Device device() const;

        QString id() const;

        void appendChild(FilePlacesItem *item);

        FilePlacesItem *childAt(int row);
        int childCount() const;

        QVariant data(int role) const;

        int row() const;

        FilePlacesItem *parent();

        static VBookmark createBookmark(VBookmarkManager *manager,
                                        const QString &label,
                                        const QUrl &url,
                                        const QString &iconName,
                                        FilePlacesItem *after = 0);
        static VBookmark createSystemBookmark(VBookmarkManager *manager,
                                              const QString &untranslatedLabel,
                                              const QString &translatedLabel,
                                              const QUrl &url,
                                              const QString &iconName);
        static VBookmark createDeviceBookmark(VBookmarkManager *manager,
                                              const QString &udi);

    signals:
        void itemChanged(const QString &id);

    private:
        VBookmarkManager *m_manager;
        VBookmark m_bookmark;

        QIcon m_icon;
        QString m_text;
        QString m_address;
        bool m_isTopLevel;
        bool m_folderIsEmpty;

        bool m_isDevice;
        mutable Solid::Device m_device;
        mutable QPointer<Solid::StorageAccess> m_access;
        mutable QPointer<Solid::StorageVolume> m_volume;
        mutable QPointer<Solid::OpticalDisc> m_disc;

        QList<FilePlacesItem *> m_childItems;
        FilePlacesItem *m_parentItem;

        static QString generateNewId();

        QVariant deviceData(int role) const;
        QVariant bookmarkData(int role) const;

        bool hasFullIcon(const VBookmark &bookmark) const;
        QString iconNameForBookmark(const VBookmark &bookmark) const;

    private slots:
        void slotAccessibilityChanged();
    };
}

#endif // VFILEPLACESITEM_P_H
