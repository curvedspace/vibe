/*
    Copyright 2010 Paulo Romulo Alves Barros <paulo.romulo@kdemail.net>

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

#ifndef VHARDWARE_BACKENDS_UPNP_DEVICE_INTERFACE_H
#define VHARDWARE_BACKENDS_UPNP_DEVICE_INTERFACE_H

#include <QtCore/QObject>

#include <backends/upnp/upnpdevice.h>
#include <ifaces/deviceinterface.h>

namespace VHardware
{
    namespace Backends
    {
        namespace UPnP
        {
            class UPnPDeviceInterface : public QObject, virtual public VHardware::Ifaces::DeviceInterface
            {
                Q_OBJECT
                Q_INTERFACES(VHardware::Ifaces::DeviceInterface)
            public:
                explicit UPnPDeviceInterface(VHardware::Backends::UPnP::UPnPDevice *device);

                virtual ~UPnPDeviceInterface();

            public:
                const VHardware::Backends::UPnP::UPnPDevice *upnpDevice() const;

            private:
                const VHardware::Backends::UPnP::UPnPDevice *m_upnpDevice;

            };
        }
    }
}

#endif // VHARDWARE_BACKENDS_UPNP_DEVICE_INTERFACE_H
