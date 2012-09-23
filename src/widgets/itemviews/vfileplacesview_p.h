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
