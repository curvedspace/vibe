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

#ifndef VACADAPTER_H
#define VACADAPTER_H

#include <VibeCore/VGlobal>
#include <VibeHardware/VDeviceInterface>

class VAcAdapterPrivate;
class VDevice;

/**
 * This device interface is available on AC adapters.
 */
class VIBE_EXPORT VAcAdapter : public VDeviceInterface
{
    Q_OBJECT
    Q_PROPERTY(bool plugged READ isPlugged)
    Q_DECLARE_PRIVATE(VAcAdapter)
    friend class VDevice;

private:
    /**
     * Creates a new VAcAdapter object.
     * You generally won't need this. It's created when necessary using
     * VDevice::as().
     *
     * @param backendObject the device interface object provided by the backend
     * @see VDevice::as()
     */
    explicit VAcAdapter(QObject *backendObject);

public:
    /**
     * Destroys an VAcAdapter object.
     */
    virtual ~VAcAdapter();

    /**
     * Get the VDeviceInterface::Type of the AcAdapter device interface.
     *
     * @return the VAcAdapter device interface type
     * @see VHardware::Ifaces::Enums::DeviceInterface::Type
     */
    static Type deviceInterfaceType() {
        return VDeviceInterface::AcAdapter;
    }

    /**
     * Indicates if this AC adapter is plugged.
     *
     * @return true if the adapter is plugged, false otherwise
     */
    bool isPlugged() const;

Q_SIGNALS:
    /**
     * This signal is emitted when the AC adapter is plugged or unplugged.
     *
     * @param newState true if the AC adapter is plugged, false otherwise
     * @param udi the UDI of the AC adapter
     */
    void plugStateChanged(bool newState, const QString &udi);
};

#endif // VACADAPTER_H
