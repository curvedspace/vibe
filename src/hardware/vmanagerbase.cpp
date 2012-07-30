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

#include "vmanagerbase_p.h"

#include <stdlib.h>
#if !defined (Q_WS_WIN) && !defined (Q_OS_MAC)
#include <config-hardware.h>
#endif

#if defined (Q_OS_UNIX)
#include "backends/udisks/udisksmanager.h"
#include "backends/upower/upowermanager.h"
#endif

#if defined (HUPNP_FOUND)
#include "backends/upnp/upnpdevicemanager.h"
#endif

#if defined (UDEV_FOUND)
#include "backends/udev/udevmanager.h"
#endif

#include "backends/fstab/fstabmanager.h"


VManagerBasePrivate::VManagerBasePrivate()
{
}

VManagerBasePrivate::~VManagerBasePrivate()
{
    qDeleteAll(m_backends);
}

void VManagerBasePrivate::loadBackends()
{
#if defined(UDEV_FOUND)
    m_backends << new VHardware::Backends::UDev::UDevManager(0);
#endif
    m_backends << new VHardware::Backends::UDisks::UDisksManager(0)
               << new VHardware::Backends::UPower::UPowerManager(0)
               << new VHardware::Backends::Fstab::FstabManager(0);

#if defined (HUPNP_FOUND)
    m_backends << new VHardware::Backends::UPnP::UPnPDeviceManager(0);
#endif
}

QList<QObject *> VManagerBasePrivate::managerBackends() const
{
    return m_backends;
}
