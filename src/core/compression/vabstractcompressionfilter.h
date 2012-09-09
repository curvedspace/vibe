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

#ifndef VABSTRACTCOMPRESSIONFILTER_H
#define VABSTRACTCOMPRESSIONFILTER_H

#include <QObject>
#include <QString>

#include <VibeCore/VGlobal>

class QIODevice;

class VAbstractCompressionFilterPrivate;

class VIBE_EXPORT VAbstractCompressionFilter
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
