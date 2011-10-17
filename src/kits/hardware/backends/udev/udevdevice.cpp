/*
    Copyright 2010 Rafael Fernández López <ereslibre@kde.org>

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

#include "udevdevice.h"

#include "udevgenericinterface.h"
#include "udevprocessor.h"
#include "udevcamera.h"
#include "udevvideo.h"
#include "udevportablemediaplayer.h"
#include "udevdvbinterface.h"
#include "udevblock.h"
#include "udevaudiointerface.h"
#include "udevserialinterface.h"
#include "udevnetworkinterface.h"
#include "cpuinfo.h"

#include <sys/socket.h>
#include <linux/if_arp.h>

#include <QFile>
#include <QDebug>

using namespace QubeHardware::Backends::UDev;

UDevDevice::UDevDevice(const UdevQt::Device device)
    : QubeHardware::Ifaces::Device()
    , m_device(device)
{
}

UDevDevice::~UDevDevice()
{
}

QString UDevDevice::udi() const
{
    return devicePath();
}

QString UDevDevice::parentUdi() const
{
    return UDEV_UDI_PREFIX;
}

QString UDevDevice::vendor() const
{
    QString vendor = m_device.sysfsProperty("manufacturer").toString();
    if (vendor.isEmpty()) {
        if (queryDeviceInterface(QubeHardware::DeviceInterface::Processor)) {
            // sysfs doesn't have anything useful here
            vendor = extractCpuInfoLine(deviceNumber(), "vendor_id\\s+:\\s+(\\S.+)");
        } else if (queryDeviceInterface(QubeHardware::DeviceInterface::Video)) {
            vendor = m_device.deviceProperty("ID_VENDOR").toString().replace('_', " ");
        }  else if (queryDeviceInterface(QubeHardware::DeviceInterface::NetworkInterface)) {
            vendor = m_device.deviceProperty("ID_VENDOR_FROM_DATABASE").toString();
        } else if (queryDeviceInterface(QubeHardware::DeviceInterface::AudioInterface)) {
            if (m_device.parent().isValid()) {
                vendor = m_device.parent().deviceProperty("ID_VENDOR_FROM_DATABASE").toString();
            }
        }

        if (vendor.isEmpty()) {
            vendor = m_device.deviceProperty("ID_VENDOR").toString().replace('_', ' ');
        }
    }
    return vendor;
}

QString UDevDevice::product() const
{
    QString product = m_device.sysfsProperty("product").toString();
    if (product.isEmpty()) {
        if (queryDeviceInterface(QubeHardware::DeviceInterface::Processor)) {
            // sysfs doesn't have anything useful here
            product = extractCpuInfoLine(deviceNumber(), "model name\\s+:\\s+(\\S.+)");
        } else if(queryDeviceInterface(QubeHardware::DeviceInterface::Video)) {
            product = m_device.deviceProperty("ID_V4L_PRODUCT").toString();
        } else if(queryDeviceInterface(QubeHardware::DeviceInterface::AudioInterface)) {
            const AudioInterface audioIface(const_cast<UDevDevice *>(this));
            product = audioIface.name();
        }  else if(queryDeviceInterface(QubeHardware::DeviceInterface::NetworkInterface)) {
            QFile typeFile(deviceName() + "/type");
            if (typeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                int mediaType = typeFile.readAll().trimmed().toInt();
                if (mediaType == ARPHRD_LOOPBACK) {
                    product = QLatin1String("Loopback device Interface");
                } else  {
                    product = m_device.deviceProperty("ID_MODEL_FROM_DATABASE").toString();
                }
            }
        } else if(queryDeviceInterface(QubeHardware::DeviceInterface::SerialInterface)) {
            const SerialInterface serialIface(const_cast<UDevDevice *>(this));
            if (serialIface.serialType() == QubeHardware::SerialInterface::Platform) {
                product.append(QLatin1String("Platform serial"));
            } else if (serialIface.serialType() == QubeHardware::SerialInterface::Usb) {
                product.append(QLatin1String("USB Serial Port"));
            }
        }

        if (product.isEmpty()) {
            product = m_device.deviceProperty("ID_MODEL").toString().replace('_', ' ');
        }
    }
    return product;
}

QString UDevDevice::icon() const
{
    if (parentUdi().isEmpty()) {
        return QLatin1String("computer");
    }

    if (queryDeviceInterface(QubeHardware::DeviceInterface::Processor)) {
        return QLatin1String("cpu");
    } else if (queryDeviceInterface(QubeHardware::DeviceInterface::PortableMediaPlayer)) {
        // TODO: check out special cases like iPod
        return QLatin1String("multimedia-player");
    } else if (queryDeviceInterface(QubeHardware::DeviceInterface::Camera)) {
        return QLatin1String("camera-photo");
    } else if (queryDeviceInterface(QubeHardware::DeviceInterface::Video)) {
        return QLatin1String("camera-web");
    } else if (queryDeviceInterface(QubeHardware::DeviceInterface::AudioInterface)) {
        const AudioInterface audioIface(const_cast<UDevDevice *>(this));
        switch (audioIface.soundcardType()) {
        case QubeHardware::AudioInterface::InternalSoundcard:
            return QLatin1String("audio-card");
        case QubeHardware::AudioInterface::UsbSoundcard:
            return QLatin1String("audio-card-usb");
        case QubeHardware::AudioInterface::FirewireSoundcard:
            return QLatin1String("audio-card-firewire");
        case QubeHardware::AudioInterface::Headset:
            if (udi().contains("usb", Qt::CaseInsensitive) ||
                audioIface.name().contains("usb", Qt::CaseInsensitive)) {
                return QLatin1String("audio-headset-usb");
            } else {
                return QLatin1String("audio-headset");
            }
        case QubeHardware::AudioInterface::Modem:
            return QLatin1String("modem");
        }
    } else if (queryDeviceInterface(QubeHardware::DeviceInterface::SerialInterface)) {
        // TODO - a serial device can be a modem, or just
        // a COM port - need a new icon?
        return QLatin1String("modem");
    }

    return QString();
}

QStringList UDevDevice::emblems() const
{
    return QStringList();
}

QString UDevDevice::description() const
{
    if (parentUdi().isEmpty()) {
        return QObject::tr("Computer");
    }

    if (queryDeviceInterface(QubeHardware::DeviceInterface::Processor)) {
        return QObject::tr("Processor");
    } else if (queryDeviceInterface(QubeHardware::DeviceInterface::PortableMediaPlayer)) {
        // TODO: check out special cases like iPod
        return QObject::tr("Portable Media Player");
    } else if (queryDeviceInterface(QubeHardware::DeviceInterface::Camera)) {
        return QObject::tr("Camera");
    } else if (queryDeviceInterface(QubeHardware::DeviceInterface::Video)) {
        return product();
    } else if (queryDeviceInterface(QubeHardware::DeviceInterface::AudioInterface)) {
        return product();
    } else if (queryDeviceInterface(QubeHardware::DeviceInterface::NetworkInterface)) {
        const NetworkInterface networkIface(const_cast<UDevDevice *>(this));
        if (networkIface.isWireless()) {
            return QObject::tr("WLAN Interface");
        }
        return QObject::tr("Networking Interface");
    }

    return QString();
}

bool UDevDevice::queryDeviceInterface(const QubeHardware::DeviceInterface::Type &type) const
{
    switch (type) {
    case QubeHardware::DeviceInterface::GenericInterface:
        return true;

    case QubeHardware::DeviceInterface::Processor:
        return property("DRIVER").toString() == "processor";

    case QubeHardware::DeviceInterface::Camera:
        return property("ID_GPHOTO2").toInt() == 1;

    case QubeHardware::DeviceInterface::PortableMediaPlayer:
        return property("ID_MEDIA_PLAYER").toInt() == 1;

    case QubeHardware::DeviceInterface::DvbInterface:
        return m_device.subsystem() ==  QLatin1String("dvb");

    case QubeHardware::DeviceInterface::Block:
        return !property("MAJOR").toString().isEmpty();

    case QubeHardware::DeviceInterface::Video:
        return m_device.subsystem() == QLatin1String("video4linux");

    case QubeHardware::DeviceInterface::AudioInterface:
        return m_device.subsystem() == QLatin1String("sound");

    case QubeHardware::DeviceInterface::NetworkInterface:
        return m_device.subsystem() == QLatin1String("net");

    case QubeHardware::DeviceInterface::SerialInterface:
        return m_device.subsystem() == QLatin1String("tty");

    default:
        return false;
    }
}

QObject *UDevDevice::createDeviceInterface(const QubeHardware::DeviceInterface::Type &type)
{
    if (!queryDeviceInterface(type)) {
        return 0;
    }

    switch (type) {
    case QubeHardware::DeviceInterface::GenericInterface:
        return new GenericInterface(this);

    case QubeHardware::DeviceInterface::Processor:
        return new Processor(this);

    case QubeHardware::DeviceInterface::Camera:
        return new Camera(this);

    case QubeHardware::DeviceInterface::PortableMediaPlayer:
        return new PortableMediaPlayer(this);

    case QubeHardware::DeviceInterface::DvbInterface:
        return new DvbInterface(this);

    case QubeHardware::DeviceInterface::Block:
        return new Block(this);

    case QubeHardware::DeviceInterface::Video:
        return new Video(this);

    case QubeHardware::DeviceInterface::AudioInterface:
        return new AudioInterface(this);

    case QubeHardware::DeviceInterface::NetworkInterface:
        return new NetworkInterface(this);

    case QubeHardware::DeviceInterface::SerialInterface:
        return new SerialInterface(this);

    default:
        qFatal("Shouldn't happen");
        return 0;
    }
}

QString UDevDevice::device() const
{
    return devicePath();
}

QVariant UDevDevice::property(const QString &key) const
{
    const QVariant res = m_device.deviceProperty(key);
    if (res.isValid()) {
        return res;
    }
    return m_device.sysfsProperty(key);
}

QMap<QString, QVariant> UDevDevice::allProperties() const
{
    QMap<QString, QVariant> res;
    foreach (const QString &prop, m_device.deviceProperties()) {
        res[prop] = property(prop);
    }
    return res;
}

bool UDevDevice::propertyExists(const QString &key) const
{
    return m_device.deviceProperties().contains(key);
}

QString UDevDevice::systemAttribute(const char *attribute) const
{
    return m_device.sysfsProperty(attribute).toString();
}

QString UDevDevice::deviceName() const
{
    return m_device.sysfsPath();
}

int UDevDevice::deviceNumber() const
{
    return m_device.sysfsNumber();
}

QString UDevDevice::devicePath() const
{
    return QString(UDEV_UDI_PREFIX) + deviceName();
}

UdevQt::Device UDevDevice::udevDevice()
{
    return m_device;
}
