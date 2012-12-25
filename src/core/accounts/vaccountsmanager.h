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

#ifndef VACCOUNTSMANAGER_H
#define VACCOUNTSMANAGER_H

#include <QList>

#include <VibeCore/VibeCoreExport>
#include <VibeCore/VUserAccountList>

class VAccountsManagerPrivate;

class VIBECORE_EXPORT VAccountsManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(VAccountsManager)
    Q_PROPERTY(VUserAccount *defaultUser READ defaultUser CONSTANT)
public:
    explicit VAccountsManager();
    ~VAccountsManager();

    VUserAccount *cacheUser(const QString &userName);
    void uncacheUser(const QString &userName);
    void uncacheUser(VUserAccount *account);

    VUserAccountList listCachedUsers();

    VUserAccount *defaultUser();

    VUserAccount *findUserById(uid_t uid);
    VUserAccount *findUserByName(const QString &userName);

    bool createUser(const QString &userName,
                    const QString &fullName,
                    VUserAccount::AccountType accountType);
    bool deleteUser(uid_t uid, bool removeFiles);

signals:
    void userAdded(VUserAccount *);
    void userDeleted(VUserAccount *);

private:
    VAccountsManagerPrivate *const d_ptr;

    uid_t minimalUid() const;
};

#endif // VACCOUNTSMANAGER_H
