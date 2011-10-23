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

namespace Qube
{
    namespace Hardware
    {
        namespace Backends
        {
            namespace UPower
            {
                class DeviceInterface : public QObject, virtual public Qube::Hardware::Ifaces::DeviceInterface
                {
                    Q_OBJECT
                    Q_INTERFACES(Qube::Hardware::Ifaces::DeviceInterface)
                public:
                    DeviceInterface(UPowerDevice *device);
                    virtual ~DeviceInterface();

                protected:
                    UPowerDevice *m_device;

                public:
                    inline static QStringList toStringList(Qube::Hardware::DeviceInterface::Type type) {
                        QStringList list;

                        switch(type) {
                        case Qube::Hardware::DeviceInterface::GenericInterface:
                            list << "generic";
                            break;
                        case Qube::Hardware::DeviceInterface::Processor:
                            // Doesn't exist with UDisks
                            break;
                        case Qube::Hardware::DeviceInterface::Block:
                            list << "block";
                            break;
                        case Qube::Hardware::DeviceInterface::StorageAccess:
                            list << "volume";
                            break;
                        case Qube::Hardware::DeviceInterface::StorageDrive:
                            list << "storage";
                            break;
                        case Qube::Hardware::DeviceInterface::OpticalDrive:
                            list << "storage.cdrom";
                            break;
                        case Qube::Hardware::DeviceInterface::StorageVolume:
                            list << "volume";
                            break;
                        case Qube::Hardware::DeviceInterface::OpticalDisc:
                            list << "volume.disc";
                            break;
                        case Qube::Hardware::DeviceInterface::Camera:
                            // Doesn't exist with UDisks
                            break;
                        case Qube::Hardware::DeviceInterface::PortableMediaPlayer:
                            // Doesn't exist with UDisks
                            break;
                        case Qube::Hardware::DeviceInterface::NetworkInterface:
                            // Doesn't exist with UDisks
                            break;
                        case Qube::Hardware::DeviceInterface::AcAdapter:
                            list << "acadapter";
                            break;
                        case Qube::Hardware::DeviceInterface::Battery:
                            list << "battery";
                            break;
                        case Qube::Hardware::DeviceInterface::Button:
                            // Doesn't exist with UDisks
                            break;
                        case Qube::Hardware::DeviceInterface::AudioInterface:
                            // Doesn't exist with UDisks
                            break;
                        case Qube::Hardware::DeviceInterface::DvbInterface:
                            // Doesn't exist with UDisks
                            break;
                        case Qube::Hardware::DeviceInterface::Video:
                            // Doesn't exist with UDisks
                            break;
                        case Qube::Hardware::DeviceInterface::SerialInterface:
                            // Doesn't exist with UDisks
                            break;
                        case Qube::Hardware::DeviceInterface::InternetGateway:
                            break;
                        case Qube::Hardware::DeviceInterface::SmartCardReader:
                            // Doesn't exist with UDisks
                            break;
                        case Qube::Hardware::DeviceInterface::NetworkShare:
                            // Doesn't exist with UPower
                            break;
                        case Qube::Hardware::DeviceInterface::Unknown:
                            break;
                        case Qube::Hardware::DeviceInterface::Last:
                            break;
                        }

                        return list;
                    }

                    inline static Qube::Hardware::DeviceInterface::Type fromString(const QString &capability) {
                        if (capability == "generic")
                            return Qube::Hardware::DeviceInterface::GenericInterface;
                        else if (capability == "processor")
                            return Qube::Hardware::DeviceInterface::Processor;
                        else if (capability == "block")
                            return Qube::Hardware::DeviceInterface::Block;
                        else if (capability == "storage")
                            return Qube::Hardware::DeviceInterface::StorageDrive;
                        else if (capability == "storage.cdrom")
                            return Qube::Hardware::DeviceInterface::OpticalDrive;
                        else if (capability == "volume")
                            return Qube::Hardware::DeviceInterface::StorageVolume;
                        else if (capability == "volume.disc")
                            return Qube::Hardware::DeviceInterface::OpticalDisc;
                        else if (capability == "camera")
                            return Qube::Hardware::DeviceInterface::Camera;
                        else if (capability == "portable_audio_player")
                            return Qube::Hardware::DeviceInterface::PortableMediaPlayer;
                        else if (capability == "net")
                            return Qube::Hardware::DeviceInterface::NetworkInterface;
                        else if (capability == "ac_adapter")
                            return Qube::Hardware::DeviceInterface::AcAdapter;
                        else if (capability == "battery")
                            return Qube::Hardware::DeviceInterface::Battery;
                        else if (capability == "button")
                            return Qube::Hardware::DeviceInterface::Button;
                        else if (capability == "alsa" || capability == "oss")
                            return Qube::Hardware::DeviceInterface::AudioInterface;
                        else if (capability == "dvb")
                            return Qube::Hardware::DeviceInterface::DvbInterface;
                        else if (capability == "video4linux")
                            return Qube::Hardware::DeviceInterface::Video;
                        else if (capability == "serial")
                            return Qube::Hardware::DeviceInterface::SerialInterface;
                        else if (capability == "smart_card_reader")
                            return Qube::Hardware::DeviceInterface::SmartCardReader;
                        else if (capability == "networkshare")
                            return Qube::Hardware::DeviceInterface::NetworkShare;
                        else
                            return Qube::Hardware::DeviceInterface::Unknown;
                    }
                };
            }
        }
    }
}

#endif // UPOWERDEVICEINTERFACE_H
