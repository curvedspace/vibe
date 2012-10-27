/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2002 Laurence Anderson
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Laurence Anderson <l.d.anderson@warwick.ac.uk>
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#include <QDebug>

#include <VCompressionFilter>
#include <VArchive>
#include <VArchiveHandlerPlugin>

#include "ararchivehandler.h"

/*
 * ArPlugin
 */

class ArPlugin : public VArchiveHandlerPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.hawaii.Vibe.VArchiveHandlerFactoryInterface" FILE "ar.json")
public:
    QStringList mimeTypes() const {
        QStringList types;
        types << "application/x-archive";

        return types;
    }

    VArchiveHandler *create(const QString &mimeType) {
        if (mimeTypes().contains(mimeType))
            return new ArArchiveHandler(mimeType);
        return 0;
    }
};

/*
 * ArArchiveHandler
 */

ArArchiveHandler::ArArchiveHandler(const QString &filename) :
    VArchiveHandler(filename)
{
}

ArArchiveHandler::~ArArchiveHandler()
{
    if (isOpen())
        close();
}

bool ArArchiveHandler::doPrepareWriting(const QString &, const QString &, const QString &,
                                        qint64, mode_t, time_t, time_t, time_t)
{
    return false;
}

bool ArArchiveHandler::doFinishWriting(qint64)
{
    return false;
}

bool ArArchiveHandler::doWriteDir(const QString &, const QString &, const QString &,
                                  mode_t, time_t, time_t, time_t)
{
    return false;
}

bool ArArchiveHandler::doWriteSymLink(const QString &, const QString &, const QString &,
                                      const QString &, mode_t, time_t, time_t, time_t)
{
    return false;
}

bool ArArchiveHandler::openArchive(QIODevice::OpenMode mode)
{
    if (mode == QIODevice::WriteOnly)
        return true;
    if (mode != QIODevice::ReadOnly && mode != QIODevice::ReadWrite) {
        qWarning() << "Unsupported mode " << mode;
        return false;
    }

    QIODevice *dev = device();
    if (!dev)
        return false;

    QByteArray magic = dev->read(7);
    if (magic != "!<arch>") {
        qWarning() << "Invalid main magic";
        return false;
    }

    char *ar_longnames = 0;
    while (! dev->atEnd()) {
        QByteArray ar_header;
        ar_header.resize(61);
        QByteArray name;
        int date, uid, gid, mode;
        qint64 size;

        // Ar headers are padded to byte boundary
        dev->seek(dev->pos() + (2 - (dev->pos() % 2)) % 2);

        // Read ar header
        if (dev->read(ar_header.data(), 60) != 60) {
            qWarning() << "Couldn't read header";
            delete[] ar_longnames;
            //return false;
            // Probably EOF / trailing junk
            return true;
        }

        // Check header magic
        if (!ar_header.endsWith("`\n")) {
            qWarning() << "Invalid magic";
            delete[] ar_longnames;
            return false;
        }

        // Process header
        name = ar_header.mid(0, 16);
        date = ar_header.mid(16, 12).toInt();
        uid = ar_header.mid(28, 6).toInt();
        gid = ar_header.mid(34, 6).toInt();
        mode = ar_header.mid(40, 8).toInt();
        size = ar_header.mid(48, 10).toInt();

        // Deal with special entries
        bool skip_entry = false;
        if (name.mid(0, 1) == "/") {
            if (name.mid(1, 1) == "/") {
                // Longfilename table entry
                delete[] ar_longnames;
                ar_longnames = new char[size + 1];
                ar_longnames[size] = '\0';
                dev->read(ar_longnames, size);
                skip_entry = true;
                qDebug() << "Read in longnames entry";
            } else if (name.mid(1, 1) == " ") {
                // Symbol table entry
                qDebug() << "Skipped symbol entry";
                dev->seek(dev->pos() + size);
                skip_entry = true;
            } else { // Longfilename
                qDebug() << "Longfilename #" << name.mid(1, 15).toInt();
                if (! ar_longnames) {
                    qWarning() << "Invalid longfilename reference";
                    delete[] ar_longnames;
                    return false;
                }
                name = &ar_longnames[name.mid(1, 15).toInt()];
                name = name.left(name.indexOf("/"));
            }
        }
        if (skip_entry)
            continue;

        // Process filename
        name = name.trimmed();
        name.replace('/', QByteArray());
        qDebug() << "Filename: " << name << " Size: " << size;

        VArchiveEntry *entry = new VArchiveFile(archive(), QString::fromLocal8Bit(name),
                                                mode, date, QString(), QString(),
                                                QString(), dev->pos(), size);
        // Ar files don't support directories, so everything in root
        rootDir()->addEntry(entry);

        dev->seek(dev->pos() + size);   // Skip contents
    }
    delete[] ar_longnames;

    return true;
}

bool ArArchiveHandler::closeArchive()
{
    return true;
}

#include "ararchivehandler.moc"
