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

#ifndef VFILEPLACESVIEW_P_H
#define VFILEPLACESVIEW_P_H

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

#include "vfileplacesviewdelegate.h"

class QUrl;
class QPersistentModelIndex;
class QTimeLine;

namespace Solid {
    class StorageAccess;
}

class VFilePlacesViewPrivate
{
    Q_DECLARE_PUBLIC(VFilePlacesView)
public:
    explicit VFilePlacesViewPrivate(VFilePlacesView *parent);

    QUrl currentUrl;
    bool showAll;

    Solid::StorageAccess *lastClickedStorage;
    QPersistentModelIndex lastClickedIndex;

    VPrivate::FilePlacesViewDelegate *delegate;

    QTimeLine itemAppearTimeLine;
    QTimeLine itemDisappearTimeLine;

    VFilePlacesView *const q_ptr;

    void setCurrentIndex(const QModelIndex &index);
    void updateHiddenRows(const QModelIndex &parent);

    void _q_placeClicked(const QModelIndex &index);
    void _q_storageSetupDone(const QModelIndex &index, bool success);
};

#endif // VFILEPLACESVIEW_P_H
