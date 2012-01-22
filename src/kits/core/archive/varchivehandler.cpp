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

/*
 * VArchiveHandler
 */

VArchiveHandler::VArchiveHandler(const QString &mimeType) :
    d_ptr(new VArchiveHandlerPrivate)
{
}

VArchive *VArchiveHandler::archive() const
{
    Q_D(const VArchiveHandler);
    return d->archive;
}

void VArchiveHandler::setArchive(VArchive *archive)
{
    Q_D(VArchiveHandler);
    d->archive = archive;
}

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

bool VArchiveHandler::writeData(const char *data, qint64 size)
{
    Q_D(VArchiveHandler);

    bool ok = d->device->write(data, size) == size;
    if (!ok)
        d->abortWriting();
    return ok;
}

bool VArchiveHandler::isOpen() const
{
    Q_D(const VArchiveHandler);
    return d->mode != QIODevice::NotOpen;
}

QIODevice::OpenMode VArchiveHandler::mode() const
{
    Q_D(const VArchiveHandler);
    return d->mode;
}

QIODevice *VArchiveHandler::device() const
{
    Q_D(const VArchiveHandler);
    return d->device;
}

void VArchiveHandler::setDevice(QIODevice *dev)
{
    Q_D(VArchiveHandler);

    if (d->deviceOwned)
        delete d->device;
    d->device = dev;
    d->deviceOwned = false;
}

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

void VArchiveHandler::setRootDir(VArchiveDirectory *rootDir)
{
    Q_D(VArchiveHandler);

    // Call setRootDir only once during parsing please ;)
    Q_ASSERT(!d->rootDir);
    d->rootDir = rootDir;
}

QString VArchiveHandler::fileName() const
{
    Q_D(const VArchiveHandler);
    return d->fileName;
}

void VArchiveHandler::setFileName(const QString &fileName)
{
    Q_D(VArchiveHandler);

    // This leaves the device set to 0, it's for the
    // use of VSaveFile, see open()
    Q_ASSERT(!fileName.isEmpty());
    d->fileName = fileName;
}

bool VArchiveHandler::isDeviceOwned() const
{
    Q_D(const VArchiveHandler);
    return d->deviceOwned;
}

void VArchiveHandler::abortWriting()
{
    Q_D(VArchiveHandler);
    d->abortWriting();
}

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
