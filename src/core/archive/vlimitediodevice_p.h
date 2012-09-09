/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2001, 2002, 2007 David Faure
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

#ifndef VLIMITEDIODEVICE_P_H
#define VLIMITEDIODEVICE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Vibe API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QIODevice>

/*!
    A readonly device that reads from an underlying device
    from a given point to another (e.g. to give access to a single
    file inside an archive).

   \author David Faure <faure@kde.org>
   \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>

   \internal - used by VArchive
*/
class VLimitedIODevice : public QIODevice
{
public:
    /**
     * Creates a new VLimitedIODevice.
     * \param dev the underlying device, opened or not
     * This device itself auto-opens (in readonly mode), no need to open it.
     * \param start where to start reading (position in bytes)
     * \param length the length of the data to read (in bytes)
     */
    VLimitedIODevice(QIODevice *dev, qint64 start, qint64 length);
    virtual ~VLimitedIODevice() {}

    virtual bool isSequential() const;

    virtual bool open(QIODevice::OpenMode m);
    virtual void close();

    virtual qint64 size() const;

    virtual qint64 readData(char *data, qint64 maxlen);
    virtual qint64 writeData(const char *, qint64) {
        return -1;    // unsupported
    }
    virtual int putChar(int) {
        return -1;    // unsupported
    }

    //virtual qint64 pos() const { return m_dev->pos() - m_start; }
    virtual bool seek(qint64 pos);
    virtual qint64 bytesAvailable() const;
private:
    QIODevice *m_dev;
    qint64 m_start;
    qint64 m_length;
};

#endif // VLIMITEDIODEVICE_P_H
