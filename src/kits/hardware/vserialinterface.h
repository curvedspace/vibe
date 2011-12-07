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

#ifndef VSERIALINTERFACE_H
#define VSERIALINTERFACE_H

#include <QtCore/QVariant>

#include <VibeHardware/VDeviceInterface>

class VSerialInterfacePrivate;
class VDevice;

/**
 * This device interface is available on serial interfaces.
 */
class VIBE_EXPORT VSerialInterface : public VDeviceInterface
{
    Q_OBJECT
    Q_PROPERTY(QVariant driverHandle READ driverHandle)
    Q_PROPERTY(QString serialType READ serialType)
    Q_PROPERTY(int port READ port)
    Q_ENUMS(SerialType)
    Q_DECLARE_PRIVATE(VSerialInterface)
    friend class VDevice;

private:
    /**
     * Creates a new VSerialInterface object.
     * You generally won't need this. It's created when necessary using
     * VDevice::as().
     *
     * @param backendObject the device interface object provided by the backend
     * @see VDevice::as()
     */
    explicit VSerialInterface(QObject *backendObject);

public:
    /**
     * Destroys a VSerialInterface object.
     */
    virtual ~VSerialInterface();

    /**
     * Get the VDeviceInterface::Type of the SerialInterface device interface.
     *
     * @return the SerialInterface device interface type
     * @see VHardware::Ifaces::Enums::DeviceInterface::Type
     */
    static Type deviceInterfaceType() {
        return VDeviceInterface::SerialInterface;
    }

    /**
     * Retrieves the name of the interface in the system.
     * This name is system dependent, it allows to identify the interface
     * in the system. For example it can be of the form "/dev/ttyS0" under Linux.
     *
     * @return the interface name
     */
    virtual QVariant driverHandle() const;

    /**
     * This enum type defines the type of a serial interface.
     *
     * - Unknown : The type could not be determined
     * - Platform : A built-in serial port
     * - USB : A USB serial port
     */
    enum SerialType { Unknown = 0, Platform, Usb };

    /**
     * Retrieves the type of the serial device.
     * Examples for Linux are "usb" for USB based serial devices,
     * or "platform" for built-in serial ports.
     *
     * @return the type of the serial device
     */
    virtual SerialType serialType() const;

    /**
     * Retrieves the port number, e.g. 0 for the first COM port.
     *
     * @return The port number of the serial device, or -1 if unknown.
     */
    virtual int port() const;
};

#endif // VSERIALINTERFACE_H
