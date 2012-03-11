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

#include <config-compression.h>

#include <QDebug>
#include <QIODevice>

#include "vgzipcompressionfilter.h"
#if HAVE_BZIP2_SUPPORT
#  include "vbzip2compressionfilter.h"
#endif
#if HAVE_XZ_SUPPORT
#  include "vxzcompressionfilter.h"
#endif

#include "vabstractcompressionfilter.h"
#include "vabstractcompressionfilter_p.h"

/*
 * VAbstractCompressionFilterPrivate
 */

VAbstractCompressionFilterPrivate::VAbstractCompressionFilterPrivate() :
    m_flags(VAbstractCompressionFilter::WithHeaders)
{
}

/*!
    This is the base class for compression filters
    such as gzip and bzip2. It's pretty much internal.
    Don't use directly, use VCompressionFilter instead.

    \author David Faure <faure@kde.org>
     \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>

    \internal
*/

VAbstractCompressionFilter::VAbstractCompressionFilter() :
    m_dev(0L),
    m_bAutoDel(false),
    d(new VAbstractCompressionFilterPrivate)
{
}

VAbstractCompressionFilter::~VAbstractCompressionFilter()
{
    if (m_bAutoDel)
        delete m_dev;
    delete d;
}

/**
    Sets the device on which the filter will work
    \param dev the device on which the filter will work
    \param autodelete if true, @p dev is deleted when the filter is deleted
*/
void VAbstractCompressionFilter::setDevice(QIODevice *dev, bool autodelete)
{
    m_dev = dev;
    m_bAutoDel = autodelete;
}

/**
    Returns the device on which the filter will work.
    \returns the device on which the filter will work
*/
QIODevice *VAbstractCompressionFilter::device()
{
    return m_dev;
}

bool VAbstractCompressionFilter::inBufferEmpty() const
{
    return inBufferAvailable() == 0;
}

bool VAbstractCompressionFilter::outBufferFull() const
{
    return outBufferAvailable() == 0;
}

/*!
    Call this to create the appropriate filter for the file
    named @p fileName.
    \param fileName the name of the file to filter
    \return the filter for the @p fileName, or 0 if not found
*/
VAbstractCompressionFilter *VAbstractCompressionFilter::findFilterByFileName(const QString &fileName)
{
    if (fileName.endsWith(QLatin1String(".gz"), Qt::CaseInsensitive))
        return new VGzipCompressionFilter;
#if HAVE_BZIP2_SUPPORT
    if (fileName.endsWith(QLatin1String(".bz2"), Qt::CaseInsensitive))
        return new VBzip2CompressionFilter;
#endif
#if HAVE_XZ_SUPPORT
    if (fileName.endsWith(QLatin1String(".lzma"), Qt::CaseInsensitive) || fileName.endsWith(QLatin1String(".xz"), Qt::CaseInsensitive))
        return new VXzCompressionFilter;
#endif
    else {
        // Not a warning, since this is called often with other mimetypes (see #88574)...
        // maybe we can avoid that though?
        //qDebug() << "VAbstractCompressionFilter::findFilterByFileName : no filter found for " << fileName;
    }

    return 0;
}

/*!
    Call this to create the appropriate filter for the mimetype
    @p mimeType. For instance application/x-gzip.
    \param mimeType the mime type of the file to filter
    \return the filter for the @p mimeType, or 0 if not found
*/
VAbstractCompressionFilter *VAbstractCompressionFilter::findFilterByMimeType(const QString &mimeType)
{
    if (mimeType == QLatin1String("application/x-gzip"))
        return new VGzipCompressionFilter;
#if HAVE_BZIP2_SUPPORT
    if (mimeType == QLatin1String("application/x-bzip"))
        return new VBzip2CompressionFilter;
#endif
#if HAVE_XZ_SUPPORT
    if (mimeType == QLatin1String("application/x-xz") ||
            mimeType == QLatin1String("application/x-lzma"))
        return new VXzCompressionFilter;
#endif

    // not a warning, since this is called often with other mimetypes (see #88574)...
    // maybe we can avoid that though?
    //qDebug() << "no filter found for" << mimeType;
    return 0;
}

void VAbstractCompressionFilter::terminate()
{
}

void VAbstractCompressionFilter::reset()
{
}

void VAbstractCompressionFilter::setFilterFlags(FilterFlags flags)
{
    d->m_flags = flags;
}

VAbstractCompressionFilter::FilterFlags VAbstractCompressionFilter::filterFlags() const
{
    return d->m_flags;
}
