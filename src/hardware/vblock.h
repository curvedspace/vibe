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

#ifndef VBLOCK_H
#define VBLOCK_H

#include <VibeHardware/VDeviceInterface>

class VBlockPrivate;
class VDevice;

/**
 * This device interface is available on block devices.
 *
 * A block device is an adressable device such as drive or partition.
 * It is possible to interact with such a device using a special file
 * in the system.
 */
class VIBE_EXPORT VBlock : public VDeviceInterface
{
    Q_OBJECT
    Q_PROPERTY(int major READ deviceMajor)
    Q_PROPERTY(int minor READ deviceMinor)
    Q_PROPERTY(QString device READ device)
    Q_DECLARE_PRIVATE(VBlock)
    friend class VDevice;

private:
    /**
     * Creates a new VBlock object.
     * You generally won't need this. It's created when necessary using
     * VDevice::as().
     *
     * @param backendObject the device interface object provided by the backend
     * @see VDevice::as()
     */
    explicit VBlock(QObject *backendObject);

public:
    /**
     * Destroys a VBlock object.
     */
    virtual ~VBlock();

    /**
     * Get the VDeviceInterface::Type of the Block device interface.
     *
     * @return the Block device interface type
     * @see VHardware::Ifaces::Enums::DeviceInterface::Type
     */
    static Type deviceInterfaceType() {
        return VDeviceInterface::Block;
    }

    /**
     * Retrieves the major number of the node file to interact with
     * the device.
     *
     * @return the device major number
     */
    int deviceMajor() const;

    /**
     * Retrieves the minor number of the node file to interact with
     * the device.
     *
     * @return the device minor number
     */
    int deviceMinor() const;

    /**
     * Retrieves the absolute path of the special file to interact
     * with the device.
     *
     * @return the absolute path of the special file to interact with
     * the device
     */
    QString device() const;
};

#endif // VBLOCK_H
