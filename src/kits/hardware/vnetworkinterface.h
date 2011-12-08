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

#ifndef VNETWORKINTERFACE_H
#define VNETWORKINTERFACE_H

#include <VibeCore/VGlobal>
#include <VibeHardware/VDeviceInterface>

class VNetworkInterfacePrivate;
class VDevice;

/**
 * This device interface is available on network interfaces.
 */
class VIBE_EXPORT VNetworkInterface : public VDeviceInterface
{
    Q_OBJECT
    Q_PROPERTY(QString ifaceName READ ifaceName)
    Q_PROPERTY(bool wireless READ isWireless)
    Q_PROPERTY(QString hwAddress READ hwAddress)
    Q_PROPERTY(qulonglong macAddress READ macAddress)
    Q_DECLARE_PRIVATE(VNetworkInterface)
    friend class VDevice;

private:
    /**
     * Creates a new VNetworkInterface object.
     * You generally won't need this. It's created when necessary using
     * VDevice::as().
     *
     * @param backendObject the device interface object provided by the backend
     * @see VDevice::as()
     */
    explicit VNetworkInterface(QObject *backendObject);

public:
    /**
     * Destroys a VNetworkInterface object.
     */
    virtual ~VNetworkInterface();

    /**
     * Get the VDeviceInterface::Type of the NetworkInterface device interface.
     *
     * @return the NetworkInterface device interface type
     * @see VHardware::Ifaces::Enums::DeviceInterface::Type
     */
    static Type deviceInterfaceType() {
        return VDeviceInterface::NetworkInterface;
    }

    /**
     * Retrieves the name of the interface in the system.
     * This name is system dependent, it allows to identify the interface
     * in the system. For example it can be of the form "eth0" under Linux.
     *
     * @return the interface name
     */
    QString ifaceName() const;

    /**
     * Indicates if this interface is wireless.
     *
     * @return true if the interface is wireless, false otherwise
     */
    bool isWireless() const;

    /**
     * Retrieves the hardware address of the interface.
     *
     * @return the hardware address as a string
     */
    QString hwAddress() const;

    /**
     * Retrieves the MAC address of the interface.
     *
     * @return the MAC address
     */
    qulonglong macAddress() const;
};

#endif // VNETWORKINTERFACE_H