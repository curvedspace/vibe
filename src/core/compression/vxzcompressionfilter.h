/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2007-2008 Per Øyvind Karlsen
 * Copyright (C) 2000 David Faure <faure@kde.org>
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Per Øyvind Karlsen <peroyvind@mandriva.org
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

#ifndef VXZCOMPRESSIONFILTER_H
#define VXZCOMPRESSIONFILTER_H

#include <config-compression.h>

#if HAVE_XZ_SUPPORT

#include "vabstractcompressionfilter.h"

/**
 * Internal class used by VCompressionFilter
 * @internal
 */
class VXzCompressionFilter : public VAbstractCompressionFilter
{
public:
    VXzCompressionFilter();
    virtual ~VXzCompressionFilter();

    virtual void init(int);
    virtual int mode() const;
    virtual void terminate();
    virtual void reset();
    virtual bool readHeader() {
        return true;    // lzma handles it by itself ! Cool !
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

#endif // VXZCOMPRESSIONFILTER_H
