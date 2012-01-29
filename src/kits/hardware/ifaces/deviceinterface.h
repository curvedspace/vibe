/*
    Copyright 2006 Kevin Ottens <ervin@kde.org>

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

#ifndef VHARDWARE_IFACES_DEVICEINTERFACE_H
#define VHARDWARE_IFACES_DEVICEINTERFACE_H

#include <QObject>

#include <VibeCore/VGlobal>

namespace VHardware
{
    namespace Ifaces
    {
        /**
         * Base interface of all the device interfaces.
         *
         * A device interface describes what a device can do. A device generally has
         * a set of device interfaces.
         *
         * @see VHardware::Ifaces::AbstractDeviceInterface
         */
        class VIBE_EXPORT DeviceInterface
        {
        public:
            /**
             * Destroys a DeviceInterface object.
             */
            virtual ~DeviceInterface();
        };
    }
}

Q_DECLARE_INTERFACE(VHardware::Ifaces::DeviceInterface, "org.vision.Vibe.Hardware.Ifaces.DeviceInterface/0.1")

#endif // VHARDWARE_IFACES_DEVICEINTERFACE_H
