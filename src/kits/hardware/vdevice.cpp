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

#include "vdevice.h"
#include "vdevice_p.h"
#include "vdevicenotifier.h"
#include "vdevicemanager_p.h"

#include "vdeviceinterface_p.h"
#include "vcall_p.h"

#include <ifaces/device.h>

#include <VibeHardware/VGenericInterface>
#include <ifaces/genericinterface.h>
#include <VibeHardware/VProcessor>
#include <ifaces/processor.h>
#include <VibeHardware/VBlock>
#include <ifaces/block.h>
#include <VibeHardware/VStorageAccess>
#include <ifaces/storageaccess.h>
#include <VibeHardware/VStorageDrive>
#include <ifaces/storagedrive.h>
#include <VibeHardware/VOpticalDrive>
#include <ifaces/opticaldrive.h>
#include <VibeHardware/VStorageVolume>
#include <ifaces/storagevolume.h>
#include <VibeHardware/VOpticalDisc>
#include <ifaces/opticaldisc.h>
#include <VibeHardware/VCamera>
#include <ifaces/camera.h>
#include <VibeHardware/VPortableMediaPlayer>
#include <ifaces/portablemediaplayer.h>
#include <VibeHardware/VNetworkInterface>
#include <ifaces/networkinterface.h>
#include <VibeHardware/VNetworkShare>
#include <ifaces/networkshare.h>
#include <VibeHardware/VAcAdapter>
#include <ifaces/acadapter.h>
#include <VibeHardware/VBattery>
#include <ifaces/battery.h>
#include <VibeHardware/VButton>
#include <ifaces/button.h>
#include <VibeHardware/VAudioInterface>
#include <ifaces/audiointerface.h>
#include <VibeHardware/VDvbInterface>
#include <ifaces/dvbinterface.h>
#include <VibeHardware/VVideo>
#include <ifaces/video.h>
#include <VibeHardware/VSerialInterface>
#include <ifaces/serialinterface.h>
#include <VibeHardware/VSmartCardReader>
#include <ifaces/smartcardreader.h>
#include <VibeHardware/VInternetGateway>
#include <ifaces/internetgateway.h>

VDevice::VDevice(const QString &udi)
{
    VDeviceManagerPrivate *manager =
        static_cast<VDeviceManagerPrivate *>(VDeviceNotifier::instance());
    d = manager->findRegisteredDevice(udi);
}

VDevice::VDevice(const VDevice &device)
    : d(device.d)
{
}

VDevice::~VDevice()
{
}

VDevice &VDevice::operator=(const VDevice &device)
{
    d = device.d;
    return *this;
}

bool VDevice::isValid() const
{
    return d->backendObject() != 0;
}

QString VDevice::udi() const
{
    return d->udi();
}

QString VDevice::parentUdi() const
{
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::Device *, d->backendObject(), QString(), parentUdi());
}

VDevice VDevice::parent() const
{
    QString udi = parentUdi();

    if (udi.isEmpty()) {
        return VDevice();
    } else {
        return VDevice(udi);
    }
}

QString VDevice::vendor() const
{
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::Device *, d->backendObject(), QString(), vendor());
}

QString VDevice::product() const
{
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::Device *, d->backendObject(), QString(), product());
}

QString VDevice::icon() const
{
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::Device *, d->backendObject(), QString(), icon());
}

QStringList VDevice::emblems() const
{
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::Device *, d->backendObject(), QStringList(), emblems());
}

QString VDevice::description() const
{
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::Device *, d->backendObject(), QString(), description());
}

bool VDevice::isDeviceInterface(const VDeviceInterface::Type &type) const
{
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::Device *, d->backendObject(), false, queryDeviceInterface(type));
}

#define deviceinterface_cast(IfaceType, DevType, backendObject) \
    (qobject_cast<IfaceType *>(backendObject) ? new DevType(backendObject) : 0)

VDeviceInterface *VDevice::asDeviceInterface(const VDeviceInterface::Type &type)
{
    const VDeviceInterface *interface = const_cast<const VDevice *>(this)->asDeviceInterface(type);
    return const_cast<VDeviceInterface *>(interface);
}

const VDeviceInterface *VDevice::asDeviceInterface(const VDeviceInterface::Type &type) const
{
    VHardware::Ifaces::Device *device = qobject_cast<VHardware::Ifaces::Device *>(d->backendObject());

    if (device != 0) {
        VDeviceInterface *iface = d->interface(type);

        if (iface != 0) {
            return iface;
        }

        QObject *dev_iface = device->createDeviceInterface(type);

        if (dev_iface != 0) {
            switch (type) {
                case VDeviceInterface::GenericInterface:
                    iface = deviceinterface_cast(VHardware::Ifaces::GenericInterface, VGenericInterface, dev_iface);
                    break;
                case VDeviceInterface::Processor:
                    iface = deviceinterface_cast(VHardware::Ifaces::Processor, VProcessor, dev_iface);
                    break;
                case VDeviceInterface::Block:
                    iface = deviceinterface_cast(VHardware::Ifaces::Block, VBlock, dev_iface);
                    break;
                case VDeviceInterface::StorageAccess:
                    iface = deviceinterface_cast(VHardware::Ifaces::StorageAccess, VStorageAccess, dev_iface);
                    break;
                case VDeviceInterface::StorageDrive:
                    iface = deviceinterface_cast(VHardware::Ifaces::StorageDrive, VStorageDrive, dev_iface);
                    break;
                case VDeviceInterface::OpticalDrive:
                    iface = deviceinterface_cast(VHardware::Ifaces::OpticalDrive, VOpticalDrive, dev_iface);
                    break;
                case VDeviceInterface::StorageVolume:
                    iface = deviceinterface_cast(VHardware::Ifaces::StorageVolume, VStorageVolume, dev_iface);
                    break;
                case VDeviceInterface::OpticalDisc:
                    iface = deviceinterface_cast(VHardware::Ifaces::OpticalDisc, VOpticalDisc, dev_iface);
                    break;
                case VDeviceInterface::Camera:
                    iface = deviceinterface_cast(VHardware::Ifaces::Camera, VCamera, dev_iface);
                    break;
                case VDeviceInterface::PortableMediaPlayer:
                    iface = deviceinterface_cast(VHardware::Ifaces::PortableMediaPlayer, VPortableMediaPlayer, dev_iface);
                    break;
                case VDeviceInterface::NetworkInterface:
                    iface = deviceinterface_cast(VHardware::Ifaces::NetworkInterface, VNetworkInterface, dev_iface);
                    break;
                case VDeviceInterface::AcAdapter:
                    iface = deviceinterface_cast(VHardware::Ifaces::AcAdapter, VAcAdapter, dev_iface);
                    break;
                case VDeviceInterface::Battery:
                    iface = deviceinterface_cast(VHardware::Ifaces::Battery, VBattery, dev_iface);
                    break;
                case VDeviceInterface::Button:
                    iface = deviceinterface_cast(VHardware::Ifaces::Button, VButton, dev_iface);
                    break;
                case VDeviceInterface::AudioInterface:
                    iface = deviceinterface_cast(VHardware::Ifaces::AudioInterface, VAudioInterface, dev_iface);
                    break;
                case VDeviceInterface::DvbInterface:
                    iface = deviceinterface_cast(VHardware::Ifaces::DvbInterface, VDvbInterface, dev_iface);
                    break;
                case VDeviceInterface::Video:
                    iface = deviceinterface_cast(VHardware::Ifaces::Video, VVideo, dev_iface);
                    break;
                case VDeviceInterface::SerialInterface:
                    iface = deviceinterface_cast(VHardware::Ifaces::SerialInterface, VSerialInterface, dev_iface);
                    break;
                case VDeviceInterface::SmartCardReader:
                    iface = deviceinterface_cast(VHardware::Ifaces::SmartCardReader, VSmartCardReader, dev_iface);
                    break;
                case VDeviceInterface::InternetGateway:
                    iface = deviceinterface_cast(VHardware::Ifaces::InternetGateway, VInternetGateway, dev_iface);
                    break;
                case VDeviceInterface::NetworkShare:
                    iface = deviceinterface_cast(VHardware::Ifaces::NetworkShare, VNetworkShare, dev_iface);
                    break;
                case VDeviceInterface::Unknown:
                case VDeviceInterface::Last:
                    break;
            }
        }

        if (iface != 0) {
            // Lie on the constness since we're simply doing caching here
            const_cast<VDevice *>(this)->d->setInterface(type, iface);
            iface->d_ptr->setDevicePrivate(d.data());
        }

        return iface;
    } else {
        return 0;
    }
}

//////////////////////////////////////////////////////////////////////

VDevicePrivate::VDevicePrivate(const QString &udi) :
    QObject(),
    QSharedData(),
    m_udi(udi)
{
}

VDevicePrivate::~VDevicePrivate()
{
    foreach(VDeviceInterface * iface, m_ifaces)
    delete iface->d_ptr->backendObject();
    setBackendObject(0);
}

void VDevicePrivate::_q_destroyed(QObject *object)
{
    Q_UNUSED(object);
    setBackendObject(0);
}

void VDevicePrivate::setBackendObject(VHardware::Ifaces::Device *object)
{
    if (m_backendObject)
        m_backendObject.data()->disconnect(this);

    delete m_backendObject.data();
    m_backendObject = object;

    if (object)
        connect(object, SIGNAL(destroyed(QObject *)),
                this, SLOT(_q_destroyed(QObject *)));

    if (!m_ifaces.isEmpty()) {
        foreach(VDeviceInterface * iface, m_ifaces)
        delete iface;

        m_ifaces.clear();
        if (!ref.deref())
            deleteLater();
    }
}

VDeviceInterface *VDevicePrivate::interface(const VDeviceInterface::Type &type) const
{
    return m_ifaces[type];
}

void VDevicePrivate::setInterface(const VDeviceInterface::Type &type, VDeviceInterface *interface)
{
    if (m_ifaces.isEmpty())
        ref.ref();
    m_ifaces[type] = interface;
}

#include "vdevice_p.moc"
