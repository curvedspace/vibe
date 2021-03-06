/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2000-2009 David Faure
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

#ifndef VGZIPCOMPRESSIONFILTER_H
#define VGZIPCOMPRESSIONFILTER_H

#include "vcompressionfilter.h"
#include "vabstractcompressionfilter.h"

/**
 * Internal class used by VCompressionFilter
 *
 * This header is not installed.
 *
 * \internal
 */
class VIBECORE_EXPORT VGzipCompressionFilter : public VAbstractCompressionFilter
{
public:
    VGzipCompressionFilter();
    virtual ~VGzipCompressionFilter();

    virtual void init(int mode);

    // The top of zlib.h explains it: there are three cases.
    // - Raw deflate, no header (e.g. inside a ZIP file)
    // - Thin zlib header (1) (which is normally what HTTP calls "deflate" (2))
    // - Gzip header, implemented here by readHeader
    //
    // (1) as written out by compress()/compress2()
    // (2) see http://www.zlib.net/zlib_faq.html#faq39
    enum Flag {
        RawDeflate = 0, // raw deflate data
        ZlibHeader = 1, // zlib headers (HTTP deflate)
        GZipHeader = 2
    };

    void init(int mode, Flag flag); // for direct users of VGzipCompressionFilter
    virtual int mode() const;
    virtual void terminate();
    virtual void reset();
    virtual bool readHeader(); // this is about the GZIP header
    virtual bool writeHeader(const QByteArray &fileName);
    void writeFooter();
    virtual void setOutBuffer(char *data, uint maxlen);
    virtual void setInBuffer(const char *data, uint size);
    virtual int  inBufferAvailable() const;
    virtual int  outBufferAvailable() const;
    virtual Result uncompress();
    virtual Result compress(bool finish);

private:
    Result uncompress_noop();

    class Private;
    Private *const d;
};

#endif // VGZIPCOMPRESSIONFILTER_H
