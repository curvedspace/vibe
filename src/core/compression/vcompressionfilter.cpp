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

#include <QDebug>
#include <QFile>

#include "vabstractcompressionfilter.h"
#include "vcompressionfilter.h"
#include "vcompressionfilter_p.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define BUFFER_SIZE 8*1024

/*
 * VCompressionFilterPrivate
 */

VCompressionFilterPrivate::VCompressionFilterPrivate() :
    bNeedHeader(true),
    bSkipHeaders(false),
    autoDeleteFilterBase(false),
    bOpenedUnderlyingDevice(false),
    bIgnoreData(false)
{
}

/*!
    \class VCompressionFilter
    \brief A class for reading and writing compressed data.
    \ingroup core Core Kit

    A class for reading and writing compressed data onto a device
    (e.g. file, but other usages are possible, like a buffer or a socket).

    To simply read/write compressed files, see deviceForFile.

    \author David Faure <faure@kde.org>
    \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
*/

/*!
    Constructs a VCompressionFilter for a given filter (e.g. gzip, bzip2 etc.).
    \param filter the VAbstractCompressionFilter to use
    \param autoDeleteFilterBase when true this object will become the
    owner of @p filter.
*/
VCompressionFilter::VCompressionFilter(VAbstractCompressionFilter *_filter, bool autoDeleteFilterBase)
    : d(new VCompressionFilterPrivate)
{
    assert(_filter);
    d->filter = _filter;
    d->autoDeleteFilterBase = autoDeleteFilterBase;
}

/*!
    Destructs the VCompressionFilter.
    Calls close() if the filter device is still open.
*/
VCompressionFilter::~VCompressionFilter()
{
    if (isOpen())
        close();
    if (d->autoDeleteFilterBase)
        delete d->filter;
    delete d;
}

/*!
    Creates an i/o device that is able to read from @p fileName,
    whether it's compressed or not. Available compression filters
    (gzip/bzip2 etc.) will automatically be used.

    The compression filter to be used is determined from the @p fileName
    if @p mimetype is empty. Pass "application/x-gzip" or "application/x-bzip"
    to force the corresponding decompression filter, if available.

    Warning: application/x-bzip may not be available.
    In that case a QFile opened on the compressed data will be returned !
    Use VAbstractCompressionFilter::findFilterByMimeType and code similar to what
    deviceForFile is doing, to better control what's happening.

    The returned QIODevice has to be deleted after using.

    \param fileName the name of the file to filter
    \param mimetype the mime type of the file to filter, or QString() if unknown
    \param forceFilter if true, the function will either find a compression filter, or return 0.
           If false, it will always return a QIODevice. If no
           filter is available it will return a simple QFile.
           This can be useful if the file is usable without a filter.
    \return if a filter has been found, the QIODevice for the filter. If the
            filter does not exist, the return value depends on @p forceFilter.
            The returned QIODevice has to be deleted after using.
*/
QIODevice *VCompressionFilter::deviceForFile(const QString &fileName, const QString &mimetype,
                                             bool forceFilter)
{
    QFile *f = new QFile(fileName);
    VAbstractCompressionFilter *base = mimetype.isEmpty() ? VAbstractCompressionFilter::findFilterByFileName(fileName)
                                       : VAbstractCompressionFilter::findFilterByMimeType(mimetype);
    if (base) {
        base->setDevice(f, true);
        return new VCompressionFilter(base, true);
    }
    if (!forceFilter)
        return f;
    else {
        delete f;
        return 0L;
    }
}

/*!
    Creates an i/o device that is able to read from the QIODevice @p inDevice,
    whether the data is compressed or not. Available compression filters
    (gzip/bzip2 etc.) will automatically be used.

    The compression filter to be used is determined @p mimetype .
    Pass "application/x-gzip" or "application/x-bzip"
    to use the corresponding decompression filter.

    Warning: application/x-bzip may not be available.
    In that case 0 will be returned !

    The returned QIODevice has to be deleted after using.
    \param inDevice input device. Won't be deleted if @p autoDeleteInDevice = false
    \param mimetype the mime type for the filter
    \param autoDeleteInDevice if true, @p inDevice will be deleted automatically
    \return a VCompressionFilter that filters the original stream. Must be deleted after using
*/
QIODevice *VCompressionFilter::device(QIODevice *inDevice, const QString &mimetype, bool autoDeleteInDevice)
{
    if (inDevice == 0)
        return 0;
    VAbstractCompressionFilter *base = VAbstractCompressionFilter::findFilterByMimeType(mimetype);
    if (base) {
        base->setDevice(inDevice, autoDeleteInDevice);
        return new VCompressionFilter(base, true /* auto-delete "base" */);
    }
    return 0;
}

/*!
    Open for reading or writing.
    If the VAbstractCompressionFilter's device is not opened, it will be opened.
*/
bool VCompressionFilter::open(QIODevice::OpenMode mode)
{
    if (isOpen()) {
        qWarning() << "VCompressionFilter::open: device is already open";
        return true; // QFile returns false, but well, the device -is- open...
    }

    if (mode == QIODevice::ReadOnly) {
        d->buffer.resize(0);
    } else {
        d->buffer.resize(BUFFER_SIZE);
        d->filter->setOutBuffer(d->buffer.data(), d->buffer.size());
    }
    d->bNeedHeader = !d->bSkipHeaders;
    d->filter->setFilterFlags(d->bSkipHeaders ? VAbstractCompressionFilter::NoHeaders : VAbstractCompressionFilter::WithHeaders);
    d->filter->init(mode);
    d->bOpenedUnderlyingDevice = !d->filter->device()->isOpen();
    bool ret = d->bOpenedUnderlyingDevice ? d->filter->device()->open(mode) : true;
    d->result = VAbstractCompressionFilter::Ok;

    if (!ret)
        qWarning() << "VCompressionFilter::open: Couldn't open underlying device";
    else
        setOpenMode(mode);

    return ret;
}

/*!
    Close after reading or writing.
    If the VAbstractCompressionFilter's device was opened by open(), it will be closed.
*/
void VCompressionFilter::close()
{
    if (!isOpen())
        return;
    if (d->filter->mode() == QIODevice::WriteOnly)
        write(0L, 0);

    d->filter->terminate();
    if (d->bOpenedUnderlyingDevice)
        d->filter->device()->close();
    setOpenMode(QIODevice::NotOpen);
}

/*!
    That one can be quite slow, when going back. Use with care.
*/
bool VCompressionFilter::seek(qint64 pos)
{
    qint64 ioIndex = this->pos(); // current position
    if (ioIndex == pos)
        return true;

    //kDebug(7005) << "seek(" << pos << ") called";

    Q_ASSERT(d->filter->mode() == QIODevice::ReadOnly);

    if (pos == 0) {
        // We can forget about the cached data
        d->bNeedHeader = !d->bSkipHeaders;
        d->result = VAbstractCompressionFilter::Ok;
        d->filter->setInBuffer(0L, 0);
        d->filter->reset();
        QIODevice::seek(pos);
        return d->filter->device()->reset();
    }

    if (ioIndex > pos)   // we can start from here
        pos = pos - ioIndex;
    else {
        // we have to start from 0 ! Ugly and slow, but better than the previous
        // solution (KTarGz was allocating everything into memory)
        if (!seek(0)) // recursive
            return false;
    }

    //kDebug(7005) << "reading " << pos << " dummy bytes";
    QByteArray dummy(qMin(pos, (qint64)3 * BUFFER_SIZE), 0);
    d->bIgnoreData = true;
    bool result = (read(dummy.data(), pos) == pos);
    d->bIgnoreData = false;
    QIODevice::seek(pos);
    return result;
}

bool VCompressionFilter::atEnd() const
{
    return (d->result == VAbstractCompressionFilter::End)
           && QIODevice::atEnd() // take QIODevice's internal buffer into account
           && d->filter->device()->atEnd();
}

qint64 VCompressionFilter::readData(char *data, qint64 maxlen)
{
    Q_ASSERT(d->filter->mode() == QIODevice::ReadOnly);
    //kDebug(7005) << "maxlen=" << maxlen;
    VAbstractCompressionFilter *filter = d->filter;

    uint dataReceived = 0;

    // We came to the end of the stream
    if (d->result == VAbstractCompressionFilter::End)
        return dataReceived;

    // If we had an error, return -1.
    if (d->result != VAbstractCompressionFilter::Ok)
        return -1;


    qint64 outBufferSize;
    if (d->bIgnoreData) {
        outBufferSize = qMin(maxlen, (qint64)3 * BUFFER_SIZE);
    } else {
        outBufferSize = maxlen;
    }
    outBufferSize -= dataReceived;
    qint64 availOut = outBufferSize;
    filter->setOutBuffer(data, outBufferSize);

    while (dataReceived < maxlen) {
        if (filter->inBufferEmpty()) {
            // Not sure about the best size to set there.
            // For sure, it should be bigger than the header size (see comment in readHeader)
            d->buffer.resize(BUFFER_SIZE);
            // Request data from underlying device
            int size = filter->device()->read(d->buffer.data(),
                                              d->buffer.size());
            //kDebug(7005) << "got" << size << "bytes from device";
            if (size) {
                filter->setInBuffer(d->buffer.data(), size);
            } else {
                // Not enough data available in underlying device for now
                break;
            }
        }
        if (d->bNeedHeader) {
            (void) filter->readHeader();
            d->bNeedHeader = false;
        }

        d->result = filter->uncompress();

        if (d->result == VAbstractCompressionFilter::Error) {
            qWarning() << "VCompressionFilter: Error when uncompressing data";
            break;
        }

        // We got that much data since the last time we went here
        uint outReceived = availOut - filter->outBufferAvailable();
        //kDebug(7005) << "avail_out = " << filter->outBufferAvailable() << " result=" << d->result << " outReceived=" << outReceived;
        if (availOut < (uint)filter->outBufferAvailable())
            qWarning() << " last availOut " << availOut << " smaller than new avail_out=" << filter->outBufferAvailable() << " !";

        dataReceived += outReceived;
        if (!d->bIgnoreData) {  // Move on in the output buffer
            data += outReceived;
            availOut = maxlen - dataReceived;
        } else if (maxlen - dataReceived < outBufferSize) {
            availOut = maxlen - dataReceived;
        }
        if (d->result == VAbstractCompressionFilter::End) {
            //kDebug(7005) << "got END. dataReceived=" << dataReceived;
            break; // Finished.
        }
        filter->setOutBuffer(data, availOut);
    }

    return dataReceived;
}

qint64 VCompressionFilter::writeData(const char *data /*0 to finish*/, qint64 len)
{
    VAbstractCompressionFilter *filter = d->filter;
    Q_ASSERT(filter->mode() == QIODevice::WriteOnly);
    // If we had an error, return 0.
    if (d->result != VAbstractCompressionFilter::Ok)
        return 0;

    bool finish = (data == 0L);
    if (!finish) {
        filter->setInBuffer(data, len);
        if (d->bNeedHeader) {
            (void)filter->writeHeader(d->origFileName);
            d->bNeedHeader = false;
        }
    }

    uint dataWritten = 0;
    uint availIn = len;
    while (dataWritten < len || finish) {

        d->result = filter->compress(finish);

        if (d->result == VAbstractCompressionFilter::Error) {
            qWarning() << "VCompressionFilter: Error when compressing data";
            // What to do ?
            break;
        }

        // Wrote everything ?
        if (filter->inBufferEmpty() || (d->result == VAbstractCompressionFilter::End)) {
            // We got that much data since the last time we went here
            uint wrote = availIn - filter->inBufferAvailable();

            //kDebug(7005) << " Wrote everything for now. avail_in=" << filter->inBufferAvailable() << "result=" << d->result << "wrote=" << wrote;

            // Move on in the input buffer
            data += wrote;
            dataWritten += wrote;

            availIn = len - dataWritten;
            //kDebug(7005) << " availIn=" << availIn << "dataWritten=" << dataWritten << "pos=" << pos();
            if (availIn > 0)
                filter->setInBuffer(data, availIn);
        }

        if (filter->outBufferFull() || (d->result == VAbstractCompressionFilter::End) || finish) {
            //kDebug(7005) << " writing to underlying. avail_out=" << filter->outBufferAvailable();
            int towrite = d->buffer.size() - filter->outBufferAvailable();
            if (towrite > 0) {
                // Write compressed data to underlying device
                int size = filter->device()->write(d->buffer.data(), towrite);
                if (size != towrite) {
                    qWarning() << "VCompressionFilter::write. Could only write " << size << " out of " << towrite << " bytes";
                    return 0; // indicate an error (happens on disk full)
                }
                //else
                //kDebug(7005) << " wrote " << size << " bytes";
            }
            if (d->result == VAbstractCompressionFilter::End) {
                //kDebug(7005) << " END";
                Q_ASSERT(finish); // hopefully we don't get end before finishing
                break;
            }
            d->buffer.resize(BUFFER_SIZE);
            filter->setOutBuffer(d->buffer.data(), d->buffer.size());
        }
    }

    return dataWritten;
}

/*!
    For writing gzip compressed files only:
    set the name of the original file, to be used in the gzip header.
    \param fileName the name of the original file
*/
void VCompressionFilter::setOrigFileName(const QByteArray &fileName)
{
    d->origFileName = fileName;
}

/*!
    Call this let this device skip the gzip headers when reading/writing.
    This way VCompressionFilter (with gzip filter) can be used as a direct wrapper
    around zlib - this is used by KZip.
*/
void VCompressionFilter::setSkipHeaders()
{
    d->bSkipHeaders = true;
}
