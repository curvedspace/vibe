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

#include "vuseraccount.h"
#include "vuseraccount_p.h"
#include "userinterface.h"

/*
 * VUserAccountPrivate
 */

VUserAccountPrivate::VUserAccountPrivate()
    : user(0)
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
    Constructs a VUserAccount object for the specified \a uid.

    \param uid User identifier.
*/
VUserAccount::VUserAccount(uid_t uid)
    : d_ptr(new VUserAccountPrivate)
{
    d_ptr->user =
            new OrgFreedesktopAccountsUserInterface("org.freedesktop.Accounts",
                                                    QString("/org/freedesktop/Accounts/User%1").arg(uid),
                                                    QDBusConnection::systemBus(),
                                                    this);
}

/*!
    Constructs a VUserAccount object from a specific objectPath in the form of
    /org/freedesktop/Accounts/UserUID where UID is user's uid.

    \param objectPath Accounts Service object path for the user account.
*/
VUserAccount::VUserAccount(const QString &objectPath)
    : d_ptr(new VUserAccountPrivate)
{
    d_ptr->user =
            new OrgFreedesktopAccountsUserInterface("org.freedesktop.Accounts",
                                                    objectPath,
                                                    QDBusConnection::systemBus(),
                                                    this);
}

/*!
    Destructor.
*/
VUserAccount::~VUserAccount()
{
    delete d_ptr;
}

/*!
    Returns the user identifier.
*/
uid_t VUserAccount::userId() const
{
    Q_D(const VUserAccount);
    return d->user->uid();
}

/*!
    Returns the account type.
*/
VUserAccount::AccountType VUserAccount::accountType() const
{
    Q_D(const VUserAccount);
    return (VUserAccount::AccountType)d->user->accountType();
}

/*!
    Sets the account type to \a type.

    \param type Account type.
*/
void VUserAccount::setAccountType(AccountType type)
{
    Q_D(VUserAccount);
    d->user->SetAccountType((int)type);
    emit accountTypeChanged();
}

/*!
    Returns whether this account is locked or not.
*/
bool VUserAccount::isLocked() const
{
    Q_D(const VUserAccount);
    return d->user->locked();
}

/*!
    Locks or unlocks the user account.
    Locking an account prevents the user from logging in.

    \param locked Whether to lock or unlock the user account.
*/
void VUserAccount::setLocked(bool locked)
{
    Q_D(VUserAccount);
    d->user->SetLocked(locked);
    emit lockedChanged();
}

/*!
    Returns whether the user account will automatically log in when the
    system starts up.
*/
bool VUserAccount::automaticLogin() const
{
    Q_D(const VUserAccount);
    return d->user->automaticLogin();
}

/*!
    Sets whether the user account will automatically log in when
    the system starts up.

    \param automaticLogin Whether automatic login is enabled for the user.
*/
void VUserAccount::setAutomaticLogin(bool automaticLogin)
{
    Q_D(VUserAccount);
    d->user->SetAutomaticLogin(automaticLogin);
    emit automaticLoginChanged();
}

/*!
    Returns how often the user has logged in.
*/
qlonglong VUserAccount::loginFrequency() const
{
    Q_D(const VUserAccount);
    return d->user->loginFrequency();
}

/*!
    Returns the last login time.
*/
qlonglong VUserAccount::loginTime() const
{
    Q_D(const VUserAccount);
    return d->user->loginTime();
}

/*!
    Returns the password mode for the user account.
*/
VUserAccount::PasswordMode VUserAccount::passwordMode() const
{
    Q_D(const VUserAccount);
    return (VUserAccount::PasswordMode)d->user->passwordMode();
}

/*!
    Sets the password mode for the user account.

    \param mode Password mode.
*/
void VUserAccount::setPasswordMode(VUserAccount::PasswordMode mode)
{
    Q_D(VUserAccount);
    d->user->SetPasswordMode((int)mode);
    emit passwordModeChanged();
}

/*!
    Returns the password hint for the user.
*/
QString VUserAccount::passwordHint() const
{
    Q_D(const VUserAccount);
    return d->user->passwordHint();
}

/*!
    Returns whether the user is a local account or not.
*/
bool VUserAccount::isLocalAccount() const
{
    Q_D(const VUserAccount);
    return d->user->localAccount();
}

/*!
    Returns whether the user is a system account or not.
*/
bool VUserAccount::isSystemAccount() const
{
    Q_D(const VUserAccount);
    return d->user->systemAccount();
}

/*!
    Returns the user name.
*/
QString VUserAccount::userName() const
{
    Q_D(const VUserAccount);
    return d->user->userName();
}

/*!
    Sets the user name to \a userName.

    \param userName The new user name.
*/
void VUserAccount::setUserName(const QString &userName)
{
    Q_D(VUserAccount);
    d->user->SetUserName(userName);
    emit userNameChanged();
    emit displayNameChanged();
}

/*!
    Returns user's real name.
*/
QString VUserAccount::realName() const
{
    Q_D(const VUserAccount);
    return d->user->realName();
}

/*!
    Sets the user's real name to \a realName.

    \param realName Real name.
*/
void VUserAccount::setRealName(const QString &realName)
{
    Q_D(VUserAccount);
    d->user->SetRealName(realName);
    emit realNameChanged();
    emit displayNameChanged();
}

/*!
    Returns user's real name if not empty, otherwise the user name.
*/
QString VUserAccount::displayName() const
{
    if (realName().isEmpty())
        return userName();
    return realName();
}

/*!
    Returns the home directory absolute path.
*/
QString VUserAccount::homeDirectory() const
{
    Q_D(const VUserAccount);
    return d->user->homeDirectory();
}

/*!
    Sets the home directory to \a homeDirectory.

    \param homeDirectory Home directory.
*/
void VUserAccount::setHomeDirectory(const QString &homeDirectory)
{
    Q_D(VUserAccount);
    d->user->SetHomeDirectory(homeDirectory);
    emit homeDirectoryChanged();
}

/*!
    Returns the login shell absolute path.
*/
QString VUserAccount::shell() const
{
    Q_D(const VUserAccount);
    return d->user->shell();
}

/*!
    Sets the login shell to \a shell.

    \param shell Login shell absolute path.
*/
void VUserAccount::setShell(const QString &shell)
{
    Q_D(VUserAccount);
    d->user->SetShell(shell);
    emit shellChanged();
}

/*!
    Returns user's picture absoulte path.
*/
QString VUserAccount::iconFileName() const
{
    Q_D(const VUserAccount);
    return d->user->iconFile();
}

/*!
    Sets users' picture absolute path to \a fileName.

    \param fileName Picture absolute path.
*/
void VUserAccount::setIconFileName(const QString &fileName)
{
    Q_D(VUserAccount);
    d->user->SetIconFile(fileName);
    emit iconFileNameChanged();
}

/*!
    Returns user's email address.
*/
QString VUserAccount::email() const
{
    Q_D(const VUserAccount);
    return d->user->email();
}

/*!
    Sets user's email address to \a email.

    \param email Email address.
*/
void VUserAccount::setEmail(const QString &email)
{
    Q_D(VUserAccount);
    d->user->SetEmail(email);
    emit emailChanged();
}

/*!
    Returns user language.
*/
QString VUserAccount::language() const
{
    Q_D(const VUserAccount);
    return d->user->language();
}

/*!
    Sets user language to \a language.

    \param language Language.
*/
void VUserAccount::setLanguage(const QString &language)
{
    Q_D(VUserAccount);
    d->user->SetLanguage(language);
    emit languageChanged();
}

/*!
    Returns user location.
*/
QString VUserAccount::location() const
{
    Q_D(const VUserAccount);
    return d->user->location();
}

/*!
    Sets user \a location.

    \param location Location.
*/
void VUserAccount::setLocation(const QString &location)
{
    Q_D(VUserAccount);
    d->user->SetLocation(location);
    emit locationChanged();
}

/*!
    Returns the X11 session for the user account.
*/
QString VUserAccount::xsession() const
{
    Q_D(const VUserAccount);
    return d->user->xSession();
}

/*!
    Sets the X11 session for the user account.

    \param session X11 session name.
*/
void VUserAccount::setXSession(const QString &session)
{
    Q_D(VUserAccount);
    d->user->SetXSession(session);
    emit xsessionChanged();
}

#include "moc_vuseraccount.cpp"
