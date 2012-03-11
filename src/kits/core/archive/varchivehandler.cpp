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

#include <QDebug>

#include <VibeCore/VSaveFile>
#include <VibeCore/VArchive>

#include "varchivehandler.h"
#include "varchivehandler_p.h"

#include <grp.h>
#include <pwd.h>

/*
 * VArchiveHandlerPrivate
 */

VArchiveHandlerPrivate::VArchiveHandlerPrivate() :
    archive(0),
    rootDir(0),
    saveFile(0),
    device(0),
    fileName(),
    mode(QIODevice::NotOpen),
    deviceOwned(false)
{
}

VArchiveHandlerPrivate::~VArchiveHandlerPrivate()
{
    delete saveFile;
    delete rootDir;
}

void VArchiveHandlerPrivate::abortWriting()
{
    if (saveFile) {
        saveFile->abort();
        delete saveFile;
        saveFile = 0;
        device = 0;
    }
}

/*!
    \class VArchiveHandler varchivehandler.h <VArchiveHandler>
    \ingroup core Core Kit

    \brief Base class for archive handlers.

    \author David Faure <faure@kde.org>
    \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
*/

/*!
   Base constructor.
*/
VArchiveHandler::VArchiveHandler(const QString &mimeType) :
    d_ptr(new VArchiveHandlerPrivate)
{
}

/*!
    Returns the pointer to the VArchive object that loaded this plugin.
*/
VArchive *VArchiveHandler::archive() const
{
    Q_D(const VArchiveHandler);
    return d->archive;
}

/*!
    Saves a pointer to the VArchive object that loaded this plugin.
    The pointer will later be used to create the root directory.

    \sa rootDir()
*/
void VArchiveHandler::setArchive(VArchive *archive)
{
    Q_D(VArchiveHandler);
    d->archive = archive;
}

/*!
    Opens the archive for reading or writing.
    \param mode may be QIODevice::ReadOnly or QIODevice::WriteOnly

    \sa close
*/
bool VArchiveHandler::open(QIODevice::OpenMode mode)
{
    Q_D(VArchiveHandler);

    Q_ASSERT(mode != QIODevice::NotOpen);

    if (isOpen())
        close();

    if (!d->fileName.isEmpty()) {
        Q_ASSERT(!d->device);
        if (!createDevice(mode))
            return false;
    }

    Q_ASSERT(d->device);

    if (!d->device->isOpen() && !d->device->open(mode))
        return false;

    d->mode = mode;

    Q_ASSERT(!d->rootDir);
    d->rootDir = 0;

    return openArchive(mode);
}

/*!
    Closes the archive.
    Inherited classes might want to reimplement closeArchive instead,
   because this method is only called by VArchive.

   \return true if close succeeded without problems

   \sa open
*/
bool VArchiveHandler::close()
{
    Q_D(VArchiveHandler);

    // If already closed return false
    if (!isOpen())
        return false;

    // moved by holger to allow kzip to write the zip central dir
    // to the file in closeArchive()
    // DF: added d->dev so that we skip closeArchive if saving aborted.
    bool closeSucceeded = true;
    if (d->device) {
        closeSucceeded = closeArchive();
        if (d->mode == QIODevice::WriteOnly && !closeSucceeded)
            d->abortWriting();
    }

    // Close the device
    if (d->device)
        d->device->close();

    // We created it ourselves in open()
    if (d->deviceOwned)
        delete d->device;
    if (d->saveFile) {
        closeSucceeded = d->saveFile->finalize();
        delete d->saveFile;
        d->saveFile = 0;
    }

    delete d->rootDir;
    d->rootDir = 0;
    d->mode = QIODevice::NotOpen;
    d->device = 0;
    return closeSucceeded;
}

/*!
    Checks whether the archive is open.
    \return true if the archive is opened
*/
bool VArchiveHandler::isOpen() const
{
    Q_D(const VArchiveHandler);
    return d->mode != QIODevice::NotOpen;
}

/*!
    Returns the mode in which the archive was opened.
    \return the mode in which the archive was opened (QIODevice::ReadOnly or QIODevice::WriteOnly)

    \see open()
*/
QIODevice::OpenMode VArchiveHandler::mode() const
{
    Q_D(const VArchiveHandler);
    return d->mode;
}

/*!
    The underlying device.
    \return the underlying device.
*/
QIODevice *VArchiveHandler::device() const
{
    Q_D(const VArchiveHandler);
    return d->device;
}

/*!
    Sets the underlying device.
    Note that this can be a file, but also a data buffer, a compression filter, etc.
    For a file in writing mode it is better to use the other constructor
    though, to benefit from the use of VSaveFile when saving.
    \param dev the I/O device where the archive reads its data
*/
void VArchiveHandler::setDevice(QIODevice *dev)
{
    Q_D(VArchiveHandler);

    if (d->deviceOwned)
        delete d->device;
    d->device = dev;
    d->deviceOwned = false;
}

/*!
    Retrieves or create the root directory.
    The default implementation assumes that openArchive() did the parsing,
    so it creates a dummy rootdir if none was set (write mode, or no '/' in the archive).
    Reimplement this to provide parsing/listing on demand.
    \return the root directory
*/
VArchiveDirectory *VArchiveHandler::rootDir()
{
    Q_D(VArchiveHandler);

    Q_ASSERT(d->archive);

    if (!d->rootDir) {
        //qDebug() << "Making root dir ";
        struct passwd *pw =  getpwuid(getuid());
        struct group *grp = getgrgid(getgid());
        QString username = pw ? QFile::decodeName(pw->pw_name) : QString::number(getuid());
        QString groupname = grp ? QFile::decodeName(grp->gr_name) : QString::number(getgid());

        d->rootDir = new VArchiveDirectory(d->archive, QLatin1String("/"), (int)(0777 + S_IFDIR), 0, username, groupname, QString());
    }
    return d->rootDir;
}

/*!
    Derived classes call setRootDir from openArchive,
    to set the root directory after parsing an existing archive.
*/
void VArchiveHandler::setRootDir(VArchiveDirectory *rootDir)
{
    Q_D(VArchiveHandler);

    // Call setRootDir only once during parsing please ;)
    Q_ASSERT(!d->rootDir);
    d->rootDir = rootDir;
}

/*!
    Returns the name of the archive file or an empty string if not set.
    \return the name of the file, or QString() if unknown
*/
QString VArchiveHandler::fileName() const
{
    Q_D(const VArchiveHandler);
    return d->fileName;
}

/*!
    Sets the name of the archive file.
    \param fileName is a local path (e.g. "/common/var/tmp/myfile.ext"),
    from which the archive will be read from, or into which the archive
    will be written, depending on the mode given to open().
*/
void VArchiveHandler::setFileName(const QString &fileName)
{
    Q_D(VArchiveHandler);

    // This leaves the device set to 0, it's for the
    // use of VSaveFile, see open()
    Q_ASSERT(!fileName.isEmpty());
    d->fileName = fileName;
}

/*!
    Returns whether the device is owned.
*/
bool VArchiveHandler::isDeviceOwned() const
{
    Q_D(const VArchiveHandler);
    return d->deviceOwned;
}

/*!
    VArchive calls this to write data into the current file, after calling prepareWriting.
*/
bool VArchiveHandler::writeData(const char *data, qint64 size)
{
    Q_D(VArchiveHandler);

    bool ok = d->device->write(data, size) == size;
    if (!ok)
        d->abortWriting();
    return ok;
}

/*!
    Ensures that @p path exists, create otherwise.
    This handles e.g. tar files missing directory entries, like mico-2.3.0.tar.gz :)
    \param path the path of the directory
    \return the directory with the given @p path
*/
VArchiveDirectory *VArchiveHandler::findOrCreate(const QString &path)
{
    Q_D(VArchiveHandler);

    Q_ASSERT(d->archive);

    if (path.isEmpty() || path == QLatin1String("/") || path == QLatin1String("."))
        return rootDir();

    // Important note : for tar files containing absolute paths
    // (i.e. beginning with "/"), this means the leading "/" will
    // be removed (no KDirectory for it), which is exactly the way
    // the "tar" program works (though it displays a warning about it)
    // See also VArchiveDirectory::entry().

    // Already created ? => found
    const VArchiveEntry *ent = rootDir()->entry(path);
    if (ent) {
        if (ent->isDirectory())
            return (VArchiveDirectory *) ent;
        else
            qWarning() << "Found" << path << "but it's not a directory";
    }

    // Otherwise go up and try again
    int pos = path.lastIndexOf(QLatin1Char('/'));
    VArchiveDirectory *parent;
    QString dirname;
    if (pos == -1) {
        // No more slash => create in root dir
        parent =  rootDir();
        dirname = path;
    } else {
        QString left = path.left(pos);
        dirname = path.mid(pos + 1);

        // Recursive call... until we find an existing directory
        parent = findOrCreate(left);
    }

    //qDebug() << "found parent " << parent->name() << " adding " << dirname << " to ensure " << path;
    // Found -> add the missing piece
    VArchiveDirectory *e = new VArchiveDirectory(d->archive, dirname, d->rootDir->permissions(),
                                                 d->rootDir->date(), d->rootDir->user(),
                                                 d->rootDir->group(), QString());
    parent->addEntry(e);
    return e;
}

/*!
    Aborts writing to the open device.
*/
void VArchiveHandler::abortWriting()
{
    Q_D(VArchiveHandler);
    d->abortWriting();
}

/*!
    Can be reimplemented in order to change the creation of the device
    (when using the fileName constructor). By default this method uses
    VSaveFile when saving, and a simple QFile on reading.
    This method is called by open().
*/
bool VArchiveHandler::createDevice(QIODevice::OpenMode mode)
{
    Q_D(VArchiveHandler);

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
                d->device = d->saveFile;
                Q_ASSERT(d->device);
            }
            break;
        case QIODevice::ReadOnly:
        case QIODevice::ReadWrite:
            // ReadWrite mode still uses QFile for now; we'd need to copy to the tempfile, in fact.
            if (!d->fileName.isEmpty()) {
                d->device = new QFile(d->fileName);
                d->deviceOwned = true;
            }
            break; // continued below
        default:
            qWarning() << "Unsupported mode " << d->mode;
            return false;
    }
    return true;
}
