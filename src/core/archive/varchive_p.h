/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2000-2005 David Faure
 * Copyright (c) 2003 Leo Savernik
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    David Faure <faure@kde.org>
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Leo Savernik <l.savernik@aon.at>
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

#ifndef VARCHIVE_P_H
#define VARCHIVE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Vibe API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QMimeDatabase>

class VArchivePrivate
{
public:
    VArchivePrivate();
    ~VArchivePrivate();

    VArchiveHandler *handler;
    QMimeDatabase mimeDatabase;
};

class VArchiveEntryPrivate
{
public:
    VArchiveEntryPrivate(VArchive *_archive, const QString &_name, int _access,
                         int _date, const QString &_user, const QString &_group,
                         const QString &_symlink);

    QString name;
    int date;
    mode_t access;
    QString user;
    QString group;
    QString symlink;
    VArchive *archive;
};

class VArchiveFilePrivate
{
public:
    VArchiveFilePrivate(qint64 _pos, qint64 _size);

    qint64 pos;
    qint64 size;
};

class VArchiveDirectoryPrivate
{
public:
    ~VArchiveDirectoryPrivate();

    QHash<QString, VArchiveEntry *> entries;
};

#endif // VARCHIVE_P_H
