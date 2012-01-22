/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2000-2005 David Faure
 * Copyright (c) 2003 Leo Savernik
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *	David Faure <faure@kde.org>
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *	Leo Savernik <l.savernik@aon.at>
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

#ifndef VARCHIVEHANDLER_H
#define VARCHIVEHANDLER_H

#include <VibeCore/VGlobal>

class VArchive;
class VArchiveDirectory;
class VArchiveHandlerPrivate;

/*!
    \addtogroup core Core Kit
    @{

    \class VArchiveHandler varchivehandler.h <VArchiveHandler>

    \brief Base class for archive handlers.

    \short generic class for archive handlers.

    \author David Faure <faure@kde.org>
    \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
*/
class VIBE_EXPORT VArchiveHandler
{
public:
    /*!
       Base constructor.
    */
    VArchiveHandler(const QString &mimeType);

    /*!
        Returns the pointer to the VArchive object that loaded this plugin.
    */
    VArchive *archive() const;

    /*!
        Saves a pointer to the VArchive object that loaded this plugin.
        The pointer will later be used to create the root directory.

        \sa rootDir()
    */
    void setArchive(VArchive *archive);

    /*!
        Opens the archive for reading or writing.
        @param mode may be QIODevice::ReadOnly or QIODevice::WriteOnly

        \sa close
    */
    bool open(QIODevice::OpenMode mode);

    /*!
        Closes the archive.
        Inherited classes might want to reimplement closeArchive instead,
       because this method is only called by VArchive.

       @return true if close succeeded without problems

       \sa open
    */
    bool close();

    /*!
        Checks whether the archive is open.
        @return true if the archive is opened
    */
    bool isOpen() const;

    /*!
        Returns the mode in which the archive was opened.
        @return the mode in which the archive was opened (QIODevice::ReadOnly or QIODevice::WriteOnly)

        \see open()
    */
    QIODevice::OpenMode mode() const;

    /*!
        The underlying device.
        @return the underlying device.
    */
    QIODevice *device() const;

    /*!
        Sets the underlying device.
        Note that this can be a file, but also a data buffer, a compression filter, etc.
        For a file in writing mode it is better to use the other constructor
        though, to benefit from the use of VSaveFile when saving.
        @param dev the I/O device where the archive reads its data
    */
    void setDevice(QIODevice *dev);

    /*!
        Retrieves or create the root directory.
        The default implementation assumes that openArchive() did the parsing,
        so it creates a dummy rootdir if none was set (write mode, or no '/' in the archive).
        Reimplement this to provide parsing/listing on demand.
        @return the root directory
    */
    virtual VArchiveDirectory *rootDir();

    /*!
        Derived classes call setRootDir from openArchive,
        to set the root directory after parsing an existing archive.
    */
    void setRootDir(VArchiveDirectory *rootDir);

    /*!
        Returns the name of the archive file or an empty string if not set.
        @return the name of the file, or QString() if unknown
    */
    QString fileName() const;

    /*!
        Sets the name of the archive file.
        @param fileName is a local path (e.g. "/common/var/tmp/myfile.ext"),
        from which the archive will be read from, or into which the archive
        will be written, depending on the mode given to open().
    */
    void setFileName(const QString &fileName);

    /*!
        Returns whether the device is owned.
    */
    bool isDeviceOwned() const;

    /*!
        VArchive calls this to write data into the current file, after calling prepareWriting.
    */
    bool writeData(const char *data, qint64 size);

    /*!
        Aborts writing to the open device.
    */
    void abortWriting();

    /*!
        Opens an archive for reading or writing.
        Called by open.
        @param mode may be QIODevice::ReadOnly or QIODevice::WriteOnly
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

        @param name the name of the directory
        @param user the user that owns the directory
        @param group the group that owns the directory
        @param perm permissions of the directory. Use 040755 if you don't have any other information.
        @param atime time the file was last accessed
        @param mtime modification time of the file
        @param ctime time of last status change

       \sa writeDir
    */
    virtual bool doWriteDir(const QString &name, const QString &user, const QString &group,
                            mode_t perm, time_t atime, time_t mtime, time_t ctime) = 0;

    /*!
        Writes a symbolic link to the archive.
        This virtual method must be implemented by subclasses.

        @param name name of symbolic link
        @param target target of symbolic link
        @param user the user that owns the directory
        @param group the group that owns the directory
        @param perm permissions of the directory
        @param atime time the file was last accessed
        @param mtime modification time of the file
        @param ctime time of last status change

        \sa writeSymLink
    */
    virtual bool doWriteSymLink(const QString &name, const QString &target,
                                const QString &user, const QString &group,
                                mode_t perm, time_t atime, time_t mtime, time_t ctime) = 0;

    /*!
        This virtual method must be implemented by subclasses.

        Depending on the archive type not all metadata might be used.

        @param name the name of the file
        @param user the user that owns the file
        @param group the group that owns the file
        @param size the size of the file
        @param perm permissions of the file. Use 0100644 if you don't have any more specific permissions to set.
        @param atime time the file was last accessed
        @param mtime modification time of the file
        @param ctime time of last status change

        \sa prepareWriting
    */
    virtual bool doPrepareWriting(const QString &name, const QString &user,
                                  const QString &group, qint64 size, mode_t perm,
                                  time_t atime, time_t mtime, time_t ctime) = 0;

    /*!
        Called after writing the data.
        This virtual method must be implemented by subclasses.

        @param size the size of the file

        \sa finishWriting()
    */
    virtual bool doFinishWriting(qint64 size) = 0;

    /*!
        Ensures that @p path exists, create otherwise.
        This handles e.g. tar files missing directory entries, like mico-2.3.0.tar.gz :)
        @param path the path of the directory
        @return the directory with the given @p path
    */
    VArchiveDirectory *findOrCreate(const QString &path);

    /*!
        Can be reimplemented in order to change the creation of the device
        (when using the fileName constructor). By default this method uses
        VSaveFile when saving, and a simple QFile on reading.
        This method is called by open().
    */
    virtual bool createDevice(QIODevice::OpenMode mode);

private:
    Q_DECLARE_PRIVATE(VArchiveHandler)

    VArchiveHandlerPrivate *const d_ptr;
};

/*!
    @}
*/

#endif // VARCHIVEHANDLER_H
