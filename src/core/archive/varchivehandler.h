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

#ifndef VARCHIVEHANDLER_H
#define VARCHIVEHANDLER_H

#include <QIODevice>

#include <VibeCore/VibeCoreExport>

class VArchive;
class VArchiveDirectory;
class VArchiveHandlerPrivate;

class VIBECORE_EXPORT VArchiveHandler
{
public:
    VArchiveHandler(const QString &mimeType);

    VArchive *archive() const;
    void setArchive(VArchive *archive);

    bool open(QIODevice::OpenMode mode);
    bool close();

    bool isOpen() const;

    QIODevice::OpenMode mode() const;

    QIODevice *device() const;
    void setDevice(QIODevice *dev);

    virtual VArchiveDirectory *rootDir();
    void setRootDir(VArchiveDirectory *rootDir);

    QString fileName() const;
    void setFileName(const QString &fileName);

    bool isDeviceOwned() const;

    bool writeData(const char *data, qint64 size);

    void abortWriting();

    /*!
        Opens an archive for reading or writing.
        Called by open.
        \param mode may be QIODevice::ReadOnly or QIODevice::WriteOnly
    */
    virtual bool openArchive(QIODevice::OpenMode mode) = 0;

    /*!
        Closes the archive.
        Called by close.
    */
    virtual bool closeArchive() = 0;

    /*!
        Write a directory to the archive.
        This virtual method must be implemented by subclasses.

        Depending on the archive type not all metadata might be used.

        \param name the name of the directory
        \param user the user that owns the directory
        \param group the group that owns the directory
        \param perm permissions of the directory. Use 040755 if you don't have any other information.
        \param atime time the file was last accessed
        \param mtime modification time of the file
        \param ctime time of last status change

       \sa writeDir
    */
    virtual bool doWriteDir(const QString &name, const QString &user, const QString &group,
                            mode_t perm, time_t atime, time_t mtime, time_t ctime) = 0;

    /*!
        Writes a symbolic link to the archive.
        This virtual method must be implemented by subclasses.

        \param name name of symbolic link
        \param target target of symbolic link
        \param user the user that owns the directory
        \param group the group that owns the directory
        \param perm permissions of the directory
        \param atime time the file was last accessed
        \param mtime modification time of the file
        \param ctime time of last status change

        \sa writeSymLink
    */
    virtual bool doWriteSymLink(const QString &name, const QString &target,
                                const QString &user, const QString &group,
                                mode_t perm, time_t atime, time_t mtime, time_t ctime) = 0;

    /*!
        This virtual method must be implemented by subclasses.

        Depending on the archive type not all metadata might be used.

        \param name the name of the file
        \param user the user that owns the file
        \param group the group that owns the file
        \param size the size of the file
        \param perm permissions of the file. Use 0100644 if you don't have any more specific permissions to set.
        \param atime time the file was last accessed
        \param mtime modification time of the file
        \param ctime time of last status change

        \sa prepareWriting
    */
    virtual bool doPrepareWriting(const QString &name, const QString &user,
                                  const QString &group, qint64 size, mode_t perm,
                                  time_t atime, time_t mtime, time_t ctime) = 0;

    /*!
        Called after writing the data.
        This virtual method must be implemented by subclasses.

        \param size the size of the file

        \sa finishWriting()
    */
    virtual bool doFinishWriting(qint64 size) = 0;

    VArchiveDirectory *findOrCreate(const QString &path);

    virtual bool createDevice(QIODevice::OpenMode mode);

private:
    Q_DECLARE_PRIVATE(VArchiveHandler)

    VArchiveHandlerPrivate *const d_ptr;
};

#endif // VARCHIVEHANDLER_H
