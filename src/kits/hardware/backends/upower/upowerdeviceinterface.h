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

#ifndef UPOWERDEVICEINTERFACE_H
#define UPOWERDEVICEINTERFACE_H

#include <ifaces/deviceinterface.h>
#include "upowerdevice.h"

#include <QtCore/QObject>
#include <QtCore/QStringList>

namespace VHardware
{
    namespace Backends
    {
        namespace UPower
        {
            class DeviceInterface : public QObject, virtual public VHardware::Ifaces::DeviceInterface
            {
                Q_OBJECT
                Q_INTERFACES(VHardware::Ifaces::DeviceInterface)
            public:
                DeviceInterface(UPowerDevice *device);
                virtual ~DeviceInterface();

            protected:
                UPowerDevice *m_device;

            public:
                inline static QStringList toStringList(VDeviceInterface::Type type) {
                    QStringList list;

                    switch (type) {
                        case VDeviceInterface::GenericInterface:
                            list << "generic";
                            break;
                        case VDeviceInterface::Processor:
                            // Doesn't exist with UDisks
                            break;
                        case VDeviceInterface::Block:
                            list << "block";
                            break;
                        case VDeviceInterface::StorageAccess:
                            list << "volume";
                            break;
                        case VDeviceInterface::StorageDrive:
                            list << "storage";
                            break;
                        case VDeviceInterface::OpticalDrive:
                            list << "storage.cdrom";
                            break;
                        case VDeviceInterface::StorageVolume:
                            list << "volume";
                            break;
                        case VDeviceInterface::OpticalDisc:
                            list << "volume.disc";
                            break;
                        case VDeviceInterface::Camera:
                            // Doesn't exist with UDisks
                            break;
                        case VDeviceInterface::PortableMediaPlayer:
                            // Doesn't exist with UDisks
                            break;
                        case VDeviceInterface::NetworkInterface:
                            // Doesn't exist with UDisks
                            break;
                        case VDeviceInterface::AcAdapter:
                            list << "acadapter";
                            break;
                        case VDeviceInterface::Battery:
                            list << "battery";
                            break;
                        case VDeviceInterface::Button:
                            // Doesn't exist with UDisks
                            break;
                        case VDeviceInterface::AudioInterface:
                            // Doesn't exist with UDisks
                            break;
                        case VDeviceInterface::DvbInterface:
                            // Doesn't exist with UDisks
                            break;
                        case VDeviceInterface::Video:
                            // Doesn't exist with UDisks
                            break;
                        case VDeviceInterface::SerialInterface:
                            // Doesn't exist with UDisks
                            break;
                        case VDeviceInterface::InternetGateway:
                            break;
                        case VDeviceInterface::SmartCardReader:
                            // Doesn't exist with UDisks
                            break;
                        case VDeviceInterface::NetworkShare:
                            // Doesn't exist with UPower
                            break;
                        case VDeviceInterface::Unknown:
                            break;
                        case VDeviceInterface::Last:
                            break;
                    }

                    return list;
                }

                inline static VDeviceInterface::Type fromString(const QString &capability) {
                    if (capability == "generic")
                        return VDeviceInterface::GenericInterface;
                    else if (capability == "processor")
                        return VDeviceInterface::Processor;
                    else if (capability == "block")
                        return VDeviceInterface::Block;
                    else if (capability == "storage")
                        return VDeviceInterface::StorageDrive;
                    else if (capability == "storage.cdrom")
                        return VDeviceInterface::OpticalDrive;
                    else if (capability == "volume")
                        return VDeviceInterface::StorageVolume;
                    else if (capability == "volume.disc")
                        return VDeviceInterface::OpticalDisc;
                    else if (capability == "camera")
                        return VDeviceInterface::Camera;
                    else if (capability == "portable_audio_player")
                        return VDeviceInterface::PortableMediaPlayer;
                    else if (capability == "net")
                        return VDeviceInterface::NetworkInterface;
                    else if (capability == "ac_adapter")
                        return VDeviceInterface::AcAdapter;
                    else if (capability == "battery")
                        return VDeviceInterface::Battery;
                    else if (capability == "button")
                        return VDeviceInterface::Button;
                    else if (capability == "alsa" || capability == "oss")
                        return VDeviceInterface::AudioInterface;
                    else if (capability == "dvb")
                        return VDeviceInterface::DvbInterface;
                    else if (capability == "video4linux")
                        return VDeviceInterface::Video;
                    else if (capability == "serial")
                        return VDeviceInterface::SerialInterface;
                    else if (capability == "smart_card_reader")
                        return VDeviceInterface::SmartCardReader;
                    else if (capability == "networkshare")
                        return VDeviceInterface::NetworkShare;
                    else
                        return VDeviceInterface::Unknown;
                }
            };
        }
    }
}

#endif // UPOWERDEVICEINTERFACE_H
