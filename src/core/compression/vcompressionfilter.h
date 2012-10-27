/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2000-2006 David Faure
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    David Faure <faure@kde.org>
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

#ifndef VCOMPRESSIONFILTER_H
#define VCOMPRESSIONFILTER_H

#include <QIODevice>
#include <QString>

#include <VibeCore/VibeCoreExport>

class QFile;

class VAbstractCompressionFilter;
class VCompressionFilterPrivate;

class VIBECORE_EXPORT VCompressionFilter : public QIODevice
{
public:
    virtual ~VCompressionFilter();

    virtual bool open(QIODevice::OpenMode mode);
    virtual void close();

    void setOrigFileName(const QByteArray &fileName);

    void setSkipHeaders();

    virtual bool seek(qint64);

    virtual bool atEnd() const;

    /// Reimplemented to return true. VCompressionFilter is a sequential QIODevice.
    /// Well, not really, since it supports seeking and KZip uses that.
    //virtual bool isSequential() const { return true; }

public:
    // TODO: turn those static methods into constructors
    static QIODevice *deviceForFile(const QString &fileName, const QString &mimetype = QString(),
                                    bool forceFilter = false);
    static QIODevice *device(QIODevice *inDevice, const QString &mimetype, bool autoDeleteInDevice = true);

protected:
    virtual qint64 readData(char *data, qint64 maxlen);
    virtual qint64 writeData(const char *data, qint64 len);

private:
    explicit VCompressionFilter(VAbstractCompressionFilter *filter, bool autoDeleteFilterBase = false);

private:
    VCompressionFilterPrivate *const d;
};

#endif // VCOMPRESSIONFILTER_H
