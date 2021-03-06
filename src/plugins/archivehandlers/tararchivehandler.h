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

#ifndef TARARCHIVE_H
#define TARARCHIVE_H

#include <VArchiveHandler>

class TarArchiveHandlerPrivate;

/*!
   A class for reading / writing (optionally compressed) tar archives.

   TarArchiveHandler allows you to read and write tar archives, including
   those that are compressed using gzip, bzip2 or xz.

   \author Torben Weis <weis@kde.org>
   \author David Faure <faure@kde.org>
   \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
*/
class VIBECORE_EXPORT TarArchiveHandler : public VArchiveHandler
{
public:
    /*!
        Creates an instance that operates on the given filename
        using the compression filter associated to given mimetype.

        \param mimetype "application/x-gzip", "application/x-bzip" or
        "application/x-xz"
        Do not use application/x-compressed-tar or similar - you only need to
        specify the compression layer !  If the mimetype is omitted, it
        will be determined from the filename.
    */
    explicit TarArchiveHandler(const QString &mimeType);

    /*!
        If the tar ball is still opened, then it will be
        closed automatically by the destructor.
    */
    virtual ~TarArchiveHandler();

    /*!
        Special function for setting the "original file name" in the gzip header,
        when writing a tar.gz file. It appears when using in the "file" command,
        for instance. Should only be called if the underlying device is a VCompressionFilter!
        \param fileName the original file name
    */
    void setOrigFileName(const QByteArray &fileName);

protected:
    // Reimplemented from VArchive
    virtual bool doWriteSymLink(const QString &name, const QString &target,
                                const QString &user, const QString &group,
                                mode_t perm, time_t atime, time_t mtime, time_t ctime);
    // Reimplemented from VArchive
    virtual bool doWriteDir(const QString &name, const QString &user, const QString &group,
                            mode_t perm, time_t atime, time_t mtime, time_t ctime);
    // Reimplemented from VArchive
    virtual bool doPrepareWriting(const QString &name, const QString &user,
                                  const QString &group, qint64 size, mode_t perm,
                                  time_t atime, time_t mtime, time_t ctime);
    // Reimplemented from VArchive
    virtual bool doFinishWriting(qint64 size);

    /*!
        Opens the archive for reading.
        Parses the directory listing of the archive
        and creates the VArchiveDirectory/VArchiveFile entries.
        \param mode the mode of the file
    */
    virtual bool openArchive(QIODevice::OpenMode mode);

    /*!
        Closes the archive.
    */
    virtual bool closeArchive();

    virtual bool createDevice(QIODevice::OpenMode mode);

private:
    TarArchiveHandlerPrivate *const d;
};

#endif // TARARCHIVE_H
