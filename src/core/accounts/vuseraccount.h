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
class VAccountsManagerPrivate;
class VUserAccountPrivate;

class VIBECORE_EXPORT VUserAccount : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(VUserAccount)
    Q_PROPERTY(int uid READ userId CONSTANT)
    Q_PROPERTY(AccountType accountType READ accountType WRITE setAccountType NOTIFY accountTypeChanged)
    Q_PROPERTY(bool locked READ isLocked WRITE setLocked NOTIFY lockedChanged)
    Q_PROPERTY(bool automaticLogin READ automaticLogin WRITE setAutomaticLogin NOTIFY automaticLoginChanged)
    Q_PROPERTY(qlonglong loginFrequency READ loginFrequency)
    Q_PROPERTY(qlonglong loginTime READ loginTime)
    Q_PROPERTY(PasswordMode passwordMode READ passwordMode WRITE setPasswordMode NOTIFY passwordModeChanged)
    Q_PROPERTY(QString passwordHint READ passwordHint)
    Q_PROPERTY(bool localAccount READ isLocalAccount)
    Q_PROPERTY(bool systemAccount READ isSystemAccount)
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString realName READ realName WRITE setRealName NOTIFY realNameChanged)
    Q_PROPERTY(QString displayName READ displayName NOTIFY displayNameChanged)
    Q_PROPERTY(QString homeDirectory READ homeDirectory WRITE setHomeDirectory NOTIFY homeDirectoryChanged)
    Q_PROPERTY(QString shell READ shell WRITE setShell NOTIFY shellChanged)
    Q_PROPERTY(QString iconFileName READ iconFileName WRITE setIconFileName NOTIFY iconFileNameChanged)
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged)
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY locationChanged)
    Q_PROPERTY(QString xsession READ xsession WRITE setXSession NOTIFY xsessionChanged)
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

Q_SIGNALS:
    void accountTypeChanged();
    void lockedChanged();
    void automaticLoginChanged();
    void passwordModeChanged();
    void userNameChanged();
    void realNameChanged();
    void displayNameChanged();
    void homeDirectoryChanged();
    void shellChanged();
    void iconFileNameChanged();
    void emailChanged();
    void languageChanged();
    void locationChanged();
    void xsessionChanged();

private:
    friend class VAccountsManager;
    friend class VAccountsManagerPrivate;

    VUserAccountPrivate *const d_ptr;

    VUserAccount(const QString &objectPath);
};

typedef QList<VUserAccount *> VUserAccountList;

#endif // VUSERACCOUNT_H
