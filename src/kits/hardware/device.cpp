/*
    Copyright 2005-2007 Kevin Ottens <ervin@kde.org>

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

#include "device.h"
#include "device_p.h"
#include "devicenotifier.h"
#include "devicemanager_p.h"

#include "deviceinterface_p.h"
#include "soliddefs_p.h"

#include <ifaces/device.h>

#include <Qube/Hardware/genericinterface.h>
#include <ifaces/genericinterface.h>
#include <Qube/Hardware/processor.h>
#include <ifaces/processor.h>
#include <Qube/Hardware/block.h>
#include <ifaces/block.h>
#include <Qube/Hardware/storageaccess.h>
#include <ifaces/storageaccess.h>
#include <Qube/Hardware/storagedrive.h>
#include <ifaces/storagedrive.h>
#include <Qube/Hardware/opticaldrive.h>
#include <ifaces/opticaldrive.h>
#include <Qube/Hardware/storagevolume.h>
#include <ifaces/storagevolume.h>
#include <Qube/Hardware/opticaldisc.h>
#include <ifaces/opticaldisc.h>
#include <Qube/Hardware/camera.h>
#include <ifaces/camera.h>
#include <Qube/Hardware/portablemediaplayer.h>
#include <ifaces/portablemediaplayer.h>
#include <Qube/Hardware/networkinterface.h>
#include <ifaces/networkinterface.h>
#include <Qube/Hardware/networkshare.h>
#include <ifaces/networkshare.h>
#include <Qube/Hardware/acadapter.h>
#include <ifaces/acadapter.h>
#include <Qube/Hardware/battery.h>
#include <ifaces/battery.h>
#include <Qube/Hardware/button.h>
#include <ifaces/button.h>
#include <Qube/Hardware/audiointerface.h>
#include <ifaces/audiointerface.h>
#include <Qube/Hardware/dvbinterface.h>
#include <ifaces/dvbinterface.h>
#include <Qube/Hardware/video.h>
#include <ifaces/video.h>
#include <Qube/Hardware/serialinterface.h>
#include <ifaces/serialinterface.h>
#include <Qube/Hardware/smartcardreader.h>
#include <ifaces/smartcardreader.h>
#include <Qube/Hardware/internetgateway.h>
#include <ifaces/internetgateway.h>


Qube::Hardware::Device::Device(const QString &udi)
{
    DeviceManagerPrivate *manager
    = static_cast<DeviceManagerPrivate *>(Qube::Hardware::DeviceNotifier::instance());
    d = manager->findRegisteredDevice(udi);
}

Qube::Hardware::Device::Device(const Device &device)
    : d(device.d)
{
}

Qube::Hardware::Device::~Device()
{
}

Qube::Hardware::Device &Qube::Hardware::Device::operator=(const Qube::Hardware::Device &device)
{
    d = device.d;
    return *this;
}

bool Qube::Hardware::Device::isValid() const
{
    return d->backendObject()!=0;
}

QString Qube::Hardware::Device::udi() const
{
    return d->udi();
}

QString Qube::Hardware::Device::parentUdi() const
{
    return_SOLID_CALL(Ifaces::Device *, d->backendObject(), QString(), parentUdi());
}

Qube::Hardware::Device Qube::Hardware::Device::parent() const
{
    QString udi = parentUdi();

    if (udi.isEmpty()) {
        return Device();
    } else {
        return Device(udi);
    }
}

QString Qube::Hardware::Device::vendor() const
{
    return_SOLID_CALL(Ifaces::Device *, d->backendObject(), QString(), vendor());
}

QString Qube::Hardware::Device::product() const
{
    return_SOLID_CALL(Ifaces::Device *, d->backendObject(), QString(), product());
}

QString Qube::Hardware::Device::icon() const
{
    return_SOLID_CALL(Ifaces::Device *, d->backendObject(), QString(), icon());
}

QStringList Qube::Hardware::Device::emblems() const
{
    return_SOLID_CALL(Ifaces::Device *, d->backendObject(), QStringList(), emblems());
}

QString Qube::Hardware::Device::description() const
{
    return_SOLID_CALL(Ifaces::Device *, d->backendObject(), QString(), description());
}

bool Qube::Hardware::Device::isDeviceInterface(const DeviceInterface::Type &type) const
{
    return_SOLID_CALL(Ifaces::Device *, d->backendObject(), false, queryDeviceInterface(type));
}

#define deviceinterface_cast(IfaceType, DevType, backendObject) \
    (qobject_cast<IfaceType *>(backendObject) ? new DevType(backendObject) : 0)

Qube::Hardware::DeviceInterface *Qube::Hardware::Device::asDeviceInterface(const DeviceInterface::Type &type)
{
    const Qube::Hardware::DeviceInterface *interface = const_cast<const Device *>(this)->asDeviceInterface(type);
    return const_cast<Qube::Hardware::DeviceInterface *>(interface);
}

const Qube::Hardware::DeviceInterface *Qube::Hardware::Device::asDeviceInterface(const DeviceInterface::Type &type) const
{
    Ifaces::Device *device = qobject_cast<Ifaces::Device *>(d->backendObject());

    if (device!=0) {
        DeviceInterface *iface = d->interface(type);

        if (iface!=0) {
            return iface;
        }

        QObject *dev_iface = device->createDeviceInterface(type);

        if (dev_iface!=0) {
            switch (type) {
            case DeviceInterface::GenericInterface:
                iface = deviceinterface_cast(Ifaces::GenericInterface, GenericInterface, dev_iface);
                break;
            case DeviceInterface::Processor:
                iface = deviceinterface_cast(Ifaces::Processor, Processor, dev_iface);
                break;
            case DeviceInterface::Block:
                iface = deviceinterface_cast(Ifaces::Block, Block, dev_iface);
                break;
            case DeviceInterface::StorageAccess:
                iface = deviceinterface_cast(Ifaces::StorageAccess, StorageAccess, dev_iface);
                break;
            case DeviceInterface::StorageDrive:
                iface = deviceinterface_cast(Ifaces::StorageDrive, StorageDrive, dev_iface);
                break;
            case DeviceInterface::OpticalDrive:
                iface = deviceinterface_cast(Ifaces::OpticalDrive, OpticalDrive, dev_iface);
                break;
            case DeviceInterface::StorageVolume:
                iface = deviceinterface_cast(Ifaces::StorageVolume, StorageVolume, dev_iface);
                break;
            case DeviceInterface::OpticalDisc:
                iface = deviceinterface_cast(Ifaces::OpticalDisc, OpticalDisc, dev_iface);
                break;
            case DeviceInterface::Camera:
                iface = deviceinterface_cast(Ifaces::Camera, Camera, dev_iface);
                break;
            case DeviceInterface::PortableMediaPlayer:
                iface = deviceinterface_cast(Ifaces::PortableMediaPlayer, PortableMediaPlayer, dev_iface);
                break;
            case DeviceInterface::NetworkInterface:
                iface = deviceinterface_cast(Ifaces::NetworkInterface, NetworkInterface, dev_iface);
                break;
            case DeviceInterface::AcAdapter:
                iface = deviceinterface_cast(Ifaces::AcAdapter, AcAdapter, dev_iface);
                break;
            case DeviceInterface::Battery:
                iface = deviceinterface_cast(Ifaces::Battery, Battery, dev_iface);
                break;
            case DeviceInterface::Button:
                iface = deviceinterface_cast(Ifaces::Button, Button, dev_iface);
                break;
            case DeviceInterface::AudioInterface:
                iface = deviceinterface_cast(Ifaces::AudioInterface, AudioInterface, dev_iface);
                break;
            case DeviceInterface::DvbInterface:
                iface = deviceinterface_cast(Ifaces::DvbInterface, DvbInterface, dev_iface);
                break;
            case DeviceInterface::Video:
                iface = deviceinterface_cast(Ifaces::Video, Video, dev_iface);
                break;
            case DeviceInterface::SerialInterface:
                iface = deviceinterface_cast(Ifaces::SerialInterface, SerialInterface, dev_iface);
                break;
            case DeviceInterface::SmartCardReader:
                iface = deviceinterface_cast(Ifaces::SmartCardReader, SmartCardReader, dev_iface);
                break;
            case DeviceInterface::InternetGateway:
                iface = deviceinterface_cast(Ifaces::InternetGateway, InternetGateway, dev_iface);
                break;
            case DeviceInterface::NetworkShare:
                iface = deviceinterface_cast(Ifaces::NetworkShare, NetworkShare, dev_iface);
                break;
            case DeviceInterface::Unknown:
            case DeviceInterface::Last:
                break;
            }
        }

        if (iface!=0) {
            // Lie on the constness since we're simply doing caching here
            const_cast<Device *>(this)->d->setInterface(type, iface);
            iface->d_ptr->setDevicePrivate(d.data());
        }

        return iface;
    } else {
        return 0;
    }
}


//////////////////////////////////////////////////////////////////////


Qube::Hardware::DevicePrivate::DevicePrivate(const QString &udi)
    : QObject(), QSharedData(), m_udi(udi)
{
}

Qube::Hardware::DevicePrivate::~DevicePrivate()
{
    foreach (DeviceInterface *iface, m_ifaces) {
        delete iface->d_ptr->backendObject();
    }
    setBackendObject(0);
}

void Qube::Hardware::DevicePrivate::_q_destroyed(QObject *object)
{
    Q_UNUSED(object);
    setBackendObject(0);
}

void Qube::Hardware::DevicePrivate::setBackendObject(Ifaces::Device *object)
{

    if (m_backendObject) {
        m_backendObject.data()->disconnect(this);
    }

    delete m_backendObject.data();
    m_backendObject = object;

    if (object) {
        connect(object, SIGNAL(destroyed(QObject *)),
                this, SLOT(_q_destroyed(QObject *)));
    }

    if (!m_ifaces.isEmpty()) {
        foreach (DeviceInterface *iface, m_ifaces) {
            delete iface;
        }

        m_ifaces.clear();
        if (!ref.deref()) deleteLater();
    }
}

Qube::Hardware::DeviceInterface *Qube::Hardware::DevicePrivate::interface(const DeviceInterface::Type &type) const
{
    return m_ifaces[type];
}

void Qube::Hardware::DevicePrivate::setInterface(const DeviceInterface::Type &type, DeviceInterface *interface)
{
    if(m_ifaces.isEmpty())
        ref.ref();
    m_ifaces[type] = interface;
}
