/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2007-2008 Per Øyvind Karlsen
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Per Øyvind Karlsen <peroyvind@mandriva.org
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
