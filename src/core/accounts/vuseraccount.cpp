/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPL-ONLY$
 *
 * This file may be used under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation and
 * appearing in the file LICENSE.LGPL included in the packaging of
 * this file, either version 2.1 of the License, or (at your option) any
 * later version.  Please review the following information to ensure the
 * GNU Lesser General Public License version 2.1 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 * If you have questions regarding the use of this file, please contact
 * us via http://www.maui-project.org/.
 *
 * $END_LICENSE$
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
    \brief The VUserAccount class describes user accounts.

    \ingroup accounts

    This class describes user accounts retrieved with VAccountsManager.

    \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>

    \sa VAccountsManager
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
    return (d->uid >= 0 && d->gid >= 0 && !d->userName.isEmpty());
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
