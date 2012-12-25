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

#include <QDebug>

#include "vaccountsmanager.h"
#include "vaccountsmanager_p.h"
#include "vuseraccount.h"

#include <unistd.h>

/*
 * VAccountsManagerPrivate
 */

VAccountsManagerPrivate::VAccountsManagerPrivate(VAccountsManager *self)
    : q_ptr(self)
{
    interface = new OrgFreedesktopAccountsInterface(
                "org.freedesktop.Accounts",
                "/org/freedesktop/Accounts",
                QDBusConnection::systemBus());
}

VAccountsManagerPrivate::~VAccountsManagerPrivate()
{
    delete interface;
}

void VAccountsManagerPrivate::_q_userAdded(const QDBusObjectPath &path)
{
    Q_Q(VAccountsManager);
    emit q->userAdded(new VUserAccount(path.path()));
}

void VAccountsManagerPrivate::_q_userDeleted(const QDBusObjectPath &path)
{
    Q_Q(VAccountsManager);
    emit q->userDeleted(new VUserAccount(path.path()));
}

/*!
    \class VAccountsManager
    \ingroup core Core Kit

    \brief VAccountsManager provides the backend for user management.

    The VAccountsManager class allows for querying and manipiulating user
    account information.

    \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
*/

/*!
    Constructs a VAccountsManager object.
*/
VAccountsManager::VAccountsManager()
    : d_ptr(new VAccountsManagerPrivate(this))
{
    connect(d_ptr->interface, SIGNAL(UserAdded(QDBusObjectPath)),
            this, SLOT(_q_userAdded(QDBusObjectPath)));
    connect(d_ptr->interface, SIGNAL(UserDeleted(QDBusObjectPath)),
            this, SLOT(_q_userDeleted(QDBusObjectPath)));
}

/*!
    Destroy a VAccountsManager object.
*/
VAccountsManager::~VAccountsManager()
{
    delete d_ptr;
}

/*!
    Caches a user account, so that it shows up in listCachedUsers() output.
    The user name may be a remote user, but the system must be able to lookup
    the user name and resolve the user information.

    \param userName The user name for the user.
*/
VUserAccount *VAccountsManager::cacheUser(const QString &userName)
{
    Q_D(VAccountsManager);

    QDBusPendingReply<QDBusObjectPath> reply = d->interface->CacheUser(userName);
    reply.waitForFinished();

    if (reply.isError()) {
        QDBusError error = reply.error();
        qWarning("Couldn't cache user %s: %s",
                 userName.toUtf8().constData(),
                 error.errorString(error.type()).toUtf8().constData());
        return 0;
    }

    QDBusObjectPath path = reply.argumentAt<0>();
    return new VUserAccount(path.path());
}

/*!
    Releases all metadata about a user account, including icon, language and session.
    If the user account is from a remote server and the user has never logged in
    before, then that account will no longer show up in listCachedUsers() output.

    \param userName The user name for the user.
*/
void VAccountsManager::uncacheUser(const QString &userName)
{
    Q_D(VAccountsManager);

    d->interface->UncacheUser(userName);
}

/*!
    Releases all metadata about a user account, including icon, language and session.
    If the user account is from a remote server and the user has never logged in
    before, then that account will no longer show up in listCachedUsers() output.

    \param account The account for the user.
*/
void VAccountsManager::uncacheUser(VUserAccount *account)
{
    return uncacheUser(account->userName());
}

/*!
    Returns a list of user accounts.

    \param systemUsers If true, returns also system users.
*/
VUserAccountList VAccountsManager::listCachedUsers()
{
    Q_D(VAccountsManager);

    VUserAccountList list;

    QDBusPendingReply<QList<QDBusObjectPath>> reply = d->interface->ListCachedUsers();
    reply.waitForFinished();

    if (reply.isError()) {
        QDBusError error = reply.error();
        qWarning("Couldn't list cached users: %s",
                 error.errorString(error.type()).toUtf8().constData());
        return list;
    }

    QList<QDBusObjectPath> value = reply.argumentAt<0>();
    for (int i = 0; i < value.size(); i++)
        list.append(new VUserAccount(value.at(i).path()));

    return list;
}

/*!
    Returns the currently logged in user.
    \return a VUserAccount object representing the currently logged
            in user.
*/
VUserAccount *VAccountsManager::defaultUser()
{
    return findUserById(getuid());
}

/*!
    Finds a user by \a uid.

    \param uid The uid to look up.
    \return the corresponding VUserAccount object.
*/
VUserAccount *VAccountsManager::findUserById(uid_t uid)
{
    Q_D(VAccountsManager);

    QDBusPendingReply<QDBusObjectPath> reply = d->interface->FindUserById(uid);
    reply.waitForFinished();

    if (reply.isError()) {
        QDBusError error = reply.error();
        qWarning("Couldn't find user by uid %d: %s", uid,
                 error.errorString(error.type()).toUtf8().constData());
        return 0;
    }

    QDBusObjectPath path = reply.argumentAt<0>();
    return new VUserAccount(path.path());
}

/*!
    Finds a user by user \a userName

    \param uid The user name to look up.
    \return the corresponding VUserAccount object.
*/
VUserAccount *VAccountsManager::findUserByName(const QString &userName)
{
    Q_D(VAccountsManager);

    QDBusPendingReply<QDBusObjectPath> reply = d->interface->FindUserByName(userName);
    reply.waitForFinished();

    if (reply.isError()) {
        QDBusError error = reply.error();
        qWarning("Couldn't find user by user name %s: %s",
                 userName.toUtf8().constData(),
                 error.errorString(error.type()).toUtf8().constData());
        return 0;
    }

    QDBusObjectPath path = reply.argumentAt<0>();
    return new VUserAccount(path.path());
}

/*!
    Creates a new \a accountType type user account whose name is \a userName,
    real name is \a fullName.

    \param userName The name of the new user to be created.
    \param fullName First name and last name.
    \param accountType The account type.
    \return whether the user was created successfully.
*/
bool VAccountsManager::createUser(const QString &userName,
                                  const QString &fullName,
                                  VUserAccount::AccountType accountType)
{
    Q_D(VAccountsManager);

    QDBusPendingReply<QDBusObjectPath> reply = d->interface->CreateUser(userName, fullName, accountType);
    if (reply.isError()) {
        QDBusError error = reply.error();
        qWarning("Couldn't create user %s: %s", userName.toUtf8().constData(),
                 error.errorString(error.type()).toUtf8().constData());
        return false;
    }

    return true;
}

/*!
    Deletes the user designated by \a uid.

    \param uid The user identifier.
    \param removeFiles If true all files owned by the user will be removed.
    \return whether the user was deleted successfully.
*/
bool VAccountsManager::deleteUser(uid_t uid, bool removeFiles)
{
    Q_D(VAccountsManager);

    QDBusPendingReply<QDBusObjectPath> reply = d->interface->DeleteUser(uid, removeFiles);
    if (reply.isError()) {
        QDBusError error = reply.error();
        qWarning("Couldn't delete user %d: %s", uid,
                 error.errorString(error.type()).toUtf8().constData());
        return false;
    }

    return true;
}

#include "moc_vaccountsmanager.cpp"
