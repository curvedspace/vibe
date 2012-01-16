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

//#include <config.h>

#include <QStack>
#include <QMap>
#include <QDir>
#include <QFile>
#include <QDebug>

#include <VibeCore/VSaveFile>
#include <VibeCore/VFileSupport>

#include "varchive.h"
#include "varchive_p.h"
#include "vlimitediodevice_p.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef Q_OS_UNIX
#  include <limits.h>
#endif

/*
 * VArchivePrivate
 */

VArchivePrivate::VArchivePrivate() :
    rootDir(0),
    saveFile(0),
    dev(0),
    fileName(),
    mode(QIODevice::NotOpen),
    deviceOwned(false)
{
}

VArchivePrivate::~VArchivePrivate()
{
    delete saveFile;
    delete rootDir;
}

void VArchivePrivate::abortWriting()
{
    if (saveFile) {
        saveFile->abort();
        delete saveFile;
        saveFile = 0;
        dev = 0;
    }
}

/*
 * VArchive
 */

VArchive::VArchive(const QString &fileName) :
    d_ptr(new VArchivePrivate)
{
    Q_D(VArchive);

    Q_ASSERT(!fileName.isEmpty());
    d->fileName = fileName;
    // This constructor leaves the device set to 0.
    // This is for the use of VSaveFile, see open().
}

VArchive::VArchive(QIODevice *dev) :
    d_ptr(new VArchivePrivate)
{
    Q_D(VArchive);
    d->dev = dev;
}

VArchive::~VArchive()
{
    if (isOpen())
        // WARNING: won't call the virtual method close in the derived class!!!
        close();

    delete d_ptr;
}

bool VArchive::open(QIODevice::OpenMode mode)
{
    Q_D(VArchive);

    Q_ASSERT(mode != QIODevice::NotOpen);

    if (isOpen())
        close();

    if (!d->fileName.isEmpty()) {
        Q_ASSERT(!d->dev);
        if (!createDevice(mode))
            return false;
    }

    Q_ASSERT(d->dev);

    if (!d->dev->isOpen() && !d->dev->open(mode))
        return false;

    d->mode = mode;

    Q_ASSERT(!d->rootDir);
    d->rootDir = 0;

    return openArchive(mode);
}

bool VArchive::createDevice(QIODevice::OpenMode mode)
{
    Q_D(VArchive);

    switch (mode) {
        case QIODevice::WriteOnly:
            if (!d->fileName.isEmpty()) {
                // The use of VSaveFile can't be done in the ctor (no mode known yet)
                //qDebug() << "Writing to a file using VSaveFile";
                d->saveFile = new VSaveFile(d->fileName);
                if (!d->saveFile->open()) {
                    qWarning() << "VSaveFile creation for " << d->fileName << " failed, " << d->saveFile->errorString();
                    delete d->saveFile;
                    d->saveFile = 0;
                    return false;
                }
                d->dev = d->saveFile;
                Q_ASSERT(d->dev);
            }
            break;
        case QIODevice::ReadOnly:
        case QIODevice::ReadWrite:
            // ReadWrite mode still uses QFile for now; we'd need to copy to the tempfile, in fact.
            if (!d->fileName.isEmpty()) {
                d->dev = new QFile(d->fileName);
                d->deviceOwned = true;
            }
            break; // continued below
        default:
            qWarning() << "Unsupported mode " << d->mode;
            return false;
    }
    return true;
}

bool VArchive::close()
{
    Q_D(VArchive);

    if (!isOpen())
        return false; // already closed (return false or true? arguable...)

    // moved by holger to allow kzip to write the zip central dir
    // to the file in closeArchive()
    // DF: added d->dev so that we skip closeArchive if saving aborted.
    bool closeSucceeded = true;
    if (d->dev) {
        closeSucceeded = closeArchive();
        if (d->mode == QIODevice::WriteOnly && !closeSucceeded)
            d->abortWriting();
    }

    if (d->dev)
        d->dev->close();

    if (d->deviceOwned) {
        delete d->dev; // we created it ourselves in open()
    }
    if (d->saveFile) {
        closeSucceeded = d->saveFile->finalize();
        delete d->saveFile;
        d->saveFile = 0;
    }

    delete d->rootDir;
    d->rootDir = 0;
    d->mode = QIODevice::NotOpen;
    d->dev = 0;
    return closeSucceeded;
}

const VArchiveDirectory *VArchive::directory() const
{
    // rootDir isn't const so that parsing-on-demand is possible
    return const_cast<VArchive *>(this)->rootDir();
}

bool VArchive::addLocalFile(const QString &fileName, const QString &destName)
{
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
        if (path_max <= 0) {
            path_max = 4096;
        }
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

    // the file must be opened before prepareWriting is called, otherwise
    // if the opening fails, no content will follow the already written
    // header and the tar file is effectively f*cked up
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "couldn't open file " << fileName;
        return false;
    }

    if (!prepareWriting(destName, fileInfo.owner(), fileInfo.group(), size,
                        fi.st_mode, fi.st_atime, fi.st_mtime, fi.st_ctime)) {
        qWarning() << " prepareWriting" << destName << "failed";
        return false;
    }

    // Read and write data in chunks to minimize memory usage
    QByteArray array;
    array.resize(int(qMin(qint64(1024 * 1024), size)));
    qint64 n;
    qint64 total = 0;
    while ((n = file.read(array.data(), array.size())) > 0) {
        if (!writeData(array.data(), n)) {
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
            //            qDebug() << "storing " << fileName;
            QString dest = destName.isEmpty() ? *it : (destName + QLatin1Char('/') + *it);
            QFileInfo fileInfo(fileName);

            if (fileInfo.isFile() || fileInfo.isSymLink())
                addLocalFile(fileName, dest);
            else if (fileInfo.isDir())
                addLocalDirectory(fileName, dest);
            // We omit sockets
        }
    }
    return true;
}

bool VArchive::writeFile(const QString &name, const QString &user,
                         const QString &group, const char *data, qint64 size,
                         mode_t perm, time_t atime, time_t mtime, time_t ctime)
{
    if (!prepareWriting(name, user, group, size, perm, atime, mtime, ctime)) {
        qWarning() << "prepareWriting failed";
        return false;
    }

    // Write data
    // Note: if data is 0L, don't call write, it would terminate the KFilterDev
    if (data && size && !writeData(data, size)) {
        qWarning() << "writeData failed";
        return false;
    }

    if (!finishWriting(size)) {
        qWarning() << "finishWriting failed";
        return false;
    }
    return true;
}

bool VArchive::writeData(const char *data, qint64 size)
{
    Q_D(VArchive);

    bool ok = device()->write(data, size) == size;
    if (!ok)
        d->abortWriting();
    return ok;
}

// The writeDir -> doWriteDir pattern allows to avoid propagating the default
// values into all virtual methods of subclasses, and it allows more extensibility:
// if a new argument is needed, we can add a writeDir overload which stores the
// additional argument in the d pointer, and doWriteDir reimplementations can fetch
// it from there.

bool VArchive::writeDir(const QString &name, const QString &user, const QString &group,
                        mode_t perm, time_t atime,
                        time_t mtime, time_t ctime)
{
    return doWriteDir(name, user, group, perm | 040000, atime, mtime, ctime);
}

bool VArchive::writeSymLink(const QString &name, const QString &target,
                            const QString &user, const QString &group,
                            mode_t perm, time_t atime,
                            time_t mtime, time_t ctime)
{
    return doWriteSymLink(name, target, user, group, perm, atime, mtime, ctime);
}


bool VArchive::prepareWriting(const QString &name, const QString &user,
                              const QString &group, qint64 size,
                              mode_t perm, time_t atime,
                              time_t mtime, time_t ctime)
{
    Q_D(VArchive);

    bool ok = doPrepareWriting(name, user, group, size, perm, atime, mtime, ctime);
    if (!ok)
        d->abortWriting();
    return ok;
}

bool VArchive::finishWriting(qint64 size)
{
    return doFinishWriting(size);
}

VArchiveDirectory *VArchive::rootDir()
{
    Q_D(VArchive);

    if (!d->rootDir) {
        //qDebug() << "Making root dir ";
        struct passwd *pw =  getpwuid(getuid());
        struct group *grp = getgrgid(getgid());
        QString username = pw ? QFile::decodeName(pw->pw_name) : QString::number(getuid());
        QString groupname = grp ? QFile::decodeName(grp->gr_name) : QString::number(getgid());

        d->rootDir = new VArchiveDirectory(this, QLatin1String("/"), (int)(0777 + S_IFDIR), 0, username, groupname, QString());
    }
    return d->rootDir;
}

VArchiveDirectory *VArchive::findOrCreate(const QString &path)
{
    Q_D(VArchive);

    //qDebug() << path;
    if (path.isEmpty() || path == QLatin1String("/") || path == QLatin1String(".")) { // root dir => found
        //qDebug() << "returning rootdir";
        return rootDir();
    }
    // Important note : for tar files containing absolute paths
    // (i.e. beginning with "/"), this means the leading "/" will
    // be removed (no KDirectory for it), which is exactly the way
    // the "tar" program works (though it displays a warning about it)
    // See also VArchiveDirectory::entry().

    // Already created ? => found
    const VArchiveEntry *ent = rootDir()->entry(path);
    if (ent) {
        if (ent->isDirectory())
            //qDebug() << "found it";
            return (VArchiveDirectory *) ent;
        else
            qWarning() << "Found" << path << "but it's not a directory";
    }

    // Otherwise go up and try again
    int pos = path.lastIndexOf(QLatin1Char('/'));
    VArchiveDirectory *parent;
    QString dirname;
    if (pos == -1) { // no more slash => create in root dir
        parent =  rootDir();
        dirname = path;
    } else {
        QString left = path.left(pos);
        dirname = path.mid(pos + 1);
        parent = findOrCreate(left);   // recursive call... until we find an existing dir.
    }

    //qDebug() << "found parent " << parent->name() << " adding " << dirname << " to ensure " << path;
    // Found -> add the missing piece
    VArchiveDirectory *e = new VArchiveDirectory(this, dirname, d->rootDir->permissions(),
                                                 d->rootDir->date(), d->rootDir->user(),
                                                 d->rootDir->group(), QString());
    parent->addEntry(e);
    return e; // now a directory to <path> exists
}

void VArchive::setDevice(QIODevice *dev)
{
    Q_D(VArchive);

    if (d->deviceOwned)
        delete d->dev;
    d->dev = dev;
    d->deviceOwned = false;
}

void VArchive::setRootDir(VArchiveDirectory *rootDir)
{
    Q_D(VArchive);

    // Call setRootDir only once during parsing please ;)
    Q_ASSERT(!d->rootDir);
    d->rootDir = rootDir;
}

QIODevice::OpenMode VArchive::mode() const
{
    Q_D(const VArchive);
    return d->mode;
}

QIODevice *VArchive::device() const
{
    Q_D(const VArchive);
    return d->dev;
}

bool VArchive::isOpen() const
{
    Q_D(const VArchive);
    return d->mode != QIODevice::NotOpen;
}

QString VArchive::fileName() const
{
    Q_D(const VArchive);
    return d->fileName;
}

/*
 * VArchiveEntryPrivate
 */

VArchiveEntryPrivate::VArchiveEntryPrivate(VArchive *_archive, const QString &_name, int _access,
                                           int _date, const QString &_user, const QString &_group,
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

/*
 * VArchiveEntry
 */

VArchiveEntry::VArchiveEntry(VArchive *t, const QString &name, int access, int date,
                             const QString &user, const QString &group, const
                             QString &symlink) :
    d_ptr(new VArchiveEntryPrivate(t, name, access, date, user, group, symlink))
{
}

VArchiveEntry::~VArchiveEntry()
{
    Q_D(VArchiveEntry);
    delete d;
}

QDateTime VArchiveEntry::datetime() const
{
    Q_D(const VArchiveEntry);

    QDateTime datetimeobj;
    datetimeobj.setTime_t(d->date);
    return datetimeobj;
}

int VArchiveEntry::date() const
{
    Q_D(const VArchiveEntry);
    return d->date;
}

QString VArchiveEntry::name() const
{
    Q_D(const VArchiveEntry);
    return d->name;
}

mode_t VArchiveEntry::permissions() const
{
    Q_D(const VArchiveEntry);
    return d->access;
}

QString VArchiveEntry::user() const
{
    Q_D(const VArchiveEntry);
    return d->user;
}

QString VArchiveEntry::group() const
{
    Q_D(const VArchiveEntry);
    return d->group;
}

QString VArchiveEntry::symLinkTarget() const
{
    Q_D(const VArchiveEntry);
    return d->symlink;
}

bool VArchiveEntry::isFile() const
{
    Q_D(const VArchiveEntry);
    return false;
}

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

/*
 * VArchiveFile
 */

VArchiveFile::VArchiveFile(VArchive *t, const QString &name, int access, int date,
                           const QString &user, const QString &group,
                           const QString &symlink,
                           qint64 pos, qint64 size)
    : VArchiveEntry(t, name, access, date, user, group, symlink),
      d_ptr(new VArchiveFilePrivate(pos, size))
{
}

VArchiveFile::~VArchiveFile()
{
    Q_D(VArchiveFile);
    delete d;
}

qint64 VArchiveFile::position() const
{
    Q_D(const VArchiveFile);
    return d->pos;
}

qint64 VArchiveFile::size() const
{
    Q_D(const VArchiveFile);
    return d->size;
}

void VArchiveFile::setSize(qint64 s)
{
    Q_D(VArchiveFile);
    d->size = s;
}

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

QIODevice *VArchiveFile::createDevice() const
{
    Q_D(const VArchiveFile);
    return new VLimitedIODevice(archive()->device(), d->pos, d->size);
}

bool VArchiveFile::isFile() const
{
    return true;
}

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

/*
 * VArchiveDirectory
 */

VArchiveDirectory::VArchiveDirectory(VArchive *t, const QString &name, int access,
                                     int date,
                                     const QString &user, const QString &group,
                                     const QString &symlink)
    : VArchiveEntry(t, name, access, date, user, group, symlink),
      d_ptr(new VArchiveDirectoryPrivate)
{
}

VArchiveDirectory::~VArchiveDirectory()
{
    Q_D(VArchiveDirectory);
    delete d;
}

QStringList VArchiveDirectory::entries() const
{
    Q_D(const VArchiveDirectory);
    return d->entries.keys();
}

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

bool VArchiveDirectory::isDirectory() const
{
    return true;
}

static bool sortByPosition(const VArchiveFile *file1, const VArchiveFile *file2)
{
    return file1->position() < file2->position();
}

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

    qSort(fileList.begin(), fileList.end(), sortByPosition);    // sort on d->pos, so we have a linear access

    for (QList<const VArchiveFile *>::const_iterator it = fileList.constBegin(), end = fileList.constEnd() ;
            it != end ; ++it) {
        const VArchiveFile *f = *it;
        qint64 pos = f->position();
        f->copyTo(fileToDir[pos]);
    }
}

void VArchive::virtual_hook(int, void *)
{
    /*BASE::virtual_hook( id, data )*/;
}

void VArchiveEntry::virtual_hook(int, void *)
{
    /*BASE::virtual_hook( id, data );*/
}

void VArchiveFile::virtual_hook(int id, void *data)
{
    VArchiveEntry::virtual_hook(id, data);
}

void VArchiveDirectory::virtual_hook(int id, void *data)
{
    VArchiveEntry::virtual_hook(id, data);
}
