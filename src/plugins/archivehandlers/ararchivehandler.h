/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2002 Laurence Anderson
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Laurence Anderson <l.d.anderson@warwick.ac.uk>
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

#ifndef ARARCHIVEHANDLER_H
#define ARARCHIVEHANDLER_H

#include <VArchiveHandler>

/*!
    ArArchiveHandler is a class for reading archives in ar format. Writing
    is not supported.

    \short A class for reading ar archives.

    \author Laurence Anderson <l.d.anderson@warwick.ac.uk>
    \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
*/
class VIBECORE_EXPORT ArArchiveHandler : public VArchiveHandler
{
public:
    /*!
        Creates an instance that operates on the given filename.

        \param filename is a local path (e.g. "/home/holger/myfile.ar")
    */
    explicit ArArchiveHandler(const QString &filename);

    /*!
        If the ar file is still opened, then it will be
        closed automatically by the destructor.
    */
    virtual ~ArArchiveHandler();

protected:
    /*!
        Writing not supported by this class, will always fail.
        \return always false
    */
    virtual bool doPrepareWriting(const QString &name, const QString &user, const QString &group, qint64 size,
                                  mode_t perm, time_t atime, time_t mtime, time_t ctime);

    /*!
        Writing not supported by this class, will always fail.
        \return always false
    */
    virtual bool doFinishWriting(qint64 size);

    /*!
        Writing not supported by this class, will always fail.
        \return always false
    */
    virtual bool doWriteDir(const QString &name, const QString &user, const QString &group,
                            mode_t perm, time_t atime, time_t mtime, time_t ctime);

    virtual bool doWriteSymLink(const QString &name, const QString &target,
                                const QString &user, const QString &group, mode_t perm, time_t atime, time_t mtime, time_t ctime);

    /*!
        Opens the archive for reading.
        Parses the directory listing of the archive
        and creates the ArArchiveHandlerchiveDirectory/ArArchiveHandlerchiveFile entries.
    */
    virtual bool openArchive(QIODevice::OpenMode mode);

    /*!
        Closes the archive.
    */
    virtual bool closeArchive();
};

#endif // ARARCHIVEHANDLER_H
