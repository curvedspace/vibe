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

#ifndef VACCOUNTSMANAGER_H
#define VACCOUNTSMANAGER_H

#include <QList>

#include <VibeCore/VGlobal>
#include <VibeCore/VUserAccountList>

class VIBE_EXPORT VAccountsManager : public QObject
{
    Q_OBJECT
public:
    explicit VAccountsManager();

    VUserAccountList listUsers(bool systemUsers);

    VUserAccount *findUserById(uid_t uid);
    VUserAccount *findUserByName(const QString &userName);

    VUserAccount *createUser(const QString &userName);

    bool deleteUser(uid_t uid, bool removeFiles);
    bool deleteUser(const QString &userName, bool removeFiles);
    bool deleteUser(VUserAccount *user, bool removeFiles);

signals:
    void userAdded(VUserAccount *);
    void userDeleted(VUserAccount *);

private:
    uid_t minimalUid() const;
};

#endif // VACCOUNTSMANAGER_H
