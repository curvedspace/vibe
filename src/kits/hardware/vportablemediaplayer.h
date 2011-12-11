/*
    Copyright 2006 Davide Bettio <davbet@aliceposta.it>
    Copyright 2007 Kevin Ottens <ervin@kde.org>
    Copyright 2007 Jeff Mitchell <kde-dev@emailgoeshere.com>

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

#ifndef VPORTABLEMEDIAPLAYER_H
#define VPORTABLEMEDIAPLAYER_H

#include <QtCore/QStringList>
#include <QtCore/QVariant>

#include <VibeCore/VGlobal>
#include <VibeHardware/VDeviceInterface>

class VPortableMediaPlayerPrivate;
class VDevice;

/**
 * This class implements Portable Media Player device interface and represents
 * a portable media player attached to the system.
 * A portable media player is a portable device able to play multimedia files.
 * Some of them have even recording capabilities.
 * @author Davide Bettio <davbet@aliceposta.it>
 */
class VIBE_EXPORT VPortableMediaPlayer : public VDeviceInterface
{
    Q_OBJECT
    Q_PROPERTY(QStringList supportedProtocols READ supportedProtocols)
    Q_PROPERTY(QStringList supportedDrivers READ supportedDrivers)
    Q_DECLARE_PRIVATE(VPortableMediaPlayer)
    friend class VDevice;

public:

private:
    /**
     * Creates a new VPortableMediaPlayer object.
     * You generally won't need this. It's created when necessary using
     * VDevice::as().
     *
     * @param backendObject the device interface object provided by the backend
     * @see VDevice::as()
     */
    explicit VPortableMediaPlayer(QObject *backendObject);

public:
    /**
     * Destroys a portable media player object.
     */
    virtual ~VPortableMediaPlayer();

    /**
     * Get the VDeviceInterface::Type of the PortableMediaPlayer device interface.
     *
     * @return the PortableMediaPlayer device interface type
     * @see VDeviceInterface::Type
     */
    static Type deviceInterfaceType() {
        return VDeviceInterface::PortableMediaPlayer;
    }

    /**
     * Retrieves known protocols this device can speak.  This list may be dependent
     * on installed device driver libraries.
     *
     * @return a list of known protocols this device can speak
     */
    QStringList supportedProtocols() const;

    /**
     * Retrieves known installed device drivers that claim to handle this device
     * using the requested protocol.  If protocol is blank, returns a list of
     * all drivers supporting the device.
     *
     * @param protocol The protocol to get drivers for.
     * @return a list of installed drivers meeting the criteria
     */
    QStringList supportedDrivers(QString protocol = QString()) const;

    /**
     * Retrieves a driver specific string allowing to access the device.
     *
     * For example for the "mtp" driver it will return the serial number
     * of the device.
     *
     * @return the driver specific data
     */
    QVariant driverHandle(const QString &driver) const;
};

#endif // VPORTABLEMEDIAPLAYER_H
