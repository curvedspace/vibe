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
 * $BEGIN_LICENSE:LGPL-ONLY$
 *
 * This file may be used under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation and
 * appearing in the file LICENSE.LGPL included in the packaging of
 * this file, either version 2.1 of the License, or (at your option) any
 * later version.  Please review the following information to ensure the
 * GNU Lesser General Public License version 2.1 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 * If you have questions regarding the use of this file, please contact
 * us via http://www.maui-project.org/.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include <config-compression.h>

#include "vbzip2compressionfilter.h"

#if HAVE_BZIP2_SUPPORT

// we don't need that
#define BZ_NO_STDIO
extern "C" {
#include <bzlib.h>
}

#if NEED_BZ2_PREFIX
#  define bzDecompressInit(x,y,z) BZ2_bzDecompressInit(x,y,z)
#  define bzDecompressEnd(x) BZ2_bzDecompressEnd(x)
#  define bzCompressEnd(x)  BZ2_bzCompressEnd(x)
#  define bzDecompress(x) BZ2_bzDecompress(x)
#  define bzCompress(x,y) BZ2_bzCompress(x, y)
#  define bzCompressInit(x,y,z,a) BZ2_bzCompressInit(x, y, z, a);
#endif

#include <QDebug>
#include <QIODevice>

// For documentation on this, see /usr/doc/bzip2-0.9.5d/bzip2-0.9.5d/manual_3.html

class VBzip2CompressionFilter::Private
{
public:
    Private()
        : isInitialized(false) {
        memset(&zStream, 0, sizeof(zStream));
        mode = 0;
    }

    bz_stream zStream;
    int mode;
    bool isInitialized;
};

VBzip2CompressionFilter::VBzip2CompressionFilter()
    : d(new Private)
{
}

VBzip2CompressionFilter::~VBzip2CompressionFilter()
{
    delete d;
}

void VBzip2CompressionFilter::init(int mode)
{
    if (d->isInitialized) {
        terminate();
    }

    d->zStream.next_in = 0;
    d->zStream.avail_in = 0;
    if (mode == QIODevice::ReadOnly) {
        (void)bzDecompressInit(&d->zStream, 0, 0);
        //qDebug() << "bzDecompressInit returned " << result;
        // TODO: test result and return false on error
    } else if (mode == QIODevice::WriteOnly) {
        (void)bzCompressInit(&d->zStream, 5, 0, 0);
        //qDebug() << "bzDecompressInit returned " << result;
        // TODO: test result and return false on error
    } else {
        qWarning() << "Unsupported mode " << mode << ". Only QIODevice::ReadOnly and QIODevice::WriteOnly supported";
        // TODO return false
    }
    d->mode = mode;
    d->isInitialized = true;
}

int VBzip2CompressionFilter::mode() const
{
    return d->mode;
}

void VBzip2CompressionFilter::terminate()
{
    if (d->mode == QIODevice::ReadOnly) {
        int result = bzDecompressEnd(&d->zStream);
        // TODO: test result and return false on error
        //qDebug() << "bzDecompressEnd returned " << result;
    } else if (d->mode == QIODevice::WriteOnly) {
        int result = bzCompressEnd(&d->zStream);
        // TODO: test result and return false on error
        //qDebug() << "bzCompressEnd returned " << result;
    } else {
        qWarning() << "Unsupported mode " << d->mode << ". Only QIODevice::ReadOnly and QIODevice::WriteOnly supported";
        // TODO return false
    }
    d->isInitialized = false;
}


void VBzip2CompressionFilter::reset()
{
    // bzip2 doesn't seem to have a reset call...
    terminate();
    init(d->mode);
}

void VBzip2CompressionFilter::setOutBuffer(char *data, uint maxlen)
{
    d->zStream.avail_out = maxlen;
    d->zStream.next_out = data;
}

void VBzip2CompressionFilter::setInBuffer(const char *data, unsigned int size)
{
    d->zStream.avail_in = size;
    d->zStream.next_in = const_cast<char *>(data);
}

int VBzip2CompressionFilter::inBufferAvailable() const
{
    return d->zStream.avail_in;
}

int VBzip2CompressionFilter::outBufferAvailable() const
{
    return d->zStream.avail_out;
}

VBzip2CompressionFilter::Result VBzip2CompressionFilter::uncompress()
{
    //qDebug() << "Calling bzDecompress with avail_in=" << inBufferAvailable() << " avail_out=" << outBufferAvailable();
    int result = bzDecompress(&d->zStream);
    if (result != BZ_OK) {
        qDebug() << "bzDecompress returned" << result;
        qDebug() << "VBzip2CompressionFilter::uncompress" << (result == BZ_STREAM_END ? VAbstractCompressionFilter::End : VAbstractCompressionFilter::Error);
    }

    switch (result) {
        case BZ_OK:
            return VAbstractCompressionFilter::Ok;
        case BZ_STREAM_END:
            return VAbstractCompressionFilter::End;
        default:
            return VAbstractCompressionFilter::Error;
    }
}

VBzip2CompressionFilter::Result VBzip2CompressionFilter::compress(bool finish)
{
    //qDebug() << "Calling bzCompress with avail_in=" << inBufferAvailable() << " avail_out=" << outBufferAvailable();
    int result = bzCompress(&d->zStream, finish ? BZ_FINISH : BZ_RUN);

    switch (result) {
        case BZ_OK:
        case BZ_FLUSH_OK:
        case BZ_RUN_OK:
        case BZ_FINISH_OK:
            return VAbstractCompressionFilter::Ok;
            break;
        case BZ_STREAM_END:
            //qDebug() << "  bzCompress returned " << result;
            return VAbstractCompressionFilter::End;
            break;
        default:
            //qDebug() << "  bzCompress returned " << result;
            return VAbstractCompressionFilter::Error;
            break;
    }
}

#endif  /* HAVE_BZIP2_SUPPORT */
