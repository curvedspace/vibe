/*
    Copyright 2009 Christopher Blauvelt <cblauvelt@gmail.com>

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

#ifndef VSMARTCARDREADER_H
#define VSMARTCARDREADER_H

#include <VibeHardware/VDeviceInterface>

class VSmartCardReaderPrivate;
class Device;

/**
 * This device interface is available on smart card readers.
 */
class VIBE_EXPORT VSmartCardReader : public VDeviceInterface
{
    Q_OBJECT
    Q_ENUMS(ReaderType)
    Q_PROPERTY(ReaderType readerType READ readerType)
    Q_DECLARE_PRIVATE(VSmartCardReader)
    friend class VDevice;

public:
    /**
     * This enum type defines the type of smart card reader attached
     *
     * - CardReader : A generic smart card reader
     * - CryptoToken : A smart card reader with a card built into the device
     */
    enum ReaderType { UnknownReaderType = -1,
                      CardReader, CryptoToken
                    };

private:
    /**
     * Creates a new VSmartCardReader object.
     * You generally won't need this. It's created when necessary using
     * VDevice::as().
     *
     * @param backendObject the device interface object provided by the backend
     * @see VDevice::as()
     */
    explicit VSmartCardReader(QObject *backendObject);

public:
    /**
     * Destroys a VSmartCardReader object.
     */
    virtual ~VSmartCardReader();

    /**
     * Get the VDeviceInterface::Type of the SmartCardReader device interface.
     *
     * @return the SmartCardReader device interface type
     * @see VDeviceInterface::Type
     */
    static Type deviceInterfaceType() {
        return VDeviceInterface::SmartCardReader;
    }

    /**
     * Retrieves the type of this smart card reader.
     *
     * @return the smart card reader type
     * @see VSmartCardReader::ReaderType
     */
    ReaderType readerType() const;
};

#endif // VSMARTCARDREADER_H
