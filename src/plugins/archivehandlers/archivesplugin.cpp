/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <QtPlugin>

#include "archivesplugin.h"
#include "tararchivehandler.h"
#include "ararchivehandler.h"

QStringList ArchivesPlugin::mimeTypes() const
{
    QStringList types;
    types << tarMimeTypes();
    types << arMimeTypes();

    return types;
}

VArchiveHandler *ArchivesPlugin::create(const QString &mimeType)
{
    if (tarMimeTypes().contains(mimeType))
        return new TarArchiveHandler(mimeType);
    if (arMimeTypes().contains(mimeType))
        return new ArArchiveHandler(mimeType);
    return 0;
}

QStringList ArchivesPlugin::tarMimeTypes() const
{
    QStringList types;
    types << "application/x-tar"
          << "application/x-compressed-tar"
          << "application/x-bzip-compressed-tar"
          << "application/x-lzma-compressed-tar"
          << "application/x-xz-compressed-tar";

    return types;
}

QStringList ArchivesPlugin::arMimeTypes() const
{
    QStringList types;
    types << "application/x-archive";

    return types;
}

Q_EXPORT_PLUGIN2(VibeArchivePlugin, ArchivesPlugin)

#include "archivesplugin.moc"
