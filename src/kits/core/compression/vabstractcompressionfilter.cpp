/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2000-2005 David Faure
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *	David Faure <faure@kde.org>
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#include <VibeCore/VMimeType>

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

/*
 * VAbstractCompressionFilter
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

void VAbstractCompressionFilter::setDevice(QIODevice *dev, bool autodelete)
{
    m_dev = dev;
    m_bAutoDel = autodelete;
}

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

void VAbstractCompressionFilter::virtual_hook(int, void *)
{
    /*BASE::virtual_hook( id, data );*/
}
