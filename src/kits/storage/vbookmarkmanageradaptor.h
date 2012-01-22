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
 * the Free Software Foundation, either version 2.1 of the License, or
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

#ifndef VBOOKMARKMANAGERADAPTOR_H
#define VBOOKMARKMANAGERADAPTOR_H

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

#include <QtDBus>

class VBookmarkManager;

#define VIBE_BOOKMARKMANAGER_DBUS_INTERFACE "org.vision.BookmarkManager"

class VBookmarkManagerAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.vision.BookmarkManager")
public:
    explicit VBookmarkManagerAdaptor(VBookmarkManager *parent);

signals:
    void bookmarkCompleteChange(QString caller);
    void bookmarksChanged(QString groupAddress);
    void bookmarkConfigChanged();

public slots:
    void notifyCompleteChange();
};

#endif // VBOOKMARKMANAGERADAPTOR_H
