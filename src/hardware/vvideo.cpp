/*
    Copyright 2006-2007 Kevin Ottens <ervin@kde.org>
    Copyright 2007      Will Stephenson <wstephenson@kde.org>

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

#include "vvideo.h"
#include "vvideo_p.h"
#include "vcall_p.h"

#include <ifaces/video.h>

VVideo::VVideo(QObject *backendObject)
    : VDeviceInterface(*new VVideoPrivate(), backendObject)
{
}

VVideo::~VVideo()
{
}

QStringList VVideo::supportedProtocols() const
{
    Q_D(const VVideo);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::Video *, d->backendObject(), QStringList(), supportedProtocols());
}

QStringList VVideo::supportedDrivers(QString protocol) const
{
    Q_D(const VVideo);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::Video *, d->backendObject(), QStringList(), supportedDrivers(protocol));
}

QVariant VVideo::driverHandle(const QString &driver) const
{
    Q_D(const VVideo);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::Video *, d->backendObject(), QVariant(), driverHandle(driver));
}

#include "moc_vvideo.cpp"
