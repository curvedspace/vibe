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

#ifndef VHARDWARE_IFACES_SMARTCARDREADER_H
#define VHARDWARE_IFACES_SMARTCARDREADER_H

#include <VibeHardware/VSmartCardReader>
#include <ifaces/deviceinterface.h>

namespace VHardware
{
    namespace Ifaces
    {
        /**
         * This device interface is available on smart card readers.
         */
        class VIBE_EXPORT SmartCardReader : virtual public DeviceInterface
        {
        public:
            /**
             * Destroys a SmartCardReader object.
             */
            virtual ~SmartCardReader();

            /**
             * Retrieves the type of this smart card reader.
             *
             * @return the reader type
             * @see VSmartCardReader::ReaderType
             */
            virtual VSmartCardReader::ReaderType readerType() const = 0;
        };
    }
}

Q_DECLARE_INTERFACE(VHardware::Ifaces::SmartCardReader, "org.maui.Hardware.Ifaces.SmartCardReader/0.1")

#endif // VHARDWARE_IFACES_SMARTCARDREADER_H