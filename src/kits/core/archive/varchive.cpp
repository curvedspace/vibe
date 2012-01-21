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

#include <QStack>
#include <QDir>
#include <QDebug>
#include <QPluginLoader>

#include <VibeCore/VFileSupport>
#include <VibeCore/VMimeType>
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

/*
 * VArchive
 */

VArchive::VArchive(const QString &fileName) :
    d_ptr(new VArchivePrivate)
{
    Q_D(VArchive);

    Q_ASSERT(!fileName.isEmpty());

    // Try to find the MIME Type for the specified file, otherwise give up
    VMimeType mimeType;
    if (!mimeType.fromFileName(fileName))
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
                handler = plugin->create(mimeType.mimeType());
                if (handler)
                    break;
            }
        }
    }
    if (!handler)
        qFatal("No archive handler for %s, cannot continue!",
               qPrintable(mimeType.mimeType()));
    d->handler = handler;
    d->handler->setArchive(this);
    d->handler->setFileName(fileName);
}

VArchive::VArchive(QIODevice *dev) :
    d_ptr(new VArchivePrivate)
{
    Q_D(VArchive);

    // Try to find the MIME Type for the specified file, otherwise give up
    VMimeType mimeType;
    if (!mimeType.fromDevice(dev))
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
                handler = plugin->create(mimeType.mimeType());
                if (handler)
                    break;
            }
        }
    }
    if (!handler)
        qFatal("No archive handler for %s, cannot continue!",
               qPrintable(mimeType.mimeType()));
    d->handler = handler;
    d->handler->setArchive(this);
    d->handler->setDevice(dev);
}

VArchive::~VArchive()
{
    if (isOpen())
        close();

    delete d_ptr;
}

bool VArchive::open(QIODevice::OpenMode mode)
{
    Q_D(VArchive);
    return d->handler->open(mode);
}

bool VArchive::close()
{
    Q_D(VArchive);
    return d->handler->close();
}

bool VArchive::isOpen() const
{
    Q_D(const VArchive);
    return d->handler->isOpen();
}

QIODevice::OpenMode VArchive::mode() const
{
    Q_D(const VArchive);
    return d->handler->mode();
}

QIODevice *VArchive::device() const
{
    Q_D(const VArchive);
    return d->handler->device();
}

QString VArchive::fileName() const
{
    Q_D(const VArchive);
    return d->handler->fileName();
}

const VArchiveDirectory *VArchive::directory() const
{
    Q_D(const VArchive);
    return d->handler->rootDir();
}

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
            // We omit sockets
        }
    }
    return true;
}

bool VArchive::writeDir(const QString &name, const QString &user, const QString &group,
                        mode_t perm, time_t atime,
                        time_t mtime, time_t ctime)
{
    Q_D(VArchive);
    return d->handler->doWriteDir(name, user, group, perm | 040000, atime, mtime, ctime);
}

bool VArchive::writeSymLink(const QString &name, const QString &target,
                            const QString &user, const QString &group,
                            mode_t perm, time_t atime,
                            time_t mtime, time_t ctime)
{
    Q_D(VArchive);
    return d->handler->doWriteSymLink(name, target, user, group, perm, atime, mtime, ctime);
}

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

bool VArchive::finishWriting(qint64 size)
{
    Q_D(VArchive);
    return d->handler->doFinishWriting(size);
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
