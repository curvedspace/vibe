/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1
 * as published by the Free Software Foundation.
 *
 * Vibe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Vibe.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef VLINEEDIT_P_H
#define VLINEEDIT_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the on API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QPixmap>
#include <QMenu>

#include "iconbutton.h"

class VLineEditPrivate : public QObject
{
    Q_DECLARE_PUBLIC(VLineEdit)
public:
    explicit VLineEditPrivate(VLineEdit *parent);

    virtual bool eventFilter(QObject *obj, QEvent *event);

    QPixmap pixmap[2];
    QMenu *menu[2];
    bool menuTabFocusTrigger[2];
    VPrivate::IconButton *iconButton[2];
    bool iconEnabled[2];
    QString oldText;

    void updateMargins();
    void updateButtonPositions();

protected:
    VLineEdit *const q_ptr;
};

#endif // VLINEEDIT_P_H
