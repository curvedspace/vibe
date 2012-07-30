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

#ifndef VBZIP2COMPRESSIONFILTER_H
#define VBZIP2COMPRESSIONFILTER_H

#include <config-compression.h>

#if HAVE_BZIP2_SUPPORT

#include "vabstractcompressionfilter.h"

/**
 * Internal class used by VCompressionFilter
 * @internal
 */
class VBzip2CompressionFilter : public VAbstractCompressionFilter
{
public:
    VBzip2CompressionFilter();
    virtual ~VBzip2CompressionFilter();

    virtual void init(int);
    virtual int mode() const;
    virtual void terminate();
    virtual void reset();
    virtual bool readHeader() {
        return true;    // bzip2 handles it by itself ! Cool !
    }
    virtual bool writeHeader(const QByteArray &) {
        return true;
    }
    virtual void setOutBuffer(char *data, uint maxlen);
    virtual void setInBuffer(const char *data, uint size);
    virtual int  inBufferAvailable() const;
    virtual int  outBufferAvailable() const;
    virtual Result uncompress();
    virtual Result compress(bool finish);
private:
    class Private;
    Private *const d;
};

#endif

#endif // VBZIP2COMPRESSIONFILTER_H
