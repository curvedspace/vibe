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
