/*
    Copyright 2009 Harald Fernengel <harry@kdevelop.org>

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

#ifndef VHARDWARE_IFACES_SERIALINTERFACE_H
#define VHARDWARE_IFACES_SERIALINTERFACE_H

#include <VibeHardware/VSerialInterface>
#include <ifaces/deviceinterface.h>

namespace VHardware
{
    namespace Ifaces
    {
        /**
         * This device interface is available on serial interfaces,
         * like modems.
         */
        class VIBE_EXPORT SerialInterface : virtual public DeviceInterface
        {
        public:
            /**
             * Destroys a SerialInterface object.
             */
            virtual ~SerialInterface();

            /**
             * Retrieves the name of the interface in the system.
             * This name is system dependent, it allows to identify the interface
             * in the system. For example it can be of the form "/dev/ttyS0" under Linux.
             *
             * @return the interface name
             */
            virtual QVariant driverHandle() const = 0;

            /**
             * Retrieves the type of the serial device.
             * Examples for Linux are "usb" for USB based serial devices,
             * or "platform" for built-in serial ports.
             *
             * @return the type of the serial device
             */
            virtual VSerialInterface::SerialType serialType() const = 0;

            /**
             * Retrieves the port number, e.g. 0 for the first COM port.
             *
             * @return The port number of the serial device, or -1 if unknown.
             */
            virtual int port() const = 0;
        };
    }
}

Q_DECLARE_INTERFACE(VHardware::Ifaces::SerialInterface, "org.vision.Vibe.Hardware.Ifaces.SerialInterface/0.1")

#endif // VHARDWARE_IFACES_SERIALINTERFACE_H
