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

#include <QDir>
#include <QFile>
#include <QTemporaryFile>
#include <QDebug>

#include <VAbstractCompressionFilter>
#include <VCompressionFilter>
#include <VArchive>
#include <VArchiveHandlerPlugin>

#include "tararchivehandler.h"

// Mime types of known filters
static const char application_gzip[] = "application/x-gzip";
static const char application_bzip[] = "application/x-bzip";
static const char application_lzma[] = "application/x-lzma";
static const char application_xz[] = "application/x-xz";
static const char application_zip[] = "application/zip";

/*
 * TarPlugin
 */

class TarPlugin : public VArchiveHandlerPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.hawaii.Vibe.VArchiveHandlerFactoryInterface" FILE "tar.json")
public:
    QStringList mimeTypes() const {
        QStringList types;
        types << "application/x-tar"
              << "application/x-compressed-tar"
              << "application/x-bzip-compressed-tar"
              << "application/x-lzma-compressed-tar"
              << "application/x-xz-compressed-tar";

        return types;
    }

    VArchiveHandler *create(const QString &mimeType) {
        if (mimeTypes().contains(mimeType))
            return new TarArchiveHandler(mimeType);
        return 0;
    }
};

/*
 * TarArchiveHandlerPrivate
 */

class TarArchiveHandlerPrivate
{
public:
    TarArchiveHandlerPrivate(TarArchiveHandler *parent)
        : q(parent),
          tarEnd(0),
          tmpFile(0) {
    }

    TarArchiveHandler *q;
    QStringList dirList;
    qint64 tarEnd;
    QTemporaryFile *tmpFile;
    QString mimeType;
    QByteArray origFileName;

    bool fillTempFile(const QString &fileName);
    bool writeBackTempFile(const QString &fileName);
    void fillBuffer(char *buffer, const char *mode, qint64 size, time_t mtime,
                    char typeflag, const char *uname, const char *gname);
    void writeLonglink(char *buffer, const QByteArray &name, char typeflag,
                       const char *uname, const char *gname);
    qint64 readRawHeader(char *buffer);
    bool readLonglink(char *buffer, QByteArray &longlink);
    qint64 readHeader(char *buffer, QString &name, QString &symlink);
};

/*
 * If we have created a temporary file, we have
 * to decompress the original file now and write
 * the contents to the temporary file.
 */
bool TarArchiveHandlerPrivate::fillTempFile(const QString &fileName)
{
    if (! tmpFile)
        return true;

    //qDebug() << "filling tmpFile of mimetype" << mimetype;

    bool forced = false;
    if (QLatin1String(application_gzip) == mimeType || QLatin1String(application_bzip) == mimeType)
        forced = true;

    QIODevice *filterDev = VCompressionFilter::deviceForFile(fileName, mimeType, forced);

    if (filterDev) {
        QFile *file = tmpFile;
        Q_ASSERT(file->isOpen());
        Q_ASSERT(file->openMode() & QIODevice::WriteOnly);
        file->seek(0);
        QByteArray buffer;
        buffer.resize(8 * 1024);
        if (! filterDev->open(QIODevice::ReadOnly)) {
            delete filterDev;
            return false;
        }
        qint64 len = -1;
        while (!filterDev->atEnd() && len != 0) {
            len = filterDev->read(buffer.data(), buffer.size());
            if (len < 0) {   // corrupted archive
                delete filterDev;
                return false;
            }
            if (file->write(buffer.data(), len) != len) {   // disk full
                delete filterDev;
                return false;
            }
        }
        filterDev->close();
        delete filterDev;

        file->flush();
        file->seek(0);
        Q_ASSERT(file->isOpen());
        Q_ASSERT(file->openMode() & QIODevice::ReadOnly);
    } else {
        qDebug() << "no filterdevice found!";
    }

    //qDebug() << "filling tmpFile finished.";
    return true;
}

/*
 * Writes back the changes of the temporary file
 * to the original file.
 * Must only be called if in write mode, not in read mode
 */
bool TarArchiveHandlerPrivate::writeBackTempFile(const QString &fileName)
{
    if (!tmpFile)
        return true;

    //qDebug() << "Write temporary file to compressed file" << fileName << mimetype;

    bool forced = false;
    if (QLatin1String(application_gzip) == mimeType || QLatin1String(application_bzip) == mimeType ||
            QLatin1String(application_lzma) == mimeType || QLatin1String(application_xz) == mimeType)
        forced = true;

    // #### TODO this should use KSaveFile to avoid problems on disk full
    // (VArchive uses KSaveFile by default, but the temp-uncompressed-file trick
    // circumvents that).

    QIODevice *dev = VCompressionFilter::deviceForFile(fileName, mimeType, forced);
    if (dev) {
        QFile *file = tmpFile;
        if (!dev->open(QIODevice::WriteOnly)) {
            file->close();
            delete dev;
            return false;
        }
        if (forced)
            static_cast<VCompressionFilter *>(dev)->setOrigFileName(origFileName);
        file->seek(0);
        QByteArray buffer;
        buffer.resize(8 * 1024);
        qint64 len;
        while (!file->atEnd()) {
            len = file->read(buffer.data(), buffer.size());
            dev->write(buffer.data(), len); // TODO error checking
        }
        file->close();
        dev->close();
        delete dev;
    }

    //qDebug() << "Write temporary file to compressed file done.";
    return true;
}

/*** Some help from the tar sources
struct posix_header
{                               byte offset
  char name[100];               *   0 *     0x0
  char mode[8];                 * 100 *     0x64
  char uid[8];                  * 108 *     0x6c
  char gid[8];                  * 116 *     0x74
  char size[12];                * 124 *     0x7c
  char mtime[12];               * 136 *     0x88
  char chksum[8];               * 148 *     0x94
  char typeflag;                * 156 *     0x9c
  char linkname[100];           * 157 *     0x9d
  char magic[6];                * 257 *     0x101
  char version[2];              * 263 *     0x107
  char uname[32];               * 265 *     0x109
  char gname[32];               * 297 *     0x129
  char devmajor[8];             * 329 *     0x149
  char devminor[8];             * 337 *     ...
  char prefix[155];             * 345 *
                                * 500 *
};
*/

void TarArchiveHandlerPrivate::fillBuffer(char *buffer,
                                          const char *mode, qint64 size, time_t mtime, char typeflag,
                                          const char *uname, const char *gname)
{
    // mode (as in stpos())
    Q_ASSERT(strlen(mode) == 6);
    memcpy(buffer + 0x64, mode, 6);
    buffer[ 0x6a ] = ' ';
    buffer[ 0x6b ] = '\0';

    // dummy uid
    strcpy(buffer + 0x6c, "   765 ");
    // dummy gid
    strcpy(buffer + 0x74, "   144 ");

    // size
    QByteArray s = QByteArray::number(size, 8);   // octal
    s = s.rightJustified(11, '0');
    memcpy(buffer + 0x7c, s.data(), 11);
    buffer[ 0x87 ] = ' '; // space-terminate (no null after)

    // modification time
    s = QByteArray::number(static_cast<qulonglong>(mtime), 8);   // octal
    s = s.rightJustified(11, '0');
    memcpy(buffer + 0x88, s.data(), 11);
    buffer[ 0x93 ] = ' '; // space-terminate (no null after) -- well current tar writes a null byte

    // spaces, replaced by the check sum later
    buffer[ 0x94 ] = 0x20;
    buffer[ 0x95 ] = 0x20;
    buffer[ 0x96 ] = 0x20;
    buffer[ 0x97 ] = 0x20;
    buffer[ 0x98 ] = 0x20;
    buffer[ 0x99 ] = 0x20;

    /* From the tar sources :
       Fill in the checksum field.  It's formatted differently from the
       other fields: it has [6] digits, a null, then a space -- rather than
       digits, a space, then a null. */

    buffer[ 0x9a ] = '\0';
    buffer[ 0x9b ] = ' ';

    // type flag (dir, file, link)
    buffer[ 0x9c ] = typeflag;

    // magic + version
    strcpy(buffer + 0x101, "ustar");
    strcpy(buffer + 0x107, "00");

    // user
    strcpy(buffer + 0x109, uname);
    // group
    strcpy(buffer + 0x129, gname);

    // Header check sum
    int check = 32;
    for (uint j = 0; j < 0x200; ++j)
        check += buffer[j];
    s = QByteArray::number(check, 8);   // octal
    s = s.rightJustified(6, '0');
    memcpy(buffer + 0x94, s.constData(), 6);
}

qint64 TarArchiveHandlerPrivate::readRawHeader(char *buffer)
{
    // Read header
    qint64 n = q->device()->read(buffer, 0x200);
    if (n == 0x200 && buffer[0] != 0) {
        // Make sure this is actually a tar header
        if (strncmp(buffer + 257, "ustar", 5)) {
            // The magic isn't there (broken/old tars), but maybe a correct checksum?

            int check = 0;
            for (uint j = 0; j < 0x200; ++j)
                check += buffer[j];

            // adjust checksum to count the checksum fields as blanks
            for (uint j = 0; j < 8 /*size of the checksum field including the \0 and the space*/; j++)
                check -= buffer[148 + j];
            check += 8 * ' ';

            QByteArray s = QByteArray::number(check, 8);   // octal

            // only compare those of the 6 checksum digits that mean something,
            // because the other digits are filled with all sorts of different chars by different tars ...
            // Some tars right-justify the checksum so it could start in one of three places - we have to check each.
            if (strncmp(buffer + 148 + 6 - s.length(), s.data(), s.length())
                    && strncmp(buffer + 148 + 7 - s.length(), s.data(), s.length())
                    && strncmp(buffer + 148 + 8 - s.length(), s.data(), s.length())) {
                qWarning() << "Tar: invalid TAR file. Header is:" << QByteArray(buffer + 257, 5)
                           << "instead of ustar. Reading from wrong pos in file?"
                           << "checksum=" << QByteArray(buffer + 148 + 6 - s.length(), s.length());
                return -1;
            }
        }/*end if*/
    } else {
        // reset to 0 if 0x200 because logical end of archive has been reached
        if (n == 0x200) n = 0;
    }/*end if*/
    return n;
}

bool TarArchiveHandlerPrivate::readLonglink(char *buffer, QByteArray &longlink)
{
    qint64 n = 0;
    //qDebug() << "reading longlink from pos " << device()->pos();
    QIODevice *dev = q->device();
    // read size of longlink from size field in header
    // size is in bytes including the trailing null (which we ignore)
    qint64 size = QByteArray(buffer + 0x7c, 12).trimmed().toLongLong(0, 8 /*octal*/);

    size--;    // ignore trailing null
    longlink.resize(size);
    qint64 offset = 0;
    while (size > 0) {
        int chunksize = qMin(size, 0x200LL);
        n = dev->read(longlink.data() + offset, chunksize);
        if (n == -1) return false;
        size -= chunksize;
        offset += 0x200;
    }/*wend*/
    // jump over the rest
    const int skip = 0x200 - (n % 0x200);
    if (skip < 0x200) {
        if (dev->read(buffer, skip) != skip)
            return false;
    }
    return true;
}

qint64 TarArchiveHandlerPrivate::readHeader(char *buffer, QString &name, QString &symlink)
{
    name.truncate(0);
    symlink.truncate(0);
    while (true) {
        qint64 n = readRawHeader(buffer);
        if (n != 0x200) return n;

        // is it a longlink?
        if (strcmp(buffer, "././@LongLink") == 0) {
            char typeflag = buffer[0x9c];
            QByteArray longlink;
            readLonglink(buffer, longlink);
            switch (typeflag) {
                case 'L':
                    name = QFile::decodeName(longlink);
                    break;
                case 'K':
                    symlink = QFile::decodeName(longlink);
                    break;
            }/*end switch*/
        } else {
            break;
        }/*end if*/
    }/*wend*/

    // if not result of longlink, read names directly from the header
    if (name.isEmpty())
        // there are names that are exactly 100 bytes long
        // and neither longlink nor \0 terminated (bug:101472)
        name = QFile::decodeName(QByteArray(buffer, 100));
    if (symlink.isEmpty())
        symlink = QFile::decodeName(QByteArray(buffer + 0x9d /*?*/, 100));

    return 0x200;
}

void TarArchiveHandlerPrivate::writeLonglink(char *buffer, const QByteArray &name, char typeflag,
                                             const char *uname, const char *gname)
{
    strcpy(buffer, "././@LongLink");
    qint64 namelen = name.length() + 1;
    fillBuffer(buffer, "     0", namelen, 0, typeflag, uname, gname);
    q->device()->write(buffer, 0x200);   // TODO error checking
    qint64 offset = 0;
    while (namelen > 0) {
        int chunksize = qMin(namelen, 0x200LL);
        memcpy(buffer, name.data() + offset, chunksize);
        // write long name
        q->device()->write(buffer, 0x200);   // TODO error checking
        // not even needed to reclear the buffer, tar doesn't do it
        namelen -= chunksize;
        offset += 0x200;
    }/*wend*/
}

/*
 * TarArchiveHandler
 */

TarArchiveHandler::TarArchiveHandler(const QString &mimeType) :
    VArchiveHandler(mimeType),
    d(new TarArchiveHandlerPrivate(this))
{
    d->mimeType = mimeType;
}

// Only called when a filename was given
bool TarArchiveHandler::createDevice(QIODevice::OpenMode mode)
{
    // gzipped tar with possibly invalid file name, ask for the appropriate filter
    if (d->mimeType == QLatin1String("application/x-compressed-tar") ||
            d->mimeType == QLatin1String("application/x-gzip"))
        d->mimeType = QLatin1String("application/x-gzip");
    // bzipped tar with possibly invalid file name, ask for the appropriate filter
    else if (d->mimeType == QLatin1String("application/x-bzip-compressed-tar") ||
             d->mimeType == QLatin1String("application/x-bzip"))
        d->mimeType = QLatin1String("application/x-bzip");
    // lzma compressed tar with possibly invalid file name, ask for the appropriate filter
    else if (d->mimeType == QLatin1String("application/x-lzma-compressed-tar") ||
             d->mimeType == QLatin1String("application/x-lzma"))
        d->mimeType = QLatin1String("application/x-lzma");
    // xz compressed tar with possibly invalid file name, ask for the appropriate filter
    else if (d->mimeType == QLatin1String("application/x-xz-compressed-tar") ||
             d->mimeType == QLatin1String("application/x-xz"))
        d->mimeType = QLatin1String("application/x-xz");

    if (d->mimeType == QLatin1String("application/x-tar")) {
        return VArchiveHandler::createDevice(mode);
    } else if (mode == QIODevice::WriteOnly) {
        if (!VArchiveHandler::createDevice(mode))
            return false;
        if (!d->mimeType.isEmpty()) {
            // Create a compression filter on top of the KSaveFile device that VArchive created.
            //qDebug() << "creating KFilterDev for" << d->mimetype;
            QIODevice *filterDev = VCompressionFilter::device(device(), d->mimeType);
            Q_ASSERT(filterDev);
            setDevice(filterDev);
        }
        return true;
    }

    // The compression filters are very slow with random access.
    // So instead of applying the filter to the device,
    // the file is completely extracted instead,
    // and we work on the extracted tar file.
    // This improves the extraction speed by the tar ioslave dramatically,
    // if the archive file contains many files.
    // This is because the tar ioslave extracts one file after the other and normally
    // has to walk through the decompression filter each time.
    // Which is in fact nearly as slow as a complete decompression for each file.

    Q_ASSERT(!d->tmpFile);
    d->tmpFile = new QTemporaryFile();
    d->tmpFile->setFileTemplate("vtar-XXXXXX.tar");
    d->tmpFile->open();
    //qDebug() << "creating tempfile:" << d->tmpFile->fileName();

    setDevice(d->tmpFile);
    return true;
}

TarArchiveHandler::~TarArchiveHandler()
{
    // mjarrett: Closes to prevent ~VArchive from aborting w/o device
    if (isOpen())
        close();

    delete d->tmpFile;
    delete d;
}

void TarArchiveHandler::setOrigFileName(const QByteArray &fileName)
{
    if (!isOpen() || !(mode() & QIODevice::WriteOnly)) {
        qWarning() << "TarArchiveHandler::setOrigFileName: File must be opened for writing first.\n";
        return;
    }
    d->origFileName = fileName;
}

bool TarArchiveHandler::openArchive(QIODevice::OpenMode mode)
{
    if (!(mode & QIODevice::ReadOnly))
        return true;

    if (!d->fillTempFile(fileName()))
        return false;

    // We'll use the permission and user/group of d->rootDir
    // for any directory we emulate (see findOrCreate)
    //struct stat buf;
    //stat( fileName(), &buf );

    d->dirList.clear();
    QIODevice *dev = device();

    if (!dev)
        return false;

    // read dir information
    char buffer[ 0x200 ];
    bool ende = false;
    do {
        QString name;
        QString symlink;

        // Read header
        qint64 n = d->readHeader(buffer, name, symlink);
        if (n < 0) return false;
        if (n == 0x200) {
            bool isdir = false;

            if (name.endsWith(QLatin1Char('/'))) {
                isdir = true;
                name.truncate(name.length() - 1);
            }

            int pos = name.lastIndexOf(QLatin1Char('/'));
            QString nm = (pos == -1) ? name : name.mid(pos + 1);

            // read access
            buffer[ 0x6b ] = 0;
            char *dummy;
            const char *p = buffer + 0x64;
            while (*p == ' ') ++p;
            int access = (int)strtol(p, &dummy, 8);

            // read user and group
            QString user = QString::fromLocal8Bit(buffer + 0x109);
            QString group = QString::fromLocal8Bit(buffer + 0x129);

            // read time
            buffer[ 0x93 ] = 0;
            p = buffer + 0x88;
            while (*p == ' ') ++p;
            int time = (int)strtol(p, &dummy, 8);

            // read type flag
            char typeflag = buffer[ 0x9c ];
            // '0' for files, '1' hard link, '2' symlink, '5' for directory
            // (and 'L' for longlink fileNames, 'K' for longlink symlink targets)
            // and 'D' for GNU tar extension DUMPDIR
            if (typeflag == '5')
                isdir = true;

            bool isDumpDir = false;
            if (typeflag == 'D') {
                isdir = false;
                isDumpDir = true;
            }

            //qDebug() << "typeflag=" << typeflag << " islink=" << ( typeflag == '1' || typeflag == '2' );

            if (isdir)
                access |= S_IFDIR; // f*cking broken tar files

            VArchiveEntry *e;
            if (isdir) {
                //qDebug() << "directory" << nm;
                e = new VArchiveDirectory(archive(), nm, access, time, user, group, symlink);
            } else {
                // read size
                QByteArray sizeBuffer(buffer + 0x7c, 12);
                qint64 size = sizeBuffer.trimmed().toLongLong(0, 8 /*octal*/);
                //qDebug() << "sizeBuffer='" << sizeBuffer << "' -> size=" << size;

                // for isDumpDir we will skip the additional info about that dirs contents
                if (isDumpDir) {
                    //qDebug() << nm << "isDumpDir";
                    e = new VArchiveDirectory(archive(), nm, access, time, user, group, symlink);
                } else {

                    // Let's hack around hard links. Our classes don't support that, so make them symlinks
                    if (typeflag == '1') {
                        qDebug() << "Hard link, setting size to 0 instead of" << size;
                        size = 0; // no contents
                    }

                    //qDebug() << "file" << nm << "size=" << size;
                    e = new VArchiveFile(archive(), nm, access, time, user, group, symlink,
                                         dev->pos(), size);
                }

                // Skip contents + align bytes
                qint64 rest = size % 0x200;
                qint64 skip = size + (rest ? 0x200 - rest : 0);
                //qDebug() << "pos()=" << dev->pos() << "rest=" << rest << "skipping" << skip;
                if (! dev->seek(dev->pos() + skip))
                    qWarning() << "skipping" << skip << "failed";
            }

            if (pos == -1) {
                if (nm == QLatin1String(".")) { // special case
                    Q_ASSERT(isdir);
                    if (isdir)
                        setRootDir(static_cast<VArchiveDirectory *>(e));
                } else
                    rootDir()->addEntry(e);
            } else {
                // In some tar files we can find dir/./file => call cleanPath
                QString path = QDir::cleanPath(name.left(pos));
                // Ensure container directory exists, create otherwise
                VArchiveDirectory *d = findOrCreate(path);
                d->addEntry(e);
            }
        } else {
            //qDebug("Terminating. Read %d bytes, first one is %d", n, buffer[0]);
            d->tarEnd = dev->pos() - n; // Remember end of archive
            ende = true;
        }
    } while (!ende);
    return true;
}

bool TarArchiveHandler::closeArchive()
{
    d->dirList.clear();

    bool ok = true;

    // If we are in readwrite mode and had created
    // a temporary tar file, we have to write
    // back the changes to the original file
    if (d->tmpFile && (mode() & QIODevice::WriteOnly)) {
        ok = d->writeBackTempFile(fileName());
        delete d->tmpFile;
        d->tmpFile = 0;
        setDevice(0);
    }

    return ok;
}

bool TarArchiveHandler::doFinishWriting(qint64 size)
{
    // Write alignment
    int rest = size % 0x200;
    if ((mode() & QIODevice::ReadWrite) == QIODevice::ReadWrite)
        d->tarEnd = device()->pos() + (rest ? 0x200 - rest : 0); // Record our new end of archive
    if (rest) {
        char buffer[ 0x201 ];
        for (uint i = 0; i < 0x200; ++i)
            buffer[i] = 0;
        qint64 nwritten = device()->write(buffer, 0x200 - rest);
        return nwritten == 0x200 - rest;
    }
    return true;
}

bool TarArchiveHandler::doPrepareWriting(const QString &name, const QString &user,
                                         const QString &group, qint64 size, mode_t perm,
                                         time_t /*atime*/, time_t mtime, time_t /*ctime*/)
{
    if (!isOpen()) {
        qWarning() << "You must open the tar file before writing to it\n";
        return false;
    }

    if (!(mode() & QIODevice::WriteOnly)) {
        qWarning() << "You must open the tar file for writing\n";
        return false;
    }

    // In some tar files we can find dir/./file => call cleanPath
    QString fileName(QDir::cleanPath(name));

    /*
      // Create toplevel dirs
      // Commented out by David since it's not necessary, and if anybody thinks it is,
      // he needs to implement a findOrCreate equivalent in writeDir.
      // But as Tar and the "tar" program both handle tar files without
      // dir entries, there's really no need for that
      QString tmp ( fileName );
      int i = tmp.lastIndexOf( '/' );
      if ( i != -1 )
      {
      QString d = tmp.left( i + 1 ); // contains trailing slash
      if ( !m_dirList.contains( d ) )
      {
      tmp = tmp.mid( i + 1 );
      writeDir( d, user, group ); // WARNING : this one doesn't create its toplevel dirs
      }
      }
    */

    char buffer[ 0x201 ];
    memset(buffer, 0, 0x200);
    if ((mode() & QIODevice::ReadWrite) == QIODevice::ReadWrite)
        device()->seek(d->tarEnd); // Go to end of archive as might have moved with a read

    // provide converted stuff we need later on
    const QByteArray encodedFileName = QFile::encodeName(fileName);
    const QByteArray uname = user.toLocal8Bit();
    const QByteArray gname = group.toLocal8Bit();

    // If more than 100 chars, we need to use the LongLink trick
    if (fileName.length() > 99)
        d->writeLonglink(buffer, encodedFileName, 'L', uname, gname);

    // Write (potentially truncated) name
    strncpy(buffer, encodedFileName, 99);
    buffer[99] = 0;
    // zero out the rest (except for what gets filled anyways)
    memset(buffer + 0x9d, 0, 0x200 - 0x9d);

    QByteArray permstr = QByteArray::number((unsigned int)perm, 8);
    permstr = permstr.rightJustified(6, '0');
    d->fillBuffer(buffer, permstr, size, mtime, 0x30, uname, gname);

    // Write header
    return device()->write(buffer, 0x200) == 0x200;
}

bool TarArchiveHandler::doWriteDir(const QString &name, const QString &user,
                                   const QString &group, mode_t perm,
                                   time_t /*atime*/, time_t mtime, time_t /*ctime*/)
{
    if (!isOpen()) {
        qWarning() << "You must open the tar file before writing to it\n";
        return false;
    }

    if (!(mode() & QIODevice::WriteOnly)) {
        qWarning() << "You must open the tar file for writing\n";
        return false;
    }

    // In some tar files we can find dir/./ => call cleanPath
    QString dirName(QDir::cleanPath(name));

    // Need trailing '/'
    if (!dirName.endsWith(QLatin1Char('/')))
        dirName += QLatin1Char('/');

    if (d->dirList.contains(dirName))
        return true; // already there

    char buffer[ 0x201 ];
    memset(buffer, 0, 0x200);
    if ((mode() & QIODevice::ReadWrite) == QIODevice::ReadWrite)
        device()->seek(d->tarEnd); // Go to end of archive as might have moved with a read

    // provide converted stuff we need lateron
    QByteArray encodedDirname = QFile::encodeName(dirName);
    QByteArray uname = user.toLocal8Bit();
    QByteArray gname = group.toLocal8Bit();

    // If more than 100 chars, we need to use the LongLink trick
    if (dirName.length() > 99)
        d->writeLonglink(buffer, encodedDirname, 'L', uname, gname);

    // Write (potentially truncated) name
    strncpy(buffer, encodedDirname, 99);
    buffer[99] = 0;
    // zero out the rest (except for what gets filled anyways)
    memset(buffer + 0x9d, 0, 0x200 - 0x9d);

    QByteArray permstr = QByteArray::number((unsigned int)perm, 8);
    permstr = permstr.rightJustified(6, ' ');
    d->fillBuffer(buffer, permstr, 0, mtime, 0x35, uname, gname);

    // Write header
    device()->write(buffer, 0x200);
    if ((mode() & QIODevice::ReadWrite) == QIODevice::ReadWrite)
        d->tarEnd = device()->pos();

    d->dirList.append(dirName);   // contains trailing slash
    return true; // TODO if wanted, better error control
}

bool TarArchiveHandler::doWriteSymLink(const QString &name, const QString &target,
                                       const QString &user, const QString &group,
                                       mode_t perm, time_t /*atime*/, time_t mtime, time_t /*ctime*/)
{
    if (!isOpen()) {
        qWarning() << "You must open the tar file before writing to it\n";
        return false;
    }

    if (!(mode() & QIODevice::WriteOnly)) {
        qWarning() << "You must open the tar file for writing\n";
        return false;
    }

    // In some tar files we can find dir/./file => call cleanPath
    QString fileName(QDir::cleanPath(name));

    char buffer[ 0x201 ];
    memset(buffer, 0, 0x200);
    if ((mode() & QIODevice::ReadWrite) == QIODevice::ReadWrite)
        device()->seek(d->tarEnd); // Go to end of archive as might have moved with a read

    // provide converted stuff we need lateron
    QByteArray encodedFileName = QFile::encodeName(fileName);
    QByteArray encodedTarget = QFile::encodeName(target);
    QByteArray uname = user.toLocal8Bit();
    QByteArray gname = group.toLocal8Bit();

    // If more than 100 chars, we need to use the LongLink trick
    if (target.length() > 99)
        d->writeLonglink(buffer, encodedTarget, 'K', uname, gname);
    if (fileName.length() > 99)
        d->writeLonglink(buffer, encodedFileName, 'L', uname, gname);

    // Write (potentially truncated) name
    strncpy(buffer, encodedFileName, 99);
    buffer[99] = 0;
    // Write (potentially truncated) symlink target
    strncpy(buffer + 0x9d, encodedTarget, 99);
    buffer[0x9d + 99] = 0;
    // zero out the rest
    memset(buffer + 0x9d + 100, 0, 0x200 - 100 - 0x9d);

    QByteArray permstr = QByteArray::number((unsigned int)perm, 8);
    permstr = permstr.rightJustified(6, ' ');
    d->fillBuffer(buffer, permstr, 0, mtime, 0x32, uname, gname);

    // Write header
    bool retval = device()->write(buffer, 0x200) == 0x200;
    if ((mode() & QIODevice::ReadWrite) == QIODevice::ReadWrite)
        d->tarEnd = device()->pos();
    return retval;
}

#include "tararchivehandler.moc"
