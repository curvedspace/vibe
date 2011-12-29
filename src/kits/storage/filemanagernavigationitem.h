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

#ifndef FILEMANAGERNAVIGATIONITEM_H
#define FILEMANAGERNAVIGATIONITEM_H

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

#include "vbookmarkmanager.h"

class QUrl;
class QIcon;

class VStorageAccess;
class VStorageVolume;
class VOpticalDisc;

class FileManagerNavigationItem : public QObject
{
    Q_OBJECT
public:
    enum FileManagerNavigationItemRole {
        ActionRole = Qt::UserRole + 1,
        UrlRole = Qt::UserRole + 2,
        HiddenRole = Qt::UserRole + 3,
        SetupNeededRole = Qt::UserRole + 4,
        FixedDeviceRole = Qt::UserRole + 5,
        CapacityBarReccomendedRole = Qt::UserRole + 6
    };

    // Constructor for top-level items
    explicit FileManagerNavigationItem(const QIcon &icon, const QString &text,
                                       FileManagerNavigationItem *parent = 0);

    // Constructor for items
    explicit FileManagerNavigationItem(VBookmarkManager *manager,
                                       const QString &address,
                                       FileManagerNavigationItem *parent = 0);

    // Constructor for devices items
    explicit FileManagerNavigationItem(VBookmarkManager *manager,
                                       const QString &address,
                                       const QString &udi = QString(),
                                       FileManagerNavigationItem *parent = 0);

    ~FileManagerNavigationItem();

    VBookmark bookmark() const;
    void setBookmark(const VBookmark &bookmark);

    bool isTopLevel() const;

    bool isDevice() const;
    VDevice device() const;

    QString id() const;

    void appendChild(FileManagerNavigationItem *item);

    FileManagerNavigationItem *child(int row);

    int childCount() const;

    QVariant data(int role) const;

    int row() const;

    FileManagerNavigationItem *parent();

    static VBookmark createBookmark(VBookmarkManager *manager,
                                    const QString &label,
                                    const QUrl &url,
                                    const QString &iconName,
                                    FileManagerNavigationItem *after = 0);
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
    mutable VDevice m_device;
    mutable QPointer<VStorageAccess> m_access;
    mutable QPointer<VStorageVolume> m_volume;
    mutable QPointer<VOpticalDisc> m_disc;

    QList<FileManagerNavigationItem *> m_childItems;
    FileManagerNavigationItem *m_parentItem;

    static QString generateNewId();

    QVariant deviceData(int role) const;
    QVariant bookmarkData(int role) const;

    bool hasFullIcon(const VBookmark &bookmark) const;
    QString iconNameForBookmark(const VBookmark &bookmark) const;

private slots:
    void slotAccessibilityChanged();
};

#endif // FILEMANAGERNAVIGATIONITEM_H
