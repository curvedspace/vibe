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

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QRegExp>

#include <VibeCore/VStandardDirectories>

#include "vaccountsmanager.h"
#include "vuseraccount_p.h"

#include <pwd.h>
#include <unistd.h>

const uid_t kMinimalUid = 500;

using namespace VStandardDirectories;

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
{
}

/*!
    Returns a list of user accounts.

    \param systemUsers If true, returns also system users.
*/
VUserAccountList VAccountsManager::listUsers(bool systemUsers)
{
    struct passwd pwd, *pwp;
    char buf[4096];
    int i;
    VUserAccountList list;

    setpwent();
    while (1) {
        i = getpwent_r(&pwd, buf, 4096, &pwp);
        if (i)
            break;

        if (systemUsers && pwp->pw_uid < minimalUid())
            continue;

        VUserAccount *user = new VUserAccount();
        user->d_ptr->uid = pwp->pw_uid;
        user->d_ptr->gid = pwp->pw_gid;
        user->d_ptr->userName = QString::fromLocal8Bit(pwp->pw_name);
        user->d_ptr->realName = QString::fromLocal8Bit(pwp->pw_gecos);
        user->d_ptr->homeDirectory = QString::fromLocal8Bit(pwp->pw_dir);
        user->d_ptr->shell = QString::fromLocal8Bit(pwp->pw_shell);
        list.append(user);
    }
    endpwent();

    return list;
}

/*!
    Finds a user by \p uid.

    \param uid The uid to look up.
    \returns the corresponding VUserAccount object.
*/
VUserAccount *VAccountsManager::findUserById(uid_t uid)
{
    struct passwd pwd;
    struct passwd *result;

    size_t bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsize == -1)
        bufsize = 16384;

    char *buf = new char[bufsize];
    if (!buf) {
        qWarning() << "Can't search for" << uid << "user: out of memory!";
    } else {
        int s = getpwuid_r(uid, &pwd, buf, bufsize, &result);
        if (result) {
            VUserAccount *user = new VUserAccount();
            user->d_ptr->uid = uid;
            user->d_ptr->gid = pwd.pw_gid;
            user->d_ptr->userName = QString::fromLocal8Bit(pwd.pw_name);
            user->d_ptr->realName = QString::fromLocal8Bit(pwd.pw_gecos);
            user->d_ptr->homeDirectory = QString::fromLocal8Bit(pwd.pw_dir);
            user->d_ptr->shell = QString::fromLocal8Bit(pwd.pw_shell);
            return user;
        } else {
            if (s == 0)
                qWarning() << "User" << uid << "not found!";
            else
                qWarning() << "Couldn't find user" << uid << ":" << QString(strerror(s));
        }

        delete buf;
    }

    return 0;
}

/*!
    Finds a user by user \a userName

    \param uid The user name to look up.
    \param ok Returns whether the user was found.
*/
VUserAccount *VAccountsManager::findUserByName(const QString &userName)
{
    struct passwd pwd;
    struct passwd *result;

    size_t bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsize == -1)
        bufsize = 16384;

    char *buf = new char[bufsize];
    if (!buf) {
        qWarning() << "Can't search for" << userName << "user: out of memory!";
    } else {
        int s = getpwnam_r(qPrintable(userName), &pwd, buf, bufsize, &result);
        if (result) {
            VUserAccount *user = new VUserAccount();
            user->d_ptr->uid = pwd.pw_uid;
            user->d_ptr->gid = pwd.pw_gid;
            user->d_ptr->userName = userName;
            user->d_ptr->realName = QString::fromLocal8Bit(pwd.pw_gecos);
            user->d_ptr->homeDirectory = QString::fromLocal8Bit(pwd.pw_dir);
            user->d_ptr->shell = QString::fromLocal8Bit(pwd.pw_shell);
            return user;
        } else {
            if (s == 0)
                qWarning() << "User" << userName << "not found!";
            else
                qWarning() << "Couldn't find user" << userName << ":" << QString(strerror(s));
        }

        delete buf;
    }

    return 0;
}

/*!
    Creates a new user account whose name is \a userName.

    If creation was successfull, a VUserAccount instance is return.
    Account details such as the real name can be changed or set calling the
    appropriate methods on such instance.

    \param userName The name of the new user to be created.
    \param ok Returns whether the creation was successfull.
*/
VUserAccount *VAccountsManager::createUser(const QString &userName)
{
    return 0;
}

/*!
    Deletes the user designated by \a uid.

    \param uid The user identifier.
    \param removeFiles If true all files owned by the user will be removed.
*/
bool VAccountsManager::deleteUser(uid_t uid, bool removeFiles)
{
    return false;
}

/*!
    Deletes the user designated by \a userName.

    \param userName The user name.
    \param removeFiles If true all files owned by the user will be removed.
*/
bool VAccountsManager::deleteUser(const QString &userName, bool removeFiles)
{
    return false;
}

/*!
    Deletes a user account.

    \param user User to delete.
    \param removeFiles Whether to remove the user's files.
*/
bool VAccountsManager::deleteUser(VUserAccount *user, bool removeFiles)
{
    return false;
}

uid_t VAccountsManager::minimalUid() const
{
    uid_t uid = kMinimalUid;
    QString fileName = QString("%1/login.defs").arg(findDirectory(
                                                        CommonSettingsDirectory));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return kMinimalUid;

    while (!file.atEnd()) {
        QString line = QString::fromLocal8Bit(file.readLine()).trimmed();

        QRegExp rx("UID_MIN\\s*(\\d+)");
        if (rx.exactMatch(line)) {
            uid = rx.cap(1).toInt();
            break;
        }
    }

    file.close();

    return uid;
}

#include "moc_vaccountsmanager.cpp"
