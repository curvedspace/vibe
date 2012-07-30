/*
    Copyright 2010 Alex Fiestas <alex@eyeos.org>
    Copyright 2010 UFO Coders <info@ufocoders.com>

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

#ifndef VHARDWARE_BACKENDS_UDEV_UDEVNETWORKINTERFACE_H
#define VHARDWARE_BACKENDS_UDEV_UDEVNETWORKINTERFACE_H

#include <ifaces/networkinterface.h>
#include "udevdeviceinterface.h"

namespace VHardware
{
    namespace Backends
    {
        namespace UDev
        {
            class UDevDevice;

            class NetworkInterface : public DeviceInterface, virtual public VHardware::Ifaces::NetworkInterface
            {
                Q_OBJECT
                Q_INTERFACES(VHardware::Ifaces::NetworkInterface)
            public:
                NetworkInterface(UDevDevice *device);
                virtual ~NetworkInterface();

                virtual QString ifaceName() const;
                virtual bool isWireless() const;
                virtual QString hwAddress() const;
                virtual qulonglong macAddress() const;

            private:
                QString     m_hwAddress;
            };
        }
    }
}

#endif // VHARDWARE_BACKENDS_UDEV_UDEVNETWORKINTERFACE_H
