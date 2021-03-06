/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2000-2005 David Faure
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

#ifndef VABSTRACTCOMPRESSIONFILTER_H
#define VABSTRACTCOMPRESSIONFILTER_H

#include <QObject>
#include <QString>

#include <VibeCore/VibeCoreExport>

class QIODevice;

class VAbstractCompressionFilterPrivate;

class VIBECORE_EXPORT VAbstractCompressionFilter
{
public:
    enum Result {
        Ok,
        End,
        Error
    };

    enum FilterFlags {
        NoHeaders = 0,
        WithHeaders = 1
    };

    VAbstractCompressionFilter();
    virtual ~VAbstractCompressionFilter();

    QIODevice *device();
    void setDevice(QIODevice *dev, bool autodelete = false);

    virtual void init(int mode) = 0; // TODO: return a bool

    virtual int mode() const = 0;

    virtual void terminate();

    virtual void reset();

    virtual bool readHeader() = 0;

    virtual bool writeHeader(const QByteArray &filename) = 0;

    virtual void setOutBuffer(char *data, uint maxlen) = 0;

    virtual void setInBuffer(const char *data, uint size) = 0;

    virtual bool inBufferEmpty() const;

    virtual int  inBufferAvailable() const = 0;

    virtual bool outBufferFull() const;

    virtual int  outBufferAvailable() const = 0;

    virtual Result compress(bool finish) = 0;
    virtual Result uncompress() = 0;

    void setFilterFlags(FilterFlags flags);
    FilterFlags filterFlags() const;

    static VAbstractCompressionFilter *findFilterByFileName(const QString &fileName);
    static VAbstractCompressionFilter *findFilterByMimeType(const QString &mimeType);

protected: // TODO: move to d pointer
    QIODevice *m_dev;
    bool m_bAutoDel;

private:
    Q_DISABLE_COPY(VAbstractCompressionFilter)

    VAbstractCompressionFilterPrivate *const d;
};

#endif // VABSTRACTCOMPRESSIONFILTER_H
