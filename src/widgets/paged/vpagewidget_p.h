/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 * Copyright (C) 2007 Matthias Kretz
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Matthias Kretz <kretz@kde.org>
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

#ifndef VPAGEWIDGET_P_H
#define VPAGEWIDGET_P_H

#include "vpageview_p.h"

class VPageWidgetModel;

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

class VPageWidgetPrivate : public VPageViewPrivate
{
    Q_DECLARE_PUBLIC(VPageWidget)
protected:
    VPageWidgetPrivate(VPageWidget *q);

    VPageWidgetModel *model() const {
        return static_cast<VPageWidgetModel *>(VPageViewPrivate::model);
    }

    void _q_slotCurrentPageChanged(const QModelIndex &, const QModelIndex &);
};

#endif // VPAGEWIDGET_P_H
