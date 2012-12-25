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

#include <QtCore/QObject>
#include <QtCore/QString>

#include <VibeCore/VibeCoreExport>

class VAccountsManager;
class VUserAccountPrivate;

class VIBECORE_EXPORT VUserAccount : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(VUserAccount)
    Q_PROPERTY(int uid READ userId CONSTANT)
    Q_PROPERTY(AccountType accountType READ accountType WRITE setAccountType)
    Q_PROPERTY(bool locked READ isLocked WRITE setLocked)
    Q_PROPERTY(bool automaticLogin READ automaticLogin WRITE setAutomaticLogin)
    Q_PROPERTY(qlonglong loginFrequency READ loginFrequency)
    Q_PROPERTY(qlonglong loginTime READ loginTime)
    Q_PROPERTY(PasswordMode passwordMode READ passwordMode WRITE setPasswordMode)
    Q_PROPERTY(QString passwordHint READ passwordHint)
    Q_PROPERTY(bool localAccount READ isLocalAccount)
    Q_PROPERTY(bool systemAccount READ isSystemAccount)
    Q_PROPERTY(QString userName READ userName WRITE setUserName)
    Q_PROPERTY(QString realName READ realName WRITE setRealName)
    Q_PROPERTY(QString displayName READ displayName)
    Q_PROPERTY(QString homeDirectory READ homeDirectory WRITE setHomeDirectory)
    Q_PROPERTY(QString shell READ shell WRITE setShell)
    Q_PROPERTY(QString iconFileName READ iconFileName WRITE setIconFileName)
    Q_PROPERTY(QString email READ email WRITE setEmail)
    Q_PROPERTY(QString language READ language WRITE setLanguage)
    Q_PROPERTY(QString location READ location WRITE setLocation)
    Q_PROPERTY(QString xsession READ xsession WRITE setXSession)
    Q_ENUMS(AccountType PasswordMode)
public:
    enum AccountType {
        StandardAccountType = 0,
        AdministratorAccountType
    };

    enum PasswordMode {
        RegularPasswordMode = 0,
        SetAtLoginPasswordMode,
        NonePasswordMode
    };

    explicit VUserAccount(uid_t uid);
    ~VUserAccount();

    uid_t userId() const;

    AccountType accountType() const;
    void setAccountType(AccountType type);

    bool isLocked() const;
    void setLocked(bool locked);

    bool automaticLogin() const;
    void setAutomaticLogin(bool automaticLogin);

    qlonglong loginFrequency() const;

    qlonglong loginTime() const;

    PasswordMode passwordMode() const;
    void setPasswordMode(PasswordMode mode);

    QString passwordHint() const;

    bool isLocalAccount() const;

    bool isSystemAccount() const;

    QString userName() const;
    void setUserName(const QString &userName);

    QString realName() const;
    void setRealName(const QString &realName);

    QString displayName() const;

    QString homeDirectory() const;
    void setHomeDirectory(const QString &homeDirectory);

    QString shell() const;
    void setShell(const QString &shell);

    QString iconFileName() const;
    void setIconFileName(const QString &fileName);

    QString email() const;
    void setEmail(const QString &email);

    QString language() const;
    void setLanguage(const QString &language);

    QString location() const;
    void setLocation(const QString &location);

    QString xsession() const;
    void setXSession(const QString &session);

private:
    friend class VAccountsManager;

    VUserAccountPrivate *const d_ptr;

    VUserAccount(const QString &objectPath);
};

typedef QList<VUserAccount *> VUserAccountList;

#endif // VUSERACCOUNT_H
