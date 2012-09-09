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

#include <QStack>
#include <QDir>
#include <QDebug>
#include <QMimeType>
#include <QPluginLoader>

#include <VibeCore/VFileSupport>
#include <VibeCore/VStandardDirectories>

#include "varchive.h"
#include "varchivehandler.h"
#include "varchivehandlerplugin.h"
#include "varchive_p.h"
#include "vlimitediodevice_p.h"

#include <errno.h>

using namespace VStandardDirectories;

/*
 * VArchivePrivate
 */

VArchivePrivate::VArchivePrivate() :
    handler(0)
{
}

VArchivePrivate::~VArchivePrivate()
{
    delete handler;
}

/*!
   \class VArchive
   \brief The VArchive class provides a standard way for reading and writing archives.

   \ingroup archives

   \author David Faure <faure@kde.org>
   \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
*/

/*!
    Constructor.

    \param fileName Is a local path (e.g. "/common/var/tmp/myfile.ext"),
    from which the archive will be read from, or into which the archive
    will be written, depending on the mode given to open().
    MIME type will be recognized and the appropriate plugin will
    automatically be loaded.
*/
VArchive::VArchive(const QString &fileName) :
    d_ptr(new VArchivePrivate)
{
    Q_D(VArchive);

    Q_ASSERT(!fileName.isEmpty());

    // Try to find the MIME Type for the specified file, otherwise give up
    QMimeType mimeType = d->mimeDatabase.mimeTypeForFile(fileName);
    if (!mimeType.isValid())
        qFatal("Could not determine MIME Type for %s, cannot continue!",
               qPrintable(fileName));

    // Try to find the appropriate plugin, otherwise give up
    VArchiveHandler *handler = 0;
    QList<QDir> directories;
    directories << QDir(QString("%1/archivehandlers")
                        .arg(findDirectory(CommonPluginsDirectory)));
    directories << QDir(QString("%1/archivehandlers")
                        .arg(findDirectory(SystemPluginsDirectory)));
    foreach(QDir pluginsDir, directories) {
        foreach(QString fileName, pluginsDir.entryList(QDir::Files)) {
            QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
            VArchiveHandlerPlugin *plugin = qobject_cast<VArchiveHandlerPlugin *>(
                                                loader.instance());
            if (plugin) {
                handler = plugin->create(mimeType.name());
                if (handler)
                    break;
            }
        }
    }
    if (!handler)
        qFatal("No archive handler for %s, cannot continue!",
               qPrintable(mimeType.name()));
    d->handler = handler;
    d->handler->setArchive(this);
    d->handler->setFileName(fileName);
}

/*!
    Constructor.
    \param dev the I/O device where the archive reads its data
    Note that this can be a file, but also a data buffer, a compression filter, etc.
    For a file in writing mode it is better to use the other constructor
    though, to benefit from the use of KSaveFile when saving.
    MIME type will be recognized and the appropriate plugin will
    automatically be loaded.
*/
VArchive::VArchive(QIODevice *dev) :
    d_ptr(new VArchivePrivate)
{
    Q_D(VArchive);

    // Try to find the MIME Type for the specified file, otherwise give up
    QMimeType mimeType = d->mimeDatabase.mimeTypeForData(dev);
    if (!mimeType.isValid())
        qFatal("Could not determine MIME Type for the device, cannot continue!");

    // Try to find the appropriate plugin, otherwise give up
    VArchiveHandler *handler = 0;
    QList<QDir> directories;
    directories << QDir(QString("%1/archivehandlers")
                        .arg(findDirectory(CommonPluginsDirectory)));
    directories << QDir(QString("%1/archivehandlers")
                        .arg(findDirectory(SystemPluginsDirectory)));
    foreach(QDir pluginsDir, directories) {
        foreach(QString fileName, pluginsDir.entryList(QDir::Files)) {
            QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
            VArchiveHandlerPlugin *plugin = qobject_cast<VArchiveHandlerPlugin *>(
                                                loader.instance());
            if (plugin) {
                handler = plugin->create(mimeType.name());
                if (handler)
                    break;
            }
        }
    }
    if (!handler)
        qFatal("No archive handler for %s, cannot continue!",
               qPrintable(mimeType.name()));
    d->handler = handler;
    d->handler->setArchive(this);
    d->handler->setDevice(dev);
}

/*!
    Destructor.
*/
VArchive::~VArchive()
{
    if (isOpen())
        close();

    delete d_ptr;
}

/*!
    Opens the archive for reading or writing.
    \param mode may be QIODevice::ReadOnly or QIODevice::WriteOnly

    \sa close
*/
bool VArchive::open(QIODevice::OpenMode mode)
{
    Q_D(VArchive);
    return d->handler->open(mode);
}

/*!
    Closes the archive.

    \return true if close succeeded without problems

    \sa open
*/
bool VArchive::close()
{
    Q_D(VArchive);
    return d->handler->close();
}

/*!
    Checks whether the archive is open.
    \return true if the archive is opened
*/
bool VArchive::isOpen() const
{
    Q_D(const VArchive);
    return d->handler->isOpen();
}

/*!
    Returns the mode in which the archive was opened.
    \return the mode in which the archive was opened (QIODevice::ReadOnly or QIODevice::WriteOnly)

    \sa open()
*/
QIODevice::OpenMode VArchive::mode() const
{
    Q_D(const VArchive);
    return d->handler->mode();
}

/*!
    The underlying device.
    \return the underlying device.
*/
QIODevice *VArchive::device() const
{
    Q_D(const VArchive);
    return d->handler->device();
}

/*!
    The name of the archive file, as passed to the constructor that takes a
    fileName, or an empty string if you used the QIODevice constructor.
    \return the name of the file, or QString() if unknown
*/
QString VArchive::fileName() const
{
    Q_D(const VArchive);
    return d->handler->fileName();
}

/*!
    If an archive is opened for reading, then the contents
    of the archive can be accessed via this function.
    \return the directory of the archive
*/
const VArchiveDirectory *VArchive::directory() const
{
    Q_D(const VArchive);
    return d->handler->rootDir();
}

/*!
    Writes a local file into the archive. The main difference with writeFile,
    is that this method minimizes memory usage, by not loading the whole file
    into memory in one go.

   If @p fileName is a symbolic link, it will be written as is, i. e.
   it will not be resolved before.
   \param fileName full path to an existing local file, to be added to the archive.
   \param destName the resulting name (or relative path) of the file in the archive.
*/
bool VArchive::addLocalFile(const QString &fileName, const QString &destName)
{
    Q_D(VArchive);

    QFileInfo fileInfo(fileName);
    if (!fileInfo.isFile() && !fileInfo.isSymLink()) {
        qWarning() << fileName << "doesn't exist or is not a regular file.";
        return false;
    }

    Vibe_struct_stat fi;
    if (Vibe::lstat(fileName, &fi) == -1) {
        qWarning() << "stat'ing" << fileName
                   << "failed:" << strerror(errno);
        return false;
    }

    if (fileInfo.isSymLink()) {
        QString symLinkTarget;
        // Do NOT use fileInfo.readLink() for unix symlinks!
        // It returns the -full- path to the target, while we want the target string "as is".
#if defined(Q_OS_UNIX) && !defined(Q_OS_OS2EMX)
        const QByteArray encodedFileName = QFile::encodeName(fileName);
        QByteArray s;
#if defined(PATH_MAX)
        s.resize(PATH_MAX + 1);
#else
        int path_max = pathconf(encodedFileName.data(), _PC_PATH_MAX);
        if (path_max <= 0)
            path_max = 4096;
        s.resize(path_max);
#endif
        int len = readlink(encodedFileName.data(), s.data(), s.size() - 1);
        if (len >= 0) {
            s[len] = '\0';
            symLinkTarget = QFile::decodeName(s);
        }
#endif
        if (symLinkTarget.isEmpty()) // Mac or Windows
            symLinkTarget = fileInfo.symLinkTarget();
        return writeSymLink(destName, symLinkTarget, fileInfo.owner(),
                            fileInfo.group(), fi.st_mode, fi.st_atime, fi.st_mtime,
                            fi.st_ctime);
    }

    qint64 size = fileInfo.size();

    // The file must be opened before prepareWriting is called, otherwise
    // if the opening fails, no content will follow the already written
    // header and the tar file is effectively f*cked up
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file " << fileName;
        return false;
    }

    if (!prepareWriting(destName, fileInfo.owner(), fileInfo.group(), size,
                        fi.st_mode, fi.st_atime, fi.st_mtime, fi.st_ctime)) {
        qWarning() << "Couldn't preparefì for writing for" << destName;
        return false;
    }

    // Read and write data in chunks to minimize memory usage
    QByteArray array;
    array.resize(int(qMin(qint64(1024 * 1024), size)));
    qint64 n;
    qint64 total = 0;
    while ((n = file.read(array.data(), array.size())) > 0) {
        if (!d->handler->writeData(array.data(), n)) {
            qWarning() << "writeData failed";
            return false;
        }
        total += n;
    }
    Q_ASSERT(total == size);

    if (!finishWriting(size)) {
        qWarning() << "finishWriting failed";
        return false;
    }
    return true;
}

/*!
   Writes a local directory into the archive, including all its contents, recursively.
   Calls addLocalFile for each file to be added.

   Iit will also add a @p path that is a symbolic link to a
   directory. The symbolic link will be dereferenced and the content of the
   directory it is pointing to added recursively. However, symbolic links
   *under* @p path will be stored as is.
   \param path full path to an existing local directory, to be added to the archive.
   \param destName the resulting name (or relative path) of the file in the archive.
*/
bool VArchive::addLocalDirectory(const QString &path, const QString &destName)
{
    QDir dir(path);
    if (!dir.exists())
        return false;
    dir.setFilter(dir.filter() | QDir::Hidden);
    const QStringList files = dir.entryList();
    for (QStringList::ConstIterator it = files.begin(); it != files.end(); ++it) {
        if (*it != QLatin1String(".") && *it != QLatin1String("..")) {
            QString fileName = path + QLatin1Char('/') + *it;
            //qDebug() << "storing " << fileName;
            QString dest = destName.isEmpty() ? *it : (destName + QLatin1Char('/') + *it);
            QFileInfo fileInfo(fileName);

            if (fileInfo.isFile() || fileInfo.isSymLink())
                addLocalFile(fileName, dest);
            else if (fileInfo.isDir())
                addLocalDirectory(fileName, dest);
            // XXX: We omit sockets
        }
    }
    return true;
}

/*!
    If an archive is opened for writing then you can add new directories
    using this function. VArchive won't write one directory twice.

    This method also allows some file metadata to be set.
    However, depending on the archive type not all metadata might be regarded.

    \param name the name of the directory
    \param user the user that owns the directory
    \param group the group that owns the directory
    \param perm permissions of the directory
    \param atime time the file was last accessed
    \param mtime modification time of the file
    \param ctime time of last status change
*/
bool VArchive::writeDir(const QString &name, const QString &user, const QString &group,
                        mode_t perm, time_t atime,
                        time_t mtime, time_t ctime)
{
    Q_D(VArchive);
    return d->handler->doWriteDir(name, user, group, perm | 040000, atime, mtime, ctime);
}

/*!
    Writes a symbolic link to the archive if supported.
    The archive must be opened for writing.

    \param name name of symbolic link
    \param target target of symbolic link
    \param user the user that owns the directory
    \param group the group that owns the directory
    \param perm permissions of the directory
    \param atime time the file was last accessed
    \param mtime modification time of the file
    \param ctime time of last status change
*/
bool VArchive::writeSymLink(const QString &name, const QString &target,
                            const QString &user, const QString &group,
                            mode_t perm, time_t atime,
                            time_t mtime, time_t ctime)
{
    Q_D(VArchive);
    return d->handler->doWriteSymLink(name, target, user, group, perm, atime, mtime, ctime);
}

/*!
    If an archive is opened for writing then you can add a new file
    using this function. If the file name is for example "mydir/test1" then
    the directory "mydir" is automatically appended first if that did not
    happen yet.

    This method also allows some file metadata to be
    set. However, depending on the archive type not all metadata might be
    regarded.
    \param name the name of the file
    \param user the user that owns the file
    \param group the group that owns the file
    \param data the data to write (\a size bytes)
    \param size the size of the file
    \param perm permissions of the file
    \param atime time the file was last accessed
    \param mtime modification time of the file
    \param ctime time of last status change
*/
bool VArchive::writeFile(const QString &name, const QString &user,
                         const QString &group, const char *data, qint64 size,
                         mode_t perm, time_t atime, time_t mtime, time_t ctime)
{
    Q_D(VArchive);

    if (!prepareWriting(name, user, group, size, perm, atime, mtime, ctime)) {
        qWarning() << "prepareWriting failed";
        return false;
    }

    // Write data
    // Note: if data is 0L, don't call write, it would terminate the KFilterDev
    if (data && size && !d->handler->writeData(data, size)) {
        qWarning() << "writeData failed";
        return false;
    }

    if (!finishWriting(size)) {
        qWarning() << "finishWriting failed";
        return false;
    }
    return true;
}

/*!
    Here's another way of writing a file into an archive:
    Call prepareWriting(), then call writeData()
    as many times as wanted then call finishWriting( totalSize ).
    For tar.gz files, you need to know the size before hand, it is needed in the header!
    For zip files, size isn't used.

    This method also allows some file metadata to be
    set. However, depending on the archive type not all metadata might be
    regarded.
    \param name the name of the file
    \param user the user that owns the file
    \param group the group that owns the file
    \param size the size of the file
    \param perm permissions of the file
    \param atime time the file was last accessed
    \param mtime modification time of the file
    \param ctime time of last status change
*/
bool VArchive::prepareWriting(const QString &name, const QString &user,
                              const QString &group, qint64 size,
                              mode_t perm, time_t atime,
                              time_t mtime, time_t ctime)
{
    Q_D(VArchive);

    bool ok = d->handler->doPrepareWriting(name, user, group, size, perm, atime, mtime, ctime);
    if (!ok)
        d->handler->abortWriting();
    return ok;
}

/*!
    Call finishWriting after writing the data.
    \param size the size of the file

    \sa prepareWriting()
*/
bool VArchive::finishWriting(qint64 size)
{
    Q_D(VArchive);
    return d->handler->doFinishWriting(size);
}

/*
 * VArchiveEntryPrivate
 */

VArchiveEntryPrivate::VArchiveEntryPrivate(VArchive *_archive,
                                           const QString &_name, int _access,
                                           int _date, const QString &_user,
                                           const QString &_group,
                                           const QString &_symlink) :
    name(_name),
    date(_date),
    access(_access),
    user(_user),
    group(_group),
    symlink(_symlink),
    archive(_archive)
{
}

/*!
    \class VArchiveEntry
    \brief The VArchiveEntry class is the base of all entries in a VArchive.

    \ingroup archives

    \author David Faure <faure@kde.org>
    \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>

    \sa VArchiveFile
    \sa VArchiveDirectory
*/

/*!
    Creates a new entry.
    \param archive the entries archive
    \param name the name of the entry
    \param access the permissions in unix format
    \param date the date (in seconds since 1970)
    \param user the user that owns the entry
    \param group the group that owns the entry
    \param symlink the symlink, or QString()
*/
VArchiveEntry::VArchiveEntry(VArchive *t, const QString &name, int access, int date,
                             const QString &user, const QString &group, const
                             QString &symlink) :
    d_ptr(new VArchiveEntryPrivate(t, name, access, date, user, group, symlink))
{
}

/*!
    Destructor.
*/
VArchiveEntry::~VArchiveEntry()
{
    Q_D(VArchiveEntry);
    delete d;
}

/*!
    Creation date of the file.
    \return the creation date
*/
QDateTime VArchiveEntry::datetime() const
{
    Q_D(const VArchiveEntry);

    QDateTime datetimeobj;
    datetimeobj.setTime_t(d->date);
    return datetimeobj;
}

/*!
    Creation date of the file.
    \return the creation date in seconds since 1970.
*/
int VArchiveEntry::date() const
{
    Q_D(const VArchiveEntry);
    return d->date;
}

/*!
    Name of the file without path.
    \return the file name without path
*/
QString VArchiveEntry::name() const
{
    Q_D(const VArchiveEntry);
    return d->name;
}

/*!
    The permissions and mode flags as returned by the stat() function
    in st_mode.
    \return the permissions
*/
mode_t VArchiveEntry::permissions() const
{
    Q_D(const VArchiveEntry);
    return d->access;
}

/*!
    User who created the file.
    \return the owner of the file
*/
QString VArchiveEntry::user() const
{
    Q_D(const VArchiveEntry);
    return d->user;
}

/*!
    Group of the user who created the file.
    \return the group of the file
*/
QString VArchiveEntry::group() const
{
    Q_D(const VArchiveEntry);
    return d->group;
}

/*!
   Symlink if there is one.
   \return the symlink, or QString()
*/
QString VArchiveEntry::symLinkTarget() const
{
    Q_D(const VArchiveEntry);
    return d->symlink;
}

/*!
    Checks whether the entry is a file.
    \return true if this entry is a file
*/
bool VArchiveEntry::isFile() const
{
    Q_D(const VArchiveEntry);
    return false;
}

/*!
    Checks whether the entry is a directory.
    \return true if this entry is a directory
*/
bool VArchiveEntry::isDirectory() const
{
    Q_D(const VArchiveEntry);
    return false;
}

VArchive *VArchiveEntry::archive() const
{
    Q_D(const VArchiveEntry);
    return d->archive;
}

/*
 * VArchiveFilePrivate
 */

VArchiveFilePrivate::VArchiveFilePrivate(qint64 _pos, qint64 _size) :
    pos(_pos),
    size(_size)
{
}

/*!
    \class VArchiveFile
    \brief VArchiveFile represents a file entry in a VArchive.

    \ingroup archives

    \sa VArchive
    \sa VArchiveDirectory

    \author David Faure <faure@kde.org>
    \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
*/

/*!
    Creates a new file entry. Do not call this, VArchive takes care of it.
    \param archive the entries archive
    \param name the name of the entry
    \param access the permissions in unix format
    \param date the date (in seconds since 1970)
    \param user the user that owns the entry
    \param group the group that owns the entry
    \param symlink the symlink, or QString()
    \param pos the position of the file in the directory
    \param size the size of the file
*/
VArchiveFile::VArchiveFile(VArchive *t, const QString &name, int access, int date,
                           const QString &user, const QString &group,
                           const QString &symlink,
                           qint64 pos, qint64 size)
    : VArchiveEntry(t, name, access, date, user, group, symlink),
      d_ptr(new VArchiveFilePrivate(pos, size))
{
}

/*!
    Destructor. Do not call this, VArchive takes care of it.
*/
VArchiveFile::~VArchiveFile()
{
    Q_D(VArchiveFile);
    delete d;
}

/*!
    Position of the data in the [uncompressed] archive.
    \return the position of the file
*/
qint64 VArchiveFile::position() const
{
    Q_D(const VArchiveFile);
    return d->pos;
}

/*!
    Size of the data.
    \return the size of the file
*/
qint64 VArchiveFile::size() const
{
    Q_D(const VArchiveFile);
    return d->size;
}

/*!
    Set size of data, usually after writing the file.
    \param s the new size of the file
*/
void VArchiveFile::setSize(qint64 s)
{
    Q_D(VArchiveFile);
    d->size = s;
}

/*!
    Returns the data of the file.
    Call data() with care (only once per file), this data isn't cached.
    \return the content of this file.
*/
QByteArray VArchiveFile::data() const
{
    Q_D(const VArchiveFile);

    if (!archive()->device()->seek(d->pos))
        qWarning() << "Failed to sync to" << d->pos << "to read" << name();

    // Read content
    QByteArray arr;
    if (d->size) {
        arr = archive()->device()->read(d->size);
        Q_ASSERT(arr.size() == d->size);
    }
    return arr;
}

/*!
    This method returns QIODevice (internal class: VLimitedIODevice)
    on top of the underlying QIODevice. This is obviously for reading only.

    WARNING: Note that the ownership of the device is being transferred to the caller,
    who will have to delete it.

    The returned device auto-opens (in readonly mode), no need to open it.
    \return the QIODevice of the file
*/
QIODevice *VArchiveFile::createDevice() const
{
    Q_D(const VArchiveFile);
    return new VLimitedIODevice(archive()->device(), d->pos, d->size);
}

/*!
    Checks whether this entry is a file.
    \return true, since this entry is a file
*/
bool VArchiveFile::isFile() const
{
    return true;
}

/*!
    Extracts the file to the directory @p dest
    \param dest the directory to extract to
*/
void VArchiveFile::copyTo(const QString &dest) const
{
    Q_D(const VArchiveFile);

    QFile f(dest + QLatin1Char('/')  + name());
    if (f.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        QIODevice *inputDev = createDevice();

        // Read and write data in chunks to minimize memory usage
        const qint64 chunkSize = 1024 * 1024;
        qint64 remainingSize = d->size;
        QByteArray array;
        array.resize(int(qMin(chunkSize, remainingSize)));

        while (remainingSize > 0) {
            const qint64 currentChunkSize = qMin(chunkSize, remainingSize);
            const qint64 n = inputDev->read(array.data(), currentChunkSize);
            Q_ASSERT(n == currentChunkSize);
            f.write(array.data(), currentChunkSize);
            remainingSize -= currentChunkSize;
        }
        f.close();

        delete inputDev;
    }
}

/*
 * VArchiveDirectoryPrivate
 */

VArchiveDirectoryPrivate::~VArchiveDirectoryPrivate()
{
    qDeleteAll(entries);
}

/*!
    \class VArchiveDirectory <varchive.h> <VArchive>
    \brief The VArchiveDirectory class represents a directory entry in a VArchive.

    \ingroup archives

    \sa VArchive
    \sa VArchiveFile

    \author David Faure <faure@kde.org>
    \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
*/

/*!
    Creates a new directory entry.
    \param archive the entries archive
    \param name the name of the entry
    \param access the permissions in unix format
    \param date the date (in seconds since 1970)
    \param user the user that owns the entry
    \param group the group that owns the entry
    \param symlink the symlink, or QString()
*/
VArchiveDirectory::VArchiveDirectory(VArchive *t, const QString &name, int access,
                                     int date,
                                     const QString &user, const QString &group,
                                     const QString &symlink)
    : VArchiveEntry(t, name, access, date, user, group, symlink),
      d_ptr(new VArchiveDirectoryPrivate)
{
}

/*!
    Destructor.
*/
VArchiveDirectory::~VArchiveDirectory()
{
    Q_D(VArchiveDirectory);
    delete d;
}

/*!
    Returns a list of sub-entries.
    Note that the list is not sorted, it's even in random order (due to using a hashtable).
   Use sort() on the result to sort the list by filename.

    \return the names of all entries in this directory (filenames, no path).
*/
QStringList VArchiveDirectory::entries() const
{
    Q_D(const VArchiveDirectory);
    return d->entries.keys();
}

/*!
    Returns the entry with the given name.
    \param name may be "test1", "mydir/test3", "mydir/mysubdir/test3", etc.
   \return a pointer to the entry in the directory.
*/
const VArchiveEntry *VArchiveDirectory::entry(const QString &_name) const
{
    Q_D(const VArchiveDirectory);

    QString name = QDir::cleanPath(_name);
    int pos = name.indexOf(QLatin1Char('/'));
    if (pos == 0) { // ouch absolute path (see also VArchive::findOrCreate)
        if (name.length() > 1) {
            name = name.mid(1);   // remove leading slash
            pos = name.indexOf(QLatin1Char('/'));   // look again
        } else // "/"
            return this;
    }
    // trailing slash ? -> remove
    if (pos != -1 && pos == name.length() - 1) {
        name = name.left(pos);
        pos = name.indexOf(QLatin1Char('/'));   // look again
    }
    if (pos != -1) {
        const QString left = name.left(pos);
        const QString right = name.mid(pos + 1);

        //qDebug() << "left=" << left << "right=" << right;

        const VArchiveEntry *e = d->entries.value(left);
        if (!e || !e->isDirectory())
            return 0;
        return static_cast<const VArchiveDirectory *>(e)->entry(right);
    }

    return d->entries.value(name);
}

/*!
    \internal
    Adds a new entry to the directory.
*/
void VArchiveDirectory::addEntry(VArchiveEntry *entry)
{
    Q_D(VArchiveDirectory);

    if (entry->name().isEmpty())
        return;

    if (d->entries.value(entry->name())) {
        qWarning() << "directory " << name()
                   << "has entry" << entry->name() << "already";
    }
    d->entries.insert(entry->name(), entry);
}

/*!
    Checks whether this entry is a directory.
   \return true, since this entry is a directory
*/
bool VArchiveDirectory::isDirectory() const
{
    return true;
}

/*!
    \internal
*/
static bool sortByPosition(const VArchiveFile *file1, const VArchiveFile *file2)
{
    return file1->position() < file2->position();
}

/*!
    Extracts all entries in this archive directory to the directory
    @p dest.
    \param dest the directory to extract to
    \param recursive if set to true, subdirectories are extracted as well
*/
void VArchiveDirectory::copyTo(const QString &dest, bool recursiveCopy) const
{
    QDir root;

    QList<const VArchiveFile *> fileList;
    QMap<qint64, QString> fileToDir;

    // placeholders for iterated items
    QStack<const VArchiveDirectory *> dirStack;
    QStack<QString> dirNameStack;

    dirStack.push(this);       // init stack at current directory
    dirNameStack.push(dest);   // ... with given path
    do {
        const VArchiveDirectory *curDir = dirStack.pop();
        const QString curDirName = dirNameStack.pop();
        root.mkdir(curDirName);

        const QStringList dirEntries = curDir->entries();
        for (QStringList::const_iterator it = dirEntries.begin(); it != dirEntries.end(); ++it) {
            const VArchiveEntry *curEntry = curDir->entry(*it);
            if (!curEntry->symLinkTarget().isEmpty()) {
                const QString linkName = curDirName + QLatin1Char('/') + curEntry->name();
#ifdef Q_OS_UNIX
                if (!::symlink(curEntry->symLinkTarget().toLocal8Bit(), linkName.toLocal8Bit()))
                    qDebug() << "symlink(" << curEntry->symLinkTarget() << ',' << linkName << ") failed:" << strerror(errno);
#else
                // TODO - how to create symlinks on other platforms?
#endif
            } else {
                if (curEntry->isFile()) {
                    const VArchiveFile *curFile = dynamic_cast<const VArchiveFile *>(curEntry);
                    if (curFile) {
                        fileList.append(curFile);
                        fileToDir.insert(curFile->position(), curDirName);
                    }
                }

                if (curEntry->isDirectory() && recursiveCopy) {
                    const VArchiveDirectory *ad = dynamic_cast<const VArchiveDirectory *>(curEntry);
                    if (ad) {
                        dirStack.push(ad);
                        dirNameStack.push(curDirName + QLatin1Char('/') + curEntry->name());
                    }
                }
            }
        }
    } while (!dirStack.isEmpty());

    // Sort on d->pos, so we have a linear access
    qSort(fileList.begin(), fileList.end(), sortByPosition);

    for (QList<const VArchiveFile *>::const_iterator it = fileList.constBegin(), end = fileList.constEnd() ;
            it != end ; ++it) {
        const VArchiveFile *f = *it;
        qint64 pos = f->position();
        f->copyTo(fileToDir[pos]);
    }
}
