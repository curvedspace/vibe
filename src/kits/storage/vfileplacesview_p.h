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

#include "private/fileplacesviewdelegate.h"

class QUrl;
class QPersistentModelIndex;
class QTimeLine;

class VStorageAccess;

class VFilePlacesViewPrivate
{
    Q_DECLARE_PUBLIC(VFilePlacesView)
public:
    explicit VFilePlacesViewPrivate(VFilePlacesView *parent);

    QUrl currentUrl;
    bool showAll;

    VStorageAccess *lastClickedStorage;
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
