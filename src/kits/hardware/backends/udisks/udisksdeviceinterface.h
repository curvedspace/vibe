/*
    Copyright 2010 Michael Zanetti <mzanetti@kde.org>

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

#ifndef UDISKSDEVICEINTERFACE_H
#define UDISKSDEVICEINTERFACE_H

#include <ifaces/deviceinterface.h>
#include "udisksdevice.h"

#include <QtCore/QObject>
#include <QtCore/QStringList>

namespace QubeHardware
{
    namespace Backends
    {
        namespace UDisks
        {

            class DeviceInterface : public QObject, virtual public QubeHardware::Ifaces::DeviceInterface
            {
                Q_OBJECT
                Q_INTERFACES(QubeHardware::Ifaces::DeviceInterface)
            public:
                DeviceInterface(UDisksDevice *device);
                virtual ~DeviceInterface();

            protected:
                UDisksDevice *m_device;

            public:
                inline static QStringList toStringList(QubeHardware::DeviceInterface::Type type) {
                    QStringList list;

                    switch(type) {
                    case QubeHardware::DeviceInterface::GenericInterface:
                        list << "generic";
                        break;
                    case QubeHardware::DeviceInterface::Processor:
                        // Doesn't exist with UDisks
                        break;
                    case QubeHardware::DeviceInterface::Block:
                        list << "block";
                        break;
                    case QubeHardware::DeviceInterface::StorageAccess:
                        list << "volume";
                        break;
                    case QubeHardware::DeviceInterface::StorageDrive:
                        list << "storage";
                        break;
                    case QubeHardware::DeviceInterface::OpticalDrive:
                        list << "storage.cdrom";
                        break;
                    case QubeHardware::DeviceInterface::StorageVolume:
                        list << "volume";
                        break;
                    case QubeHardware::DeviceInterface::OpticalDisc:
                        list << "volume.disc";
                        break;
                    case QubeHardware::DeviceInterface::Camera:
                        // Doesn't exist with UDisks
                        break;
                    case QubeHardware::DeviceInterface::PortableMediaPlayer:
                        // Doesn't exist with UDisks
                        break;
                    case QubeHardware::DeviceInterface::NetworkInterface:
                        // Doesn't exist with UDisks
                        break;
                    case QubeHardware::DeviceInterface::AcAdapter:
                        // Doesn't exist with UDisks
                        break;
                    case QubeHardware::DeviceInterface::Battery:
                        // Doesn't exist with UDisks
                        break;
                    case QubeHardware::DeviceInterface::Button:
                        // Doesn't exist with UDisks
                        break;
                    case QubeHardware::DeviceInterface::AudioInterface:
                        // Doesn't exist with UDisks
                        break;
                    case QubeHardware::DeviceInterface::DvbInterface:
                        // Doesn't exist with UDisks
                        break;
                    case QubeHardware::DeviceInterface::Video:
                        // Doesn't exist with UDisks
                        break;
                    case QubeHardware::DeviceInterface::SerialInterface:
                        // Doesn't exist with UDisks
                        break;
                    case QubeHardware::DeviceInterface::InternetGateway:
                        break;
                    case QubeHardware::DeviceInterface::SmartCardReader:
                        // Doesn't exist with UDisks
                    case QubeHardware::DeviceInterface::NetworkShare:
                        // Doesn't exist with UDisks
                        break;
                    case QubeHardware::DeviceInterface::Unknown:
                        break;
                    case QubeHardware::DeviceInterface::Last:
                        break;
                    }

                    return list;
                }

                inline static QubeHardware::DeviceInterface::Type fromString(const QString &capability) {
                    if (capability == "generic")
                        return QubeHardware::DeviceInterface::GenericInterface;
                    else if (capability == "processor")
                        return QubeHardware::DeviceInterface::Processor;
                    else if (capability == "block")
                        return QubeHardware::DeviceInterface::Block;
                    else if (capability == "storage")
                        return QubeHardware::DeviceInterface::StorageDrive;
                    else if (capability == "storage.cdrom")
                        return QubeHardware::DeviceInterface::OpticalDrive;
                    else if (capability == "volume")
                        return QubeHardware::DeviceInterface::StorageVolume;
                    else if (capability == "volume.disc")
                        return QubeHardware::DeviceInterface::OpticalDisc;
                    else if (capability == "camera")
                        return QubeHardware::DeviceInterface::Camera;
                    else if (capability == "portable_audio_player")
                        return QubeHardware::DeviceInterface::PortableMediaPlayer;
                    else if (capability == "net")
                        return QubeHardware::DeviceInterface::NetworkInterface;
                    else if (capability == "ac_adapter")
                        return QubeHardware::DeviceInterface::AcAdapter;
                    else if (capability == "battery")
                        return QubeHardware::DeviceInterface::Battery;
                    else if (capability == "button")
                        return QubeHardware::DeviceInterface::Button;
                    else if (capability == "alsa" || capability == "oss")
                        return QubeHardware::DeviceInterface::AudioInterface;
                    else if (capability == "dvb")
                        return QubeHardware::DeviceInterface::DvbInterface;
                    else if (capability == "video4linux")
                        return QubeHardware::DeviceInterface::Video;
                    else if (capability == "serial")
                        return QubeHardware::DeviceInterface::SerialInterface;
                    else if (capability == "smart_card_reader")
                        return QubeHardware::DeviceInterface::SmartCardReader;
                    else if (capability == "networkshare")
                        return QubeHardware::DeviceInterface::NetworkShare;
                    else
                        return QubeHardware::DeviceInterface::Unknown;
                }
            };

        }
    }
}

#endif // UDISKSDEVICEINTERFACE_H
