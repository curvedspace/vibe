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

#ifndef VCOMPRESSIONFILTER_H
#define VCOMPRESSIONFILTER_H

#include <QIODevice>
#include <QString>

#include <VibeCore/VGlobal>

class QFile;

class VAbstractCompressionFilter;
class VCompressionFilterPrivate;

class VIBE_EXPORT VCompressionFilter : public QIODevice
{
public:
    virtual ~VCompressionFilter();

    virtual bool open(QIODevice::OpenMode mode);
    virtual void close();

    void setOrigFileName(const QByteArray &fileName);

    void setSkipHeaders();

    virtual bool seek(qint64);

    virtual bool atEnd() const;

    /// Reimplemented to return true. VCompressionFilter is a sequential QIODevice.
    /// Well, not really, since it supports seeking and KZip uses that.
    //virtual bool isSequential() const { return true; }

public:
    // TODO: turn those static methods into constructors
    static QIODevice *deviceForFile(const QString &fileName, const QString &mimetype = QString(),
                                    bool forceFilter = false);
    static QIODevice *device(QIODevice *inDevice, const QString &mimetype, bool autoDeleteInDevice = true);

protected:
    virtual qint64 readData(char *data, qint64 maxlen);
    virtual qint64 writeData(const char *data, qint64 len);

private:
    explicit VCompressionFilter(VAbstractCompressionFilter *filter, bool autoDeleteFilterBase = false);

private:
    VCompressionFilterPrivate *const d;
};

#endif // VCOMPRESSIONFILTER_H
