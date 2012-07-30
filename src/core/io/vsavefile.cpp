/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 1999 Waldo Bastian
 * Copyright (c) 2006 Allen Winter
 * Copyright (c) 2006 Gregory S. Hayes
 * Copyright (c) 2006 Jaison Lee
 * Copyright (c) 2010-2011 Pier Luigi Fiorini
 *
 * Author(s):
 *    Waldo Bastian <bastian@kde.org>
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Gregory S. Hayes <syncomm@kde.org>
 *    Jaison Lee <lee.jaison@gmail.com>
 *    Allen Winter <winter@kde.org>
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

#include <QDir>
#include <QTemporaryFile>
#include <QSettings>
#include <QFileInfo>

#include "vsavefile.h"

extern "C" {
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
}

class VSaveFile::Private
{
public:
    QString realFileName; //The name of the end-result file
    QString tempFileName; //The name of the temp file we are using

    QFile::FileError error;
    QString errorString;
    bool wasFinalized;

    Private() {
        error = QFile::NoError;
        wasFinalized = false;
    }
};

VSaveFile::VSaveFile()
    : d(new Private())
{
}

VSaveFile::VSaveFile(const QString &filename)
    : d(new Private())
{
    VSaveFile::setFileName(filename);
}

VSaveFile::~VSaveFile()
{
    if (!d->wasFinalized)
        finalize();

    delete d;
}

bool VSaveFile::open(OpenMode flags)
{
    if (d->realFileName.isNull()) {
        d->error = QFile::OpenError;
        d->errorString = tr("No target filename has been given.");
        return false;
    }

    if (!d->tempFileName.isNull()) {
#if 0 // do not set an error here, this open() fails, but the file itself is without errors
        d->error = QFile::OpenError;
        d->errorString = tr("Already opened.");
#endif
        return false;
    }

    // We only check here if the directory can be written to the actual
    // filename isn't written to, but replaced later with the contents
    // of our tempfile
    QFileInfo realFileInfo(d->realFileName);
    QFileInfo realFilePathInfo(realFileInfo.absolutePath());
    if (!realFilePathInfo.permission(QFile::WriteUser)) {
        d->error = QFile::PermissionsError;
        d->errorString = tr("Insufficient permissions in target directory.");
        return false;
    }

    //Create our temporary file
    QTemporaryFile tempFile;
    tempFile.setAutoRemove(false);
    tempFile.setFileTemplate(d->realFileName + QLatin1String("XXXXXX.new"));
    if (!tempFile.open()) {
        d->error = QFile::OpenError;
        d->errorString = tr("Unable to open temporary file.");
        return false;
    }

    // if we're overwriting an existing file, ensure temp file's
    // permissions are the same as existing file so the existing
    // file's permissions are preserved. this will succeed completely
    // only if we are the same owner and group - or allmighty root.
    QFileInfo fi(d->realFileName);
    if (fi.exists()) {
        //Qt apparently has no way to change owner/group of file :(
        if (fchown(tempFile.handle(), fi.ownerId(), fi.groupId())) {
            // failed to set user and group => try to restore group only.
            fchown(tempFile.handle(), -1, fi.groupId());
        }

        tempFile.setPermissions(fi.permissions());
    } else {
        mode_t umsk = umask(0);
        fchmod(tempFile.handle(), 0666 & (~umsk));
    }

    //Open oursleves with the temporary file
    QFile::setFileName(tempFile.fileName());
    if (!QFile::open(flags)) {
        tempFile.setAutoRemove(true);
        return false;
    }

    d->tempFileName = tempFile.fileName();
    d->error = QFile::NoError;
    d->errorString.clear();
    return true;
}

void VSaveFile::setFileName(const QString &filename)
{
    d->realFileName = filename;

    // Make absolute if needed
    if (QDir::isRelativePath(filename))
        d->realFileName = QDir::current().absoluteFilePath(filename);

    // Follow symbolic link, if any
    QFileInfo realFileInfo(d->realFileName);
    if (realFileInfo.isSymLink())
        d->realFileName = realFileInfo.symLinkTarget();
}

QFile::FileError VSaveFile::error() const
{
    if (d->error != QFile::NoError)
        return d->error;
    return QFile::error();
}

QString VSaveFile::errorString() const
{
    if (!d->errorString.isEmpty())
        return d->errorString;
    return QFile::errorString();
}

QString VSaveFile::fileName() const
{
    return d->realFileName;
}

void VSaveFile::abort()
{
    close();
    QFile::remove(d->tempFileName); //non-static QFile::remove() does not work.
    d->wasFinalized = true;
}

#ifdef HAVE_FDATASYNC
#  define FDATASYNC fdatasync
#else
#  define FDATASYNC fsync
#endif

bool VSaveFile::finalize()
{
    bool success = false;

    if (!d->wasFinalized) {

#ifdef Q_OS_UNIX
        static int extraSync = -1;
        if (extraSync < 0)
            extraSync = getenv("VIBE_EXTRA_FSYNC") != 0 ? 1 : 0;
        if (extraSync) {
            if (flush()) {
                forever {
                    if (!FDATASYNC(handle()))
                        break;
                    if (errno != EINTR) {
                        d->error = QFile::WriteError;
                        d->errorString = tr("Synchronization to disk failed");
                        break;
                    }
                }
            }
        }
#endif

        close();

        if (error() != NoError)
            QFile::remove(d->tempFileName);

        // Qt does not allow us to atomically overwrite an existing file,
        // so if the target file already exists, there is no way to change it
        // to the temp file without creating a small race condition. So we use
        // the standard rename call instead, which will do the copy without the
        // race condition.
        else if (::rename(QFile::encodeName(d->tempFileName).constData(), QFile::encodeName(d->realFileName).constData()) == 0) {
            d->error = QFile::NoError;
            d->errorString.clear();
            success = true;
        } else {
            d->error = QFile::OpenError;
            d->errorString = tr("Error during rename.");
            QFile::remove(d->tempFileName);
        }

        d->wasFinalized = true;
    }

    return success;
}

#undef FDATASYNC

bool VSaveFile::backupFile(const QString &qFilename, const QString &backupDir)
{
    // get backup type from config, by default use "simple"
    // get extension from config, by default use "~"
    // get max number of backups from config, by default set to 10

    QSettings settings("Vision", "Desktop");
    QString type = settings.value("backups/type", "simple").toString();
    QString extension = settings.value("backups/extension", "~").toString();
    int maxnum = settings.value("backups/maxnum", 10).toInt();

    if (type.toLower() == QLatin1String("numbered"))
        return numberedBackupFile(qFilename, backupDir, extension, maxnum);
    else
        return simpleBackupFile(qFilename, backupDir, extension);
}

bool VSaveFile::simpleBackupFile(const QString &qFilename,
                                 const QString &backupDir,
                                 const QString &backupExtension)
{
    QString backupFileName = qFilename + backupExtension;

    if (!backupDir.isEmpty()) {
        QFileInfo fileInfo(qFilename);
        backupFileName = backupDir + QLatin1Char('/') + fileInfo.fileName() + backupExtension;
    }

    //qDebug() << "VSaveFile copying " << qFilename << " to " << backupFileName;
    QFile::remove(backupFileName);
    return QFile::copy(qFilename, backupFileName);
}

bool VSaveFile::numberedBackupFile(const QString &qFilename,
                                   const QString &backupDir,
                                   const QString &backupExtension,
                                   const uint maxBackups)
{
    QFileInfo fileInfo(qFilename);

    // The backup file name template.
    QString sTemplate;
    if (backupDir.isEmpty())
        sTemplate = qFilename + QLatin1String(".%1") + backupExtension;
    else
        sTemplate = backupDir + QLatin1Char('/') + fileInfo.fileName() + QLatin1String(".%1") + backupExtension;

    // First, search backupDir for numbered backup files to remove.
    // Remove all with number 'maxBackups' and greater.
    QDir d = backupDir.isEmpty() ? fileInfo.dir() : backupDir;
    d.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    const QStringList nameFilters = QStringList(fileInfo.fileName() + QLatin1String(".*") + backupExtension);
    d.setNameFilters(nameFilters);
    d.setSorting(QDir::Name);

    uint maxBackupFound = 0;
    foreach(const QFileInfo & fi, d.entryInfoList()) {
        if (fi.fileName().endsWith(backupExtension)) {
            // sTemp holds the file name, without the ending backupExtension
            QString sTemp = fi.fileName();
            sTemp.truncate(fi.fileName().length() - backupExtension.length());
            // compute the backup number
            int idex = sTemp.lastIndexOf(QLatin1Char('.'));
            if (idex > 0) {
                bool ok;
                uint num = sTemp.mid(idex + 1).toUInt(&ok);
                if (ok) {
                    if (num >= maxBackups) {
                        QFile::remove(fi.filePath());
                    } else {
                        maxBackupFound = qMax(maxBackupFound, num);
                    }
                }
            }
        }
    }

    // Next, rename max-1 to max, max-2 to max-1, etc.
    QString to = sTemplate.arg(maxBackupFound + 1);
    for (int i = maxBackupFound; i > 0; i--) {
        QString from = sTemplate.arg(i);
        //qDebug() << "VSaveFile renaming " << from << " to " << to;
        QFile::rename(from, to);
        to = from;
    }

    // Finally create most recent backup by copying the file to backup number 1.
    //qDebug() << "VSaveFile copying " << qFilename << " to " << sTemplate.arg(1);
    return QFile::copy(qFilename, sTemplate.arg(1));
}
