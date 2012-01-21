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

#ifndef VARCHIVE_H
#define VARCHIVE_H

#include <sys/stat.h>
#include <sys/types.h>

#include <QDate>
#include <QString>
#include <QStringList>
#include <QHash>

#include <VibeCore/VGlobal>

/** \addtogroup core Core Kit
 *  @{
 */

class VArchiveDirectory;
class VArchiveFile;

class VArchivePrivate;

/**
 * \class VArchive varchive.h <VArchive>
 *
 * \brief Basease class for reading and writing archives.
 *
 * \short generic class for reading/writing archives
 *
 * \author David Faure <faure@kde.org>
 * \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VIBE_EXPORT VArchive
{
    Q_DECLARE_PRIVATE(VArchive)
public:
    /**
     * Constructor.
     * @param fileName is a local path (e.g. "/common/var/tmp/myfile.ext"),
     * from which the archive will be read from, or into which the archive
     * will be written, depending on the mode given to open().
     * MIME type will be recognized and the appropriate plugin will
     * automatically be loaded.
     */
    VArchive(const QString &fileName);

    /**
     * Constructor.
     * @param dev the I/O device where the archive reads its data
     * Note that this can be a file, but also a data buffer, a compression filter, etc.
     * For a file in writing mode it is better to use the other constructor
     * though, to benefit from the use of KSaveFile when saving.
     * MIME type will be recognized and the appropriate plugin will
     * automatically be loaded.
     */
    VArchive(QIODevice *dev);

    /**
     * Destructor.
     */
    virtual ~VArchive();

    /**
     * Opens the archive for reading or writing.
     * @param mode may be QIODevice::ReadOnly or QIODevice::WriteOnly
     * @see close
     */
    bool open(QIODevice::OpenMode mode);

    /**
     * Closes the archive.
     *
     * @return true if close succeeded without problems
     * @see open
     */
    bool close();

    /**
     * Returns true if
     */
    bool isValid() const;

    /**
     * Checks whether the archive is open.
     * @return true if the archive is opened
     */
    bool isOpen() const;

    /**
     * Returns the mode in which the archive was opened.
     * @return the mode in which the archive was opened (QIODevice::ReadOnly or QIODevice::WriteOnly)
     * @see open()
     */
    QIODevice::OpenMode mode() const;

    /**
     * The underlying device.
     * @return the underlying device.
     */
    QIODevice *device() const;

    /**
     * The name of the archive file, as passed to the constructor that takes a
     * fileName, or an empty string if you used the QIODevice constructor.
     * @return the name of the file, or QString() if unknown
     */
    QString fileName() const;

    /**
     * If an archive is opened for reading, then the contents
     * of the archive can be accessed via this function.
     * @return the directory of the archive
     */
    const VArchiveDirectory *directory() const;

    /**
     * Writes a local file into the archive. The main difference with writeFile,
     * is that this method minimizes memory usage, by not loading the whole file
     * into memory in one go.
     *
     * If @p fileName is a symbolic link, it will be written as is, i. e.
     * it will not be resolved before.
     * @param fileName full path to an existing local file, to be added to the archive.
     * @param destName the resulting name (or relative path) of the file in the archive.
     */
    bool addLocalFile(const QString &fileName, const QString &destName);

    /**
     * Writes a local directory into the archive, including all its contents, recursively.
     * Calls addLocalFile for each file to be added.
     *
     * Since KDE 3.2 it will also add a @p path that is a symbolic link to a
     * directory. The symbolic link will be dereferenced and the content of the
     * directory it is pointing to added recursively. However, symbolic links
     * *under* @p path will be stored as is.
     * @param path full path to an existing local directory, to be added to the archive.
     * @param destName the resulting name (or relative path) of the file in the archive.
     */
    bool addLocalDirectory(const QString &path, const QString &destName);

    enum { UnknownTime = static_cast<time_t>(-1) };

    /**
     * If an archive is opened for writing then you can add new directories
     * using this function. VArchive won't write one directory twice.
     *
     * This method also allows some file metadata to be set.
     * However, depending on the archive type not all metadata might be regarded.
     *
     * @param name the name of the directory
     * @param user the user that owns the directory
     * @param group the group that owns the directory
     * @param perm permissions of the directory
     * @param atime time the file was last accessed
     * @param mtime modification time of the file
     * @param ctime time of last status change
     */
    bool writeDir(const QString &name, const QString &user, const QString &group,
                  mode_t perm = 040755, time_t atime = UnknownTime,
                  time_t mtime = UnknownTime, time_t ctime = UnknownTime);

    /**
     * Writes a symbolic link to the archive if supported.
     * The archive must be opened for writing.
     *
     * @param name name of symbolic link
     * @param target target of symbolic link
     * @param user the user that owns the directory
     * @param group the group that owns the directory
     * @param perm permissions of the directory
     * @param atime time the file was last accessed
     * @param mtime modification time of the file
     * @param ctime time of last status change
     */
    bool writeSymLink(const QString &name, const QString &target,
                      const QString &user, const QString &group,
                      mode_t perm = 0120755, time_t atime = UnknownTime,
                      time_t mtime = UnknownTime, time_t ctime = UnknownTime);

    /**
     * If an archive is opened for writing then you can add a new file
     * using this function. If the file name is for example "mydir/test1" then
     * the directory "mydir" is automatically appended first if that did not
     * happen yet.
     *
     * This method also allows some file metadata to be
     * set. However, depending on the archive type not all metadata might be
     * regarded.
     * @param name the name of the file
     * @param user the user that owns the file
     * @param group the group that owns the file
     * @param data the data to write (@p size bytes)
     * @param size the size of the file
     * @param perm permissions of the file
     * @param atime time the file was last accessed
     * @param mtime modification time of the file
     * @param ctime time of last status change
     */
    bool writeFile(const QString &name, const QString &user, const QString &group,
                   const char *data, qint64 size,
                   mode_t perm = 0100644, time_t atime = UnknownTime,
                   time_t mtime = UnknownTime, time_t ctime = UnknownTime);

    /**
     * Write data into the current file - to be called after calling prepareWriting
     */
    bool writeData(const char *data, qint64 size);

    /**
     * Here's another way of writing a file into an archive:
     * Call prepareWriting(), then call writeData()
     * as many times as wanted then call finishWriting( totalSize ).
     * For tar.gz files, you need to know the size before hand, it is needed in the header!
     * For zip files, size isn't used.
     *
     * This method also allows some file metadata to be
     * set. However, depending on the archive type not all metadata might be
     * regarded.
     * @param name the name of the file
     * @param user the user that owns the file
     * @param group the group that owns the file
     * @param size the size of the file
     * @param perm permissions of the file
     * @param atime time the file was last accessed
     * @param mtime modification time of the file
     * @param ctime time of last status change
     */
    bool prepareWriting(const QString &name, const QString &user,
                        const QString &group, qint64 size,
                        mode_t perm = 0100644, time_t atime = UnknownTime,
                        time_t mtime = UnknownTime, time_t ctime = UnknownTime);

    /**
     * Call finishWriting after writing the data.
     * @param size the size of the file
     * @see prepareWriting()
     */
    bool finishWriting(qint64 size);

private:
    VArchivePrivate *const d_ptr;
};

class VArchiveEntryPrivate;

/**
 * \class VArchiveEntry <varchive.h> <VArchive>
 *
 * \brief A base class for entries in an VArchive.
 *
 * \short Base class for the archive-file's directory structure.
 *
 * \see VArchiveFile
 * \see VArchiveDirectory
 *
 * \author David Faure <faure@kde.org>
 * \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VIBE_EXPORT VArchiveEntry
{
    Q_DECLARE_PRIVATE(VArchiveEntry)
public:
    /**
     * Creates a new entry.
     * @param archive the entries archive
     * @param name the name of the entry
     * @param access the permissions in unix format
     * @param date the date (in seconds since 1970)
     * @param user the user that owns the entry
     * @param group the group that owns the entry
     * @param symlink the symlink, or QString()
     */
    VArchiveEntry(VArchive *archive, const QString &name, int access, int date,
                  const QString &user, const QString &group,
                  const QString &symlink);

    virtual ~VArchiveEntry();

    /**
     * Creation date of the file.
     * @return the creation date
     */
    QDateTime datetime() const;

    /**
     * Creation date of the file.
     * @return the creation date in seconds since 1970
     */
    int date() const;

    /**
     * Name of the file without path.
     * @return the file name without path
     */
    QString name() const;
    /**
     * The permissions and mode flags as returned by the stat() function
     * in st_mode.
     * @return the permissions
     */
    mode_t permissions() const;
    /**
     * User who created the file.
     * @return the owner of the file
     */
    QString user() const;
    /**
     * Group of the user who created the file.
     * @return the group of the file
     */
    QString group() const;

    /**
     * Symlink if there is one.
     * @return the symlink, or QString()
     */
    QString symLinkTarget() const;

    /**
     * Checks whether the entry is a file.
     * @return true if this entry is a file
     */
    virtual bool isFile() const;

    /**
     * Checks whether the entry is a directory.
     * @return true if this entry is a directory
     */
    virtual bool isDirectory() const;

protected:
    VArchive *archive() const;

private:
    VArchiveEntryPrivate *const d_ptr;
};

class VArchiveFilePrivate;

/**
 * \class VArchiveFile <varchive.h> <VArchive>
 *
 * \brief Represents a file entry in a VArchive.
 *
 * \short A file in an archive.
 *
 * \see VArchive
 * \see VArchiveDirectory
 *
 * \author David Faure <faure@kde.org>
 * \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VIBE_EXPORT VArchiveFile : public VArchiveEntry
{
    Q_DECLARE_PRIVATE(VArchiveFile)
public:
    /**
     * Creates a new file entry. Do not call this, VArchive takes care of it.
     * @param archive the entries archive
     * @param name the name of the entry
     * @param access the permissions in unix format
     * @param date the date (in seconds since 1970)
     * @param user the user that owns the entry
     * @param group the group that owns the entry
     * @param symlink the symlink, or QString()
     * @param pos the position of the file in the directory
     * @param size the size of the file
     */
    VArchiveFile(VArchive *archive, const QString &name, int access, int date,
                 const QString &user, const QString &group, const QString &symlink,
                 qint64 pos, qint64 size);

    /**
     * Destructor. Do not call this, VArchive takes care of it.
     */
    virtual ~VArchiveFile();

    /**
     * Position of the data in the [uncompressed] archive.
     * @return the position of the file
     */
    qint64 position() const;
    /**
     * Size of the data.
     * @return the size of the file
     */
    qint64 size() const;
    /**
     * Set size of data, usually after writing the file.
     * @param s the new size of the file
     */
    void setSize(qint64 s);

    /**
     * Returns the data of the file.
     * Call data() with care (only once per file), this data isn't cached.
     * @return the content of this file.
     */
    virtual QByteArray data() const;

    /**
     * This method returns QIODevice (internal class: VLimitedIODevice)
     * on top of the underlying QIODevice. This is obviously for reading only.
     *
     * WARNING: Note that the ownership of the device is being transferred to the caller,
     * who will have to delete it.
     *
     * The returned device auto-opens (in readonly mode), no need to open it.
     * @return the QIODevice of the file
     */
    virtual QIODevice *createDevice() const;

    /**
     * Checks whether this entry is a file.
     * @return true, since this entry is a file
     */
    virtual bool isFile() const;

    /**
     * Extracts the file to the directory @p dest
     * @param dest the directory to extract to
     */
    void copyTo(const QString &dest) const;

private:
    VArchiveFilePrivate *const d_ptr;
};

class VArchiveDirectoryPrivate;

/**
 * \class VArchiveDirectory <varchive.h> <VArchive>
 * \brief Represents a directory entry in a VArchive.
 *
 * \short A directory in an archive.
 *
 * \see VArchive
 * \see VArchiveFile
 *
 * \author David Faure <faure@kde.org>
 * \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VIBE_EXPORT VArchiveDirectory : public VArchiveEntry
{
    Q_DECLARE_PRIVATE(VArchiveDirectory)
public:
    /**
     * Creates a new directory entry.
     * @param archive the entries archive
     * @param name the name of the entry
     * @param access the permissions in unix format
     * @param date the date (in seconds since 1970)
     * @param user the user that owns the entry
     * @param group the group that owns the entry
     * @param symlink the symlink, or QString()
     */
    VArchiveDirectory(VArchive *archive, const QString &name, int access, int date,
                      const QString &user, const QString &group,
                      const QString &symlink);

    virtual ~VArchiveDirectory();

    /**
     * Returns a list of sub-entries.
     * Note that the list is not sorted, it's even in random order (due to using a hashtable).
     * Use sort() on the result to sort the list by filename.
     *
     * @return the names of all entries in this directory (filenames, no path).
     */
    QStringList entries() const;

    /**
     * Returns the entry with the given name.
     * @param name may be "test1", "mydir/test3", "mydir/mysubdir/test3", etc.
     * @return a pointer to the entry in the directory.
     */
    const VArchiveEntry *entry(const QString &name) const;

    /**
     * @internal
     * Adds a new entry to the directory.
     */
    void addEntry(VArchiveEntry *);

    /**
     * Checks whether this entry is a directory.
     * @return true, since this entry is a directory
     */
    virtual bool isDirectory() const;

    /**
     * Extracts all entries in this archive directory to the directory
     * @p dest.
     * @param dest the directory to extract to
     * @param recursive if set to true, subdirectories are extracted as well
     */
    void copyTo(const QString &dest, bool recursive = true) const;

private:
    VArchiveDirectoryPrivate *const d_ptr;
};

/** @}*/

#endif // VARCHIVE_H
