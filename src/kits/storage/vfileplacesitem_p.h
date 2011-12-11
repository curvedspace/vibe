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

#ifndef VFILEPLACESITEM_P_H
#define VFILEPLACESITEM_P_H

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QModelIndex>

#include <VibeCore/VGlobal>
#include <VibeHardware/VDevice>

#include "vbookmark.h"

class VStorageAccess;
class VStorageVolume;
class VOpticalDisc;

class VBookmarkManager;

class VIBE_EXPORT VFilePlacesItem : public QObject
{
    Q_OBJECT
public:
    VFilePlacesItem(VBookmarkManager *manager,
                    const QString &address,
                    const QString &udi = QString());
    ~VFilePlacesItem();

    QString id() const;

    bool isDevice() const;
    VBookmark bookmark() const;
    void setBookmark(const VBookmark &bookmark);
    VDevice device() const;
    QVariant data(int role) const;

    static VBookmark createBookmark(VBookmarkManager *manager,
                                    const QString &label,
                                    const QUrl &url,
                                    const QString &iconName,
                                    VFilePlacesItem *after = 0);
    static VBookmark createSystemBookmark(VBookmarkManager *manager,
                                          const QString &untranslatedLabel,
                                          const QString &translatedLabel,
                                          const QUrl &url,
                                          const QString &iconName);
    static VBookmark createDeviceBookmark(VBookmarkManager *manager,
                                          const QString &udi);

Q_SIGNALS:
    void itemChanged(const QString &id);

private Q_SLOTS:
    void onAccessibilityChanged();
    void onListerCompleted();

private:
    QVariant bookmarkData(int role) const;
    QVariant deviceData(int role) const;

    bool hasFullIcon(const VBookmark &bookmark) const;
    QString iconNameForBookmark(const VBookmark &bookmark) const;

    static QString generateNewId();

    VBookmarkManager *m_manager;
    VBookmark m_bookmark;
#ifdef GIGI
    KDirLister *m_lister;
#endif
    bool m_folderIsEmpty;
    QString m_text;
    mutable VDevice m_device;
    mutable QPointer<VStorageAccess> m_access;
    mutable QPointer<VStorageVolume> m_volume;
    mutable QPointer<VOpticalDisc> m_disc;
};

#endif // VFILEPLACESITEM_P_H
