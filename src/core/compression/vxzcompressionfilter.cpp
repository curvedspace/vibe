/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2007-2008 Per Øyvind Karlsen
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Per Øyvind Karlsen <peroyvind@mandriva.org
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

#include <config-compression.h>

#include "vxzcompressionfilter.h"

#if HAVE_XZ_SUPPORT
extern "C" {
#include <lzma.h>
}

#include <QDebug>
#include <QIODevice>

class VXzCompressionFilter::Private
{
public:
    Private()
        : isInitialized(false) {
        memset(&zStream, 0, sizeof(zStream));
        mode = 0;
    }

    lzma_stream zStream;
    int mode;
    bool isInitialized;
};

VXzCompressionFilter::VXzCompressionFilter()
    : d(new Private)
{
}

VXzCompressionFilter::~VXzCompressionFilter()
{
    delete d;
}

void VXzCompressionFilter::init(int mode)
{
    if (d->isInitialized) {
        terminate();
    }

    lzma_ret result;
    d->zStream.next_in = 0;
    d->zStream.avail_in = 0;
    if (mode == QIODevice::ReadOnly) {
        /* We set the memlimit for decompression to 100MiB which should be
         * more than enough to be sufficient for level 9 which requires 65 MiB.
         */
        result = lzma_auto_decoder(&d->zStream, 100 << 20, 0);
        //kDebug(7131) << "lzma_auto_decoder returned " << result;
    } else if (mode == QIODevice::WriteOnly) {
        result = lzma_easy_encoder(&d->zStream, LZMA_PRESET_DEFAULT, LZMA_CHECK_CRC32);
        //kDebug(7131) << "lzma_easy_encoder returned " << result;
    } else
        qWarning() << "Unsupported mode " << mode << ". Only QIODevice::ReadOnly and QIODevice::WriteOnly supported";
    d->mode = mode;
    d->isInitialized = true;
}

int VXzCompressionFilter::mode() const
{
    return d->mode;
}

void VXzCompressionFilter::terminate()
{
    if (d->mode == QIODevice::ReadOnly || d->mode == QIODevice::WriteOnly) {
        lzma_end(&d->zStream);
    } else {
        qWarning() << "Unsupported mode " << d->mode << ". Only QIODevice::ReadOnly and QIODevice::WriteOnly supported";
    }
    d->isInitialized = false;
}


void VXzCompressionFilter::reset()
{
    //kDebug(7131) << "VXzCompressionFilter::reset";
    // liblzma doesn't have a reset call...
    terminate();
    init(d->mode);
}

void VXzCompressionFilter::setOutBuffer(char *data, uint maxlen)
{
    d->zStream.avail_out = maxlen;
    d->zStream.next_out = (uint8_t *)data;
}

void VXzCompressionFilter::setInBuffer(const char *data, unsigned int size)
{
    d->zStream.avail_in = size;
    d->zStream.next_in = (uint8_t *)const_cast<char *>(data);
}

int VXzCompressionFilter::inBufferAvailable() const
{
    return d->zStream.avail_in;
}

int VXzCompressionFilter::outBufferAvailable() const
{
    return d->zStream.avail_out;
}

VXzCompressionFilter::Result VXzCompressionFilter::uncompress()
{
    //kDebug(7131) << "Calling lzma_code with avail_in=" << inBufferAvailable() << " avail_out =" << outBufferAvailable();
    lzma_ret result = lzma_code(&d->zStream, LZMA_RUN);
    if (result != LZMA_OK) {
        qDebug() << "lzma_code returned " << result;
        qDebug() << "VXzCompressionFilter::uncompress " << (result == LZMA_STREAM_END ? VAbstractCompressionFilter::End : VAbstractCompressionFilter::Error);
    }

    switch (result) {
        case LZMA_OK:
            return VAbstractCompressionFilter::Ok;
        case LZMA_STREAM_END:
            return VAbstractCompressionFilter::End;
        default:
            return VAbstractCompressionFilter::Error;
    }
}

VXzCompressionFilter::Result VXzCompressionFilter::compress(bool finish)
{
    //kDebug(7131) << "Calling lzma_code with avail_in=" << inBufferAvailable() << " avail_out=" << outBufferAvailable();
    lzma_ret result = lzma_code(&d->zStream, finish ? LZMA_FINISH : LZMA_RUN);

    switch (result) {
        case LZMA_OK:
            return VAbstractCompressionFilter::Ok;
            break;
        case LZMA_STREAM_END:
            qDebug() << "  lzma_code returned " << result;
            return VAbstractCompressionFilter::End;
            break;
        default:
            qDebug() << "  lzma_code returned " << result;
            return VAbstractCompressionFilter::Error;
            break;
    }
}

#endif  /* HAVE_XZ_SUPPORT */
