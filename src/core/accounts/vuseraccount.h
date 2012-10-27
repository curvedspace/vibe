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

#ifndef VUSERACCOUNT_H
#define VUSERACCOUNT_H

#include <QtCore/QString>

#include <VibeCore/VibeCoreExport>

class VAccountsManager;
class VUserAccountPrivate;

class VIBECORE_EXPORT VUserAccount
{
public:
    enum AccountType {
        StandardAccountType,
        AdministratorAccountType
    };

    enum PasswordMode {
        RegularPasswordMode,
        SetAtLoginPasswordMode,
        NonePasswordMode
    };

    explicit VUserAccount();
    ~VUserAccount();

    bool isValid() const;

    uid_t userId() const;
    gid_t groupId() const;

    QString userName() const;
    QString realName() const;
    QString displayName() const;
    QString homeDirectory() const;
    QString shell() const;

    QString imageFileName() const;

    bool isLoggedIn() const;

    //QList<VGroup> groups() const;

private:
    friend class VAccountsManager;

    Q_DECLARE_PRIVATE(VUserAccount)

    VUserAccountPrivate *d_ptr;
};

typedef QList<VUserAccount *> VUserAccountList;

#endif // VUSERACCOUNT_H
