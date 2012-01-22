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

#include <QDebug>

#include "vlimitediodevice_p.h"

VLimitedIODevice::VLimitedIODevice(QIODevice *dev, qint64 start, qint64 length) :
    m_dev(dev),
    m_start(start),
    m_length(length)
{
    //qDebug() << "start=" << start << "length=" << length;
    open(QIODevice::ReadOnly);   //krazy:exclude=syscalls
}

bool VLimitedIODevice::open(QIODevice::OpenMode m)
{
    //qDebug() << "m=" << m;
    if (m & QIODevice::ReadOnly) {
#if 0
        bool ok = false;
        if (m_dev->isOpen())
            ok = (m_dev->mode() == QIODevice::ReadOnly);
        else
            ok = m_dev->open(m);
        if (ok)
#endif
            // No concurrent access!
            m_dev->seek(m_start);
    } else
        qWarning() << "VLimitedIODevice::open only supports QIODevice::ReadOnly!";
    setOpenMode(QIODevice::ReadOnly);
    return true;
}

void VLimitedIODevice::close()
{
}

qint64 VLimitedIODevice::size() const
{
    return m_length;
}

qint64 VLimitedIODevice::readData(char *data, qint64 maxlen)
{
    // Apply upper limit
    maxlen = qMin(maxlen, m_length - pos());
    return m_dev->read(data, maxlen);
}

bool VLimitedIODevice::seek(qint64 pos)
{
    Q_ASSERT(pos <= m_length);

    // Apply upper limit
    pos = qMin(pos, m_length);
    bool ret = m_dev->seek(m_start + pos);
    if (ret)
        QIODevice::seek(pos);
    return ret;
}

qint64 VLimitedIODevice::bytesAvailable() const
{
    return m_length + QIODevice::bytesAvailable();
}

bool VLimitedIODevice::isSequential() const
{
    return m_dev->isSequential();
}
