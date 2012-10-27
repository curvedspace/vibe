/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
 * Copyright (c) 2007 Matthias Kretz
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Matthias Kretz <kretz@kde.org>
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
