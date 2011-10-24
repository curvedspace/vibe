/*
    Copyright 2006 Davide Bettio <davbet@aliceposta.it>
    Copyright 2007 Kevin Ottens <ervin@kde.org>
    Copyright 2007 Jeff Mitchell <kde-dev@emailgoeshere.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "portablemediaplayer.h"
#include "portablemediaplayer_p.h"

#include "call_p.h"
#include <ifaces/portablemediaplayer.h>

Qube::Hardware::PortableMediaPlayer::PortableMediaPlayer(QObject *backendObject)
    : DeviceInterface(*new PortableMediaPlayerPrivate(), backendObject)
{
}

Qube::Hardware::PortableMediaPlayer::~PortableMediaPlayer()
{

}

QStringList Qube::Hardware::PortableMediaPlayer::supportedProtocols() const
{
    Q_D(const PortableMediaPlayer);
    return_QUBE_HARDWARE_CALL(Ifaces::PortableMediaPlayer *, d->backendObject(), QStringList(), supportedProtocols());
}

QStringList Qube::Hardware::PortableMediaPlayer::supportedDrivers(QString protocol) const
{
    Q_D(const PortableMediaPlayer);
    return_QUBE_HARDWARE_CALL(Ifaces::PortableMediaPlayer *, d->backendObject(), QStringList(), supportedDrivers(protocol));
}

QVariant Qube::Hardware::PortableMediaPlayer::driverHandle(const QString &driver) const
{
    Q_D(const PortableMediaPlayer);
    return_QUBE_HARDWARE_CALL(Ifaces::PortableMediaPlayer *, d->backendObject(), QVariant(), driverHandle(driver));
}

#include "portablemediaplayer.moc"
