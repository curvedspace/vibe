/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 * Copyright (c) 2007 Henrique Pinto
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Henrique Pinto <henrique.pinto@kdemail.net>
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

#include <sys/stat.h>
#include <fcntl.h>

#include <QCoreApplication>
#include <QLocale>

#include "vfilestring.h"

namespace VFileString
{
    QString forSize(qreal size)
    {
        QLocale locale;

        qreal kib = size / 1024.0;
        if (kib < 1.0)
            return QCoreApplication::translate("VFileString",
                                               "%1 bytes", "Bytes format").arg(locale.toString(size));

        qreal mib = kib / 1024.0;
        if (mib < 1.0)
            return QCoreApplication::translate("VFileString",
                                               "%1 KiB", "File size format").arg(locale.toString(kib, 'f', 3));

        qreal gib = mib / 1024.0;
        if (gib < 1.0)
            return QCoreApplication::translate("VFileString",
                                               "%1 MiB", "File size format").arg(locale.toString(mib, 'f', 3));

        qreal tib = gib / 1024.0;
        if (tib < 1.0)
            return QCoreApplication::translate("VFileString",
                                               "%1 GiB", "File size format").arg(locale.toString(gib, 'f', 3));

        qreal pib = tib / 1024.0;
        if (pib < 1.0)
            return QCoreApplication::translate("VFileString",
                                               "%1 TiB", "File size format").arg(locale.toString(tib, 'f', 3));

        return QCoreApplication::translate("VFileString",
                                           "%1 PiB", "File size format").arg(locale.toString(pib, 'f', 3));
    }

    /*
     * From ark/plugins/karchiveplugin/karchiveplugin.cpp
     *
     * Copyright (C) 2007 Henrique Pinto <henrique.pinto@kdemail.net>
     */
    QString forPermissions(mode_t perm)
    {
        static char buffer[12];

        char uxbit, gxbit, oxbit;

        if ((perm & (S_IXUSR | S_ISUID)) == (S_IXUSR | S_ISUID))
            uxbit = 's';
        else if ((perm & (S_IXUSR | S_ISUID)) == S_ISUID)
            uxbit = 'S';
        else if ((perm & (S_IXUSR | S_ISUID)) == S_IXUSR)
            uxbit = 'x';
        else
            uxbit = '-';

        if ((perm & (S_IXGRP | S_ISGID)) == (S_IXGRP | S_ISGID))
            gxbit = 's';
        else if ((perm & (S_IXGRP | S_ISGID)) == S_ISGID)
            gxbit = 'S';
        else if ((perm & (S_IXGRP | S_ISGID)) == S_IXGRP)
            gxbit = 'x';
        else
            gxbit = '-';

        if ((perm & (S_IXOTH | S_ISVTX)) == (S_IXOTH | S_ISVTX))
            oxbit = 't';
        else if ((perm & (S_IXOTH | S_ISVTX)) == S_ISVTX)
            oxbit = 'T';
        else if ((perm & (S_IXOTH | S_ISVTX)) == S_IXOTH)
            oxbit = 'x';
        else
            oxbit = '-';

        // Include the type in the first character, people are more used to seeing it,
        // even though it's not really part of the permissions per se
        if (S_ISDIR(perm))
            buffer[0] = 'd';
        else if (S_ISLNK(perm))
            buffer[0] = 'l';
        else
            buffer[0] = '-';

        buffer[1] = (((perm & S_IRUSR) == S_IRUSR) ? 'r' : '-');
        buffer[2] = (((perm & S_IWUSR) == S_IWUSR) ? 'w' : '-');
        buffer[3] = uxbit;
        buffer[4] = (((perm & S_IRGRP) == S_IRGRP) ? 'r' : '-');
        buffer[5] = (((perm & S_IWGRP) == S_IWGRP) ? 'w' : '-');
        buffer[6] = gxbit;
        buffer[7] = (((perm & S_IROTH) == S_IROTH) ? 'r' : '-');
        buffer[8] = (((perm & S_IWOTH) == S_IWOTH) ? 'w' : '-');
        buffer[9] = oxbit;
        buffer[10] = 0;

        return QString::fromLatin1(buffer);
    }

    QString forPermissions(const QFileInfo &info)
    {
        mode_t p = 0;

        if (info.isDir())
            p |= S_IFDIR;
        if (info.isSymLink())
            p |= S_IFLNK;

        if (info.permission(QFile::ReadUser))
            p |= S_IRWXU & S_IRUSR;
        if (info.permission(QFile::WriteUser))
            p |= S_IRWXU & S_IWUSR;
        if (info.permission(QFile::ExeUser))
            p |= S_IRWXU & S_IXUSR;

        if (info.permission(QFile::ReadGroup))
            p |= S_IRWXG & S_IRGRP;
        if (info.permission(QFile::WriteGroup))
            p |= S_IRWXG & S_IWGRP;
        if (info.permission(QFile::ExeGroup))
            p |= S_IRWXG & S_IXGRP;

        if (info.permission(QFile::ReadOther))
            p |= S_IRWXO & S_IROTH;
        if (info.permission(QFile::WriteOther))
            p |= S_IRWXO & S_IWOTH;
        if (info.permission(QFile::ExeOther))
            p |= S_IRWXO & S_IXOTH;

        return forPermissions(p);
    }
}
