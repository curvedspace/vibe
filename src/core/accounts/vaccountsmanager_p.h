/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef VACCOUNTSMANAGER_P_H
#define VACCOUNTSMANAGER_P_H

#include "accountsinterface.h"

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

class VAccountsManagerPrivate
{
    Q_DECLARE_PUBLIC(VAccountsManager)
public:
    VAccountsManagerPrivate(VAccountsManager *self);
    ~VAccountsManagerPrivate();

    void _q_userAdded(const QDBusObjectPath &path);
    void _q_userDeleted(const QDBusObjectPath &path);

    OrgFreedesktopAccountsInterface *interface;

protected:
    VAccountsManager *const q_ptr;
};

#endif // VACCOUNTSMANAGER_P_H
