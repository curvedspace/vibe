/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef VUSERACCOUNT_H
#define VUSERACCOUNT_H

#include <VibeCore/VGlobal>

class VAccountsManager;
class VUserAccountPrivate;

class VIBE_EXPORT VUserAccount
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
