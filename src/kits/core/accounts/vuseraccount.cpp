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

#include "vuseraccount.h"
#include "vuseraccount_p.h"

/*
 * VUserAccountPrivate
 */

VUserAccountPrivate::VUserAccountPrivate() :
    uid(-1),
    gid(-1)
{
}

/*!
    \class VUserAcccount
    \ingroup core Core Kit

    \brief The VUserAccount class describes user accounts.

    This class describes user accounts retrieved with VAccountsManager.

    \sa VAccountsManager

    \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
*/

/*!
    Constructs a VUserAccount object initialized with default property values
    that indicates an invalid user account.
*/
VUserAccount::VUserAccount() :
    d_ptr(new VUserAccountPrivate)
{
}

/*!
    Destructor.
*/
VUserAccount::~VUserAccount()
{
    delete d_ptr;
}

/*!
    Returns true if the VUserAccount object contains valid data, otherwise
    returns false.
    A valid object has positive uid() and gid() and non-empty userName().
*/
bool VUserAccount::isValid() const
{
    Q_D(const VUserAccount);
    return (d->uid >= 0 && d->gid >= 0 && !d.userName.isEmpty());
}

/*!
    Returns the user identifier.
*/
uid_t VUserAccount::userId() const
{
    Q_D(const VUserAccount);
    return d->uid;
}

/*!
    Returns the group identifier.
*/
gid_t VUserAccount::groupId() const
{
    Q_D(const VUserAccount);
    return d->gid;
}

/*!
    Returns the user name.
*/
QString VUserAccount::userName() const
{
    Q_D(const VUserAccount);
    return d->userName;
}

/*!
    Returns user's real name.
*/
QString VUserAccount::realName() const
{
    Q_D(const VUserAccount);
    return d->realName;
}

/*!
    Returns user's real name if not empty, otherwise the user name.
*/
QString VUserAccount::displayName() const
{
    Q_D(const VUserAccount);
    if (d->realName.isEmpty())
        return d->userName;
    return d->realName;
}

/*!
    Returns the home directory absolute path.
*/
QString VUserAccount::homeDirectory() const
{
    Q_D(const VUserAccount);
    return d->homeDirectory;
}

/*!
    Returns the login shell absolute path.
*/
QString VUserAccount::shell() const
{
    Q_D(const VUserAccount);
    return d->shell;
}

/*!
    Returns user's picture absoulte path.
*/
QString VUserAccount::imageFileName() const
{
    Q_D(const VUserAccount);
    if (!d->homeDirectory.isEmpty())
        return QString("%1/.face").arg(d->homeDirectory);
    return QStringLiteral("");
}

/*!
    Retutns whether this user is logged in or not.
*/
bool VUserAccount::isLoggedIn() const
{
    // TODO: Not yet implemented
    return false;
}
