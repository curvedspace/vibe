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
