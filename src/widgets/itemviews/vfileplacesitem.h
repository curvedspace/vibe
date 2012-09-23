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
