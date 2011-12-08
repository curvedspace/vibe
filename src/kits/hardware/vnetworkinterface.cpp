/*
    Copyright 2006-2007 Kevin Ottens <ervin@kde.org>

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

#include "vnetworkinterface.h"
#include "vnetworkinterface_p.h"
#include "vcall_p.h"

#include <ifaces/networkinterface.h>

VNetworkInterface::VNetworkInterface(QObject *backendObject)
    : VDeviceInterface(*new VNetworkInterfacePrivate(), backendObject)
{
}

VNetworkInterface::~VNetworkInterface()
{
}

QString VNetworkInterface::ifaceName() const
{
    Q_D(const VNetworkInterface);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::NetworkInterface *, d->backendObject(), QString(), ifaceName());
}

bool VNetworkInterface::isWireless() const
{
    Q_D(const VNetworkInterface);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::NetworkInterface *, d->backendObject(), false, isWireless());
}

QString VNetworkInterface::hwAddress() const
{
    Q_D(const VNetworkInterface);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::NetworkInterface *, d->backendObject(), QString(), hwAddress());
}

qulonglong VNetworkInterface::macAddress() const
{
    Q_D(const VNetworkInterface);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::NetworkInterface *, d->backendObject(), 0, macAddress());
}

#include "vnetworkinterface.moc"