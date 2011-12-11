/*
    Copyright 2011 Mario Bensi <mbensi@ipsquad.net>

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

#ifndef VNETWORKSHARE_H
#define VNETWORKSHARE_H

#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QVariant>

#include <VibeCore/VGlobal>
#include <VibeHardware/VDeviceInterface>

class VDevice;
class VNetworkSharePrivate;

/**
 * VNetworkShare interface.
 *
 * A VNetworkShare interface is used to determine the type of
 * network access.
 */
class VIBE_EXPORT VNetworkShare : public VDeviceInterface
{
    Q_OBJECT
    Q_ENUMS(ShareType)
    Q_PROPERTY(ShareType type READ type)
    Q_PROPERTY(QUrl url READ url)
    Q_DECLARE_PRIVATE(VNetworkShare)
    friend class VDevice;

private:
    /**
     * Creates a new VNetworkShare object.
     * You generally won't need this. It's created when necessary using
     * VDevice::as().
     *
     * @param backendObject the device interface object provided by the backend
     * @see VDevice::as()
     */
    explicit VNetworkShare(QObject *backendObject);

public:
    /**
     * Destroys a VNetworkShare object.
     */
    virtual ~VNetworkShare();

    /**
     * This enum type defines the type of networkShare device can be.
     *
     * - Unknown : a unsupported network protocol
     * - Nfs : nfs protocol
     * - Cifs : samba protocol
     * - Upnp : Upnp protocol
     */
    enum ShareType { Unknown, Nfs, Cifs, Upnp };

    /**
     * Get the VDeviceInterface::Type of the NetworkShare device interface.
     *
     * @return the NetworkShare device interface type
     * @see VDeviceInterface::Type
     */
    static Type deviceInterfaceType() {
        return VDeviceInterface::NetworkShare;
    }

    /**
     * Retrieves the type of network share
     *
     * @return the type of network share
     */
    ShareType type() const;

    /**
     * Retrieves the url of network share
     *
     * @return the url of network share
     */
    QUrl url() const;
};

#endif // VNETWORKSHARE_H
