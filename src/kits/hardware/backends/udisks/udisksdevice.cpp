/*
    Copyright 2010 Michael Zanetti <mzanetti@kde.org>
    Copyright 2010-2011 Lukas Tinkl <ltinkl@redhat.com>

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

#include "udisks.h"
#include "udisksdevice.h"
#include "udisksdeviceinterface.h"
#include "udisksstoragevolume.h"
#include "udisksopticaldisc.h"
#include "udisksopticaldrive.h"
#include "udisksstorageaccess.h"
#include "udisksgenericinterface.h"

#include <QtCore/QStringList>
#include <QtCore/QDebug>

#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusMetaType>
#include <QtDBus/QDBusPendingReply>

#include <VibeHardware/VGenericInterface>
#include <VibeHardware/VDeviceInterface>
#include <VibeHardware/VDevice>

using namespace VHardware::Backends::UDisks;

// Adapted from KLocale as VibeHardware needs to be Qt-only
static QString formatByteSize(double size)
{
    // Per IEC 60027-2

    // Binary prefixes
    //Tebi-byte             TiB             2^40    1,099,511,627,776 bytes
    //Gibi-byte             GiB             2^30    1,073,741,824 bytes
    //Mebi-byte             MiB             2^20    1,048,576 bytes
    //Kibi-byte             KiB             2^10    1,024 bytes

    QString s;
    // Gibi-byte
    if (size >= 1073741824.0) {
        size /= 1073741824.0;
        if (size > 1024)   // Tebi-byte
            s = QObject::tr("%1 TiB").arg(QLocale().toString(size / 1024.0, 'f', 1));
        else
            s = QObject::tr("%1 GiB").arg(QLocale().toString(size, 'f', 1));
    }
    // Mebi-byte
    else if (size >= 1048576.0) {
        size /= 1048576.0;
        s = QObject::tr("%1 MiB").arg(QLocale().toString(size, 'f', 1));
    }
    // Kibi-byte
    else if (size >= 1024.0) {
        size /= 1024.0;
        s = QObject::tr("%1 KiB").arg(QLocale().toString(size, 'f', 1));
    }
    // Just byte
    else if (size > 0) {
        s = QObject::tr("%1 B").arg(QLocale().toString(size, 'f', 1));
    }
    // Nothing
    else {
        s = QObject::tr("0 B");
    }
    return s;
}

UDisksDevice::UDisksDevice(const QString &udi) :
    VHardware::Ifaces::Device(),
    m_udi(udi)
{
    QString realUdi = m_udi;
    if (realUdi.endsWith(":media"))
        realUdi.chop(6);
    m_device = new QDBusInterface(UD_DBUS_SERVICE, realUdi,
                                  UD_DBUS_INTERFACE_DISKS_DEVICE,
                                  QDBusConnection::systemBus());

    if (m_device->isValid())
        connect(m_device, SIGNAL(Changed()), this, SLOT(slotChanged()));
}

UDisksDevice::~UDisksDevice()
{
    delete m_device;
}

QObject *UDisksDevice::createDeviceInterface(const VDeviceInterface::Type &type)
{
    if (!queryDeviceInterface(type)) {
        return 0;
    }

    DeviceInterface *iface = 0;
    switch (type) {
        case VDeviceInterface::GenericInterface:
            iface = new GenericInterface(this);
            break;
        case VDeviceInterface::Block:
            iface = new Block(this);
            break;
        case VDeviceInterface::StorageAccess:
            iface = new UDisksStorageAccess(this);
            break;
        case VDeviceInterface::StorageDrive:
            iface = new UDisksStorageDrive(this);
            break;
        case VDeviceInterface::OpticalDrive:
            iface = new UDisksOpticalDrive(this);
            break;
        case VDeviceInterface::StorageVolume:
            iface = new UDisksStorageVolume(this);
            break;
        case VDeviceInterface::OpticalDisc:
            iface = new OpticalDisc(this);
            break;
        default:
            break;
    }
    return iface;
}

bool UDisksDevice::queryDeviceInterface(const VDeviceInterface::Type &type) const
{
    switch (type) {
        case VDeviceInterface::GenericInterface:
            return true;
        case VDeviceInterface::Block:
            return prop("DeviceMajor").toInt() != -1;
        case VDeviceInterface::StorageVolume:
            if (prop("DeviceIsOpticalDisc").toBool()) {
                return m_udi.endsWith(":media");
            } else {
                return prop("DeviceIsPartition").toBool()
                       || prop("IdUsage").toString() == "filesystem";
            }

        case VDeviceInterface::StorageAccess:
            if (prop("DeviceIsOpticalDisc").toBool()) {
                return prop("IdUsage").toString() == "filesystem"
                       && m_udi.endsWith(":media");

            } else {
                return prop("IdUsage").toString() == "filesystem"
                       || prop("IdUsage").toString() == "crypto";
            }

        case VDeviceInterface::StorageDrive:
            return !m_udi.endsWith(":media") && prop("DeviceIsDrive").toBool();
        case VDeviceInterface::OpticalDrive:
            return !m_udi.endsWith(":media")
                   && prop("DeviceIsDrive").toBool()
                   && !prop("DriveMediaCompatibility").toStringList().filter("optical_").isEmpty();
        case VDeviceInterface::OpticalDisc:
            return m_udi.endsWith(":media") && prop("DeviceIsOpticalDisc").toBool();
        default:
            return false;
    }
}

QStringList UDisksDevice::emblems() const
{
    QStringList res;

    if (queryDeviceInterface(VDeviceInterface::StorageAccess)) {

        bool isEncrypted = false;
        if (queryDeviceInterface(VDeviceInterface::StorageVolume)) {
            const UDisks::UDisksStorageVolume volIface(const_cast<UDisksDevice *>(this));
            isEncrypted = (volIface.usage() == VStorageVolume::Encrypted);
        }

        const UDisks::UDisksStorageAccess accessIface(const_cast<UDisksDevice *>(this));
        if (accessIface.isAccessible()) {
            if (isEncrypted)
                res << "emblem-encrypted-unlocked";
            else
                res << "emblem-mounted";
        } else {
            if (isEncrypted)
                res << "emblem-encrypted-locked";
            else
                res << "emblem-unmounted";
        }
    }

    return res;
}

QString UDisksDevice::description() const
{
    if (queryDeviceInterface(VDeviceInterface::StorageDrive))
        return storageDescription();
    else if (queryDeviceInterface(VDeviceInterface::StorageVolume))
        return volumeDescription();
    else
        return product();
}

QString UDisksDevice::storageDescription() const
{
    QString description;
    const UDisks::UDisksStorageDrive storageDrive(const_cast<UDisksDevice *>(this));
    VStorageDrive::DriveType drive_type = storageDrive.driveType();
    bool drive_is_hotpluggable = storageDrive.isHotpluggable();
    const UDisks::UDisksStorageVolume storageVolume(const_cast<UDisksDevice *>(this));

    if (drive_type == VStorageDrive::CdromDrive) {
        const UDisks::UDisksOpticalDrive opticalDrive(const_cast<UDisksDevice *>(this));
        VOpticalDrive::MediumTypes mediumTypes = opticalDrive.supportedMedia();
        QString first;
        QString second;

        first = QObject::tr("CD-ROM", "First item of %1%2 Drive sentence");
        if (mediumTypes & VOpticalDrive::Cdr)
            first = QObject::tr("CD-R", "First item of %1%2 Drive sentence");
        if (mediumTypes & VOpticalDrive::Cdrw)
            first = QObject::tr("CD-RW", "First item of %1%2 Drive sentence");

        if (mediumTypes & VOpticalDrive::Dvd)
            second = QObject::tr("/DVD-ROM", "Second item of %1%2 Drive sentence");
        if (mediumTypes & VOpticalDrive::Dvdplusr)
            second = QObject::tr("/DVD+R", "Second item of %1%2 Drive sentence");
        if (mediumTypes & VOpticalDrive::Dvdplusrw)
            second = QObject::tr("/DVD+RW", "Second item of %1%2 Drive sentence");
        if (mediumTypes & VOpticalDrive::Dvdr)
            second = QObject::tr("/DVD-R", "Second item of %1%2 Drive sentence");
        if (mediumTypes & VOpticalDrive::Dvdrw)
            second = QObject::tr("/DVD-RW", "Second item of %1%2 Drive sentence");
        if (mediumTypes & VOpticalDrive::Dvdram)
            second = QObject::tr("/DVD-RAM", "Second item of %1%2 Drive sentence");
        if ((mediumTypes & VOpticalDrive::Dvdr) && (mediumTypes & VOpticalDrive::Dvdplusr)) {
            if (mediumTypes & VOpticalDrive::Dvdplusdl)
                second = QObject::tr("/DVD±R DL", "Second item of %1%2 Drive sentence");
            else
                second = QObject::tr("/DVD±R", "Second item of %1%2 Drive sentence");
        }
        if ((mediumTypes & VOpticalDrive::Dvdrw) && (mediumTypes & VOpticalDrive::Dvdplusrw)) {
            if ((mediumTypes & VOpticalDrive::Dvdplusdl) || (mediumTypes & VOpticalDrive::Dvdplusdlrw))
                second = QObject::tr("/DVD±RW DL", "Second item of %1%2 Drive sentence");
            else
                second = QObject::tr("/DVD±RW", "Second item of %1%2 Drive sentence");
        }
        if (mediumTypes & VOpticalDrive::Bd)
            second = QObject::tr("/BD-ROM", "Second item of %1%2 Drive sentence");
        if (mediumTypes & VOpticalDrive::Bdr)
            second = QObject::tr("/BD-R", "Second item of %1%2 Drive sentence");
        if (mediumTypes & VOpticalDrive::Bdre)
            second = QObject::tr("/BD-RE", "Second item of %1%2 Drive sentence");
        if (mediumTypes & VOpticalDrive::HdDvd)
            second = QObject::tr("/HD DVD-ROM", "Second item of %1%2 Drive sentence");
        if (mediumTypes & VOpticalDrive::HdDvdr)
            second = QObject::tr("/HD DVD-R", "Second item of %1%2 Drive sentence");
        if (mediumTypes & VOpticalDrive::HdDvdrw)
            second = QObject::tr("/HD DVD-RW", "Second item of %1%2 Drive sentence");

        if (drive_is_hotpluggable)
            description = QObject::tr("External %1%2 Drive", "%1 is CD-ROM/CD-R/etc; %2 is '/DVD-ROM'/'/DVD-R'/etc (with leading slash)").arg(first).arg(second);
        else
            description = QObject::tr("%1%2 Drive", "%1 is CD-ROM/CD-R/etc; %2 is '/DVD-ROM'/'/DVD-R'/etc (with leading slash)").arg(first).arg(second);

        return description;
    }

    if (drive_type == VStorageDrive::Floppy) {
        if (drive_is_hotpluggable)
            description = QObject::tr("External Floppy Drive");
        else
            description = QObject::tr("Floppy Drive");

        return description;
    }

    bool drive_is_removable = storageDrive.isRemovable();

    if (drive_type == VStorageDrive::HardDisk && !drive_is_removable) {
        QString size_str = formatByteSize(storageVolume.size());
        if (!size_str.isEmpty()) {
            if (drive_is_hotpluggable)
                description = QObject::tr("%1 External Hard Drive", "%1 is the size").arg(size_str);
            else
                description = QObject::tr("%1 Hard Drive", "%1 is the size").arg(size_str);
        } else {
            if (drive_is_hotpluggable)
                description = QObject::tr("External Hard Drive");
            else
                description = QObject::tr("Hard Drive");
        }

        return description;
    }

    QString vendormodel_str;
    QString model = product();
    QString vendor_str = vendor();

    if (vendor_str.isEmpty()) {
        if (!model.isEmpty())
            vendormodel_str = model;
    } else {
        if (model.isEmpty())
            vendormodel_str = vendor_str;
        else
            vendormodel_str = QObject::tr("%1 %2", "%1 is the vendor, %2 is the model of the device").arg(vendor_str).arg(model);
    }

    if (vendormodel_str.isEmpty())
        description = QObject::tr("Drive");
    else
        description = vendormodel_str;

    return description;
}

QString UDisksDevice::volumeDescription() const
{
    QString description;
    const UDisks::UDisksStorageVolume storageVolume(const_cast<UDisksDevice *>(this));
    QString volume_label = prop("IdLabel").toString();
    if (volume_label.isEmpty())
        volume_label = prop("PartitionLabel").toString();
    if (!volume_label.isEmpty())
        return volume_label;

    const UDisks::UDisksStorageDrive storageDrive(const_cast<UDisksDevice *>(this));
    VStorageDrive::DriveType drive_type = storageDrive.driveType();

    // Handle media in optical drives
    if (drive_type == VStorageDrive::CdromDrive) {
        const UDisks::OpticalDisc disc(const_cast<UDisksDevice *>(this));
        switch (disc.discType()) {
            case VOpticalDisc::UnknownDiscType:
            case VOpticalDisc::CdRom:
                description = QObject::tr("CD-ROM");
                break;

            case VOpticalDisc::CdRecordable:
                if (disc.isBlank())
                    description = QObject::tr("Blank CD-R");
                else
                    description = QObject::tr("CD-R");
                break;

            case VOpticalDisc::CdRewritable:
                if (disc.isBlank())
                    description = QObject::tr("Blank CD-RW");
                else
                    description = QObject::tr("CD-RW");
                break;

            case VOpticalDisc::DvdRom:
                description = QObject::tr("DVD-ROM");
                break;

            case VOpticalDisc::DvdRam:
                if (disc.isBlank())
                    description = QObject::tr("Blank DVD-RAM");
                else
                    description = QObject::tr("DVD-RAM");
                break;

            case VOpticalDisc::DvdRecordable:
                if (disc.isBlank())
                    description = QObject::tr("Blank DVD-R");
                else
                    description = QObject::tr("DVD-R");
                break;

            case VOpticalDisc::DvdPlusRecordableDuallayer:
                if (disc.isBlank())
                    description = QObject::tr("Blank DVD+R Dual-Layer");
                else
                    description = QObject::tr("DVD+R Dual-Layer");
                break;

            case VOpticalDisc::DvdRewritable:
                if (disc.isBlank())
                    description = QObject::tr("Blank DVD-RW");
                else
                    description = QObject::tr("DVD-RW");
                break;

            case VOpticalDisc::DvdPlusRecordable:
                if (disc.isBlank())
                    description = QObject::tr("Blank DVD+R");
                else
                    description = QObject::tr("DVD+R");
                break;

            case VOpticalDisc::DvdPlusRewritable:
                if (disc.isBlank())
                    description = QObject::tr("Blank DVD+RW");
                else
                    description = QObject::tr("DVD+RW");
                break;

            case VOpticalDisc::DvdPlusRewritableDuallayer:
                if (disc.isBlank())
                    description = QObject::tr("Blank DVD+RW Dual-Layer");
                else
                    description = QObject::tr("DVD+RW Dual-Layer");
                break;

            case VOpticalDisc::BluRayRom:
                description = QObject::tr("BD-ROM");
                break;

            case VOpticalDisc::BluRayRecordable:
                if (disc.isBlank())
                    description = QObject::tr("Blank BD-R");
                else
                    description = QObject::tr("BD-R");
                break;

            case VOpticalDisc::BluRayRewritable:
                if (disc.isBlank())
                    description = QObject::tr("Blank BD-RE");
                else
                    description = QObject::tr("BD-RE");
                break;

            case VOpticalDisc::HdDvdRom:
                description = QObject::tr("HD DVD-ROM");
                break;

            case VOpticalDisc::HdDvdRecordable:
                if (disc.isBlank())
                    description = QObject::tr("Blank HD DVD-R");
                else
                    description = QObject::tr("HD DVD-R");
                break;

            case VOpticalDisc::HdDvdRewritable:
                if (disc.isBlank())
                    description = QObject::tr("Blank HD DVD-RW");
                else
                    description = QObject::tr("HD DVD-RW");
                break;
        }

        // Special case for pure audio disc
        if (disc.availableContent() == VOpticalDisc::Audio)
            description = QObject::tr("Audio CD");

        return description;
    }

    bool drive_is_removable = storageDrive.isRemovable();
    bool drive_is_hotpluggable = storageDrive.isHotpluggable();
    bool drive_is_encrypted_container = (storageVolume.usage() == VStorageVolume::Encrypted);

    QString size_str = formatByteSize(storageVolume.size());
    if (drive_is_encrypted_container) {
        if (!size_str.isEmpty())
            description = QObject::tr("%1 Encrypted Container", "%1 is the size").arg(size_str);
        else
            description = QObject::tr("Encrypted Container");
    } else if (drive_type == VStorageDrive::HardDisk && !drive_is_removable) {
        if (!size_str.isEmpty()) {
            if (drive_is_hotpluggable)
                description = QObject::tr("%1 External Hard Drive", "%1 is the size").arg(size_str);
            else
                description = QObject::tr("%1 Hard Drive", "%1 is the size").arg(size_str);
        } else {
            if (drive_is_hotpluggable)
                description = QObject::tr("External Hard Drive");
            else
                description = QObject::tr("Hard Drive");
        }
    } else {
        if (drive_is_removable)
            description = QObject::tr("%1 Removable Media", "%1 is the size").arg(size_str);
        else
            description = QObject::tr("%1 Media", "%1 is the size").arg(size_str);
    }

    return description;
}

QString UDisksDevice::icon() const
{
    QString iconName = prop("DevicePresentationIconName").toString();

    if (!iconName.isEmpty()) {
        return iconName;
    } else {
        bool isPartition = prop("DeviceIsPartition").toBool();
        if (isPartition) {      // this is a slave device, we need to return its parent's icon
            UDisksDevice *parent = 0;
            if (!parentUdi().isEmpty())
                parent = new UDisksDevice(parentUdi());

            if (parent) {
                iconName = parent->icon();
                delete parent;
            }

            if (!iconName.isEmpty())
                return iconName;
        }

        // handle media
        const QString media = prop("DriveMedia").toString();
        bool isOptical = prop("DeviceIsOpticalDisc").toBool();

        if (!media.isEmpty()) {
            if (isOptical) {    // optical stuff
                bool isWritable = prop("OpticalDiscIsBlank").toBool() || prop("OpticalDiscIsAppendable").toBool();

                const UDisks::OpticalDisc disc(const_cast<UDisksDevice *>(this));
                VOpticalDisc::ContentTypes availContent = disc.availableContent();

                if (availContent & VOpticalDisc::VideoDvd) // Video DVD
                    return "media-optical-dvd-video";
                else if ((availContent & VOpticalDisc::VideoCd) || (availContent & VOpticalDisc::SuperVideoCd)) // Video CD
                    return "media-optical-video";
                else if ((availContent & VOpticalDisc::Data) && (availContent & VOpticalDisc::Audio)) // Mixed CD
                    return "media-optical-mixed-cd";
                else if (availContent & VOpticalDisc::Audio) // Audio CD
                    return "media-optical-audio";
                else if (availContent & VOpticalDisc::Data) // Data CD
                    return "media-optical-data";
                else if (isWritable)
                    return "media-optical-recordable";
                else {
                    if (media.startsWith("optical_dvd") || media.startsWith("optical_hddvd"))       // DVD
                        return "media-optical-dvd";
                    else if (media.startsWith("optical_bd"))     // BluRay
                        return "media-optical-blu-ray";
                }

                // fallback for every other optical disc
                return "media-optical";
            }

            if (media == "flash_ms")   // Flash & Co.
                return "media-flash-memory-stick";
            else if (media == "flash_sd" || media == "flash_sdhc" || media == "flash_mmc")
                return "media-flash-sd-mmc";
            else if (media == "flash_sm")
                return "media-flash-smart-media";
            else if (media.startsWith("flash"))
                return "media-flash";
            else if (media == "floppy")   // the good ol' floppy
                return "media-floppy";

        }

        // handle drives
        bool isRemovable = prop("DeviceIsRemovable").toBool();
        const QString conn = prop("DriveConnectionInterface").toString();

        if (queryDeviceInterface(VDeviceInterface::OpticalDrive))
            return "drive-optical";
        else if (isRemovable && !isOptical) {
            if (conn == "usb")
                return "drive-removable-media-usb";
            else
                return "drive-removable-media";
        }
    }

    return "drive-harddisk";    // general fallback
}

QString UDisksDevice::product() const
{
    QString product = prop("DriveModel").toString();
    const bool isDrive = prop("DeviceIsDrive").toBool() && !m_udi.endsWith(":media");

    if (!isDrive) {
        QString label = prop("IdLabel").toString();
        if (!label.isEmpty()) {
            product = label;
        }
    }

    return product;
}

QString UDisksDevice::vendor() const
{
    return prop("DriveVendor").toString();
}

QString UDisksDevice::udi() const
{
    return m_udi;
}

QString UDisksDevice::parentUdi() const
{
    if (m_udi.endsWith(QLatin1String(":media"))) {
        QString result = m_udi;
        return result.remove(":media");
    } else if (prop("DeviceIsLuksCleartext").toBool())
        return prop("LuksCleartextSlave").value<QDBusObjectPath>().path();
    else {
        QString parent = prop("PartitionSlave").value<QDBusObjectPath>().path();
        if (parent.isEmpty() || parent == "/") {
            parent = UD_UDI_DISKS_PREFIX;
        }
        return parent;
    }
}

void UDisksDevice::checkCache(const QString &key) const
{
    if (m_cache.isEmpty()) // recreate the cache
        allProperties();

    if (m_cache.contains(key))
        return;

    QVariant reply = m_device->property(key.toUtf8());

    if (reply.isValid()) {
        m_cache[key] = reply;
    } else {
        m_cache[key] = QVariant();
    }
}

QVariant UDisksDevice::prop(const QString &key) const
{
    checkCache(key);
    return m_cache.value(key);
}

bool UDisksDevice::propertyExists(const QString &key) const
{
    checkCache(key);
    return m_cache.contains(key);
}

QMap<QString, QVariant> UDisksDevice::allProperties() const
{
    QDBusMessage call = QDBusMessage::createMethodCall(m_device->service(), m_device->path(),
                                                       "org.freedesktop.DBus.Properties", "GetAll");
    QDBusPendingReply< QVariantMap > reply = QDBusConnection::systemBus().asyncCall(call);
    reply.waitForFinished();

    if (reply.isValid())
        m_cache = reply.value();
    else
        m_cache.clear();

    return m_cache;
}

void UDisksDevice::slotChanged()
{
    // given we cannot know which property/ies changed, clear the cache
    m_cache.clear();
    emit changed();
}

bool UDisksDevice::isDeviceBlacklisted() const
{
    return prop("DevicePresentationHide").toBool() || prop("DevicePresentationNopolicy").toBool() ||
           prop("DeviceMountPaths").toStringList().contains("/boot") ||
           prop("IdLabel").toString() == "System Reserved" ||
           (prop("IdUsage").toString().isEmpty() && !prop("OpticalDiscIsBlank").toBool());
}

QString UDisksDevice::errorToString(const QString &error) const
{
    if (error == UD_ERROR_UNAUTHORIZED)
        return QObject::tr("You are not authorized to perform this operation.");
    else if (error == UD_ERROR_BUSY)
        return QObject::tr("The device is currently busy.");
    else if (error == UD_ERROR_FAILED)
        return QObject::tr("The requested operation has failed.");
    else if (error == UD_ERROR_CANCELED)
        return QObject::tr("The requested operation has been canceled.");
    else if (error == UD_ERROR_INVALID_OPTION)
        return QObject::tr("An invalid or malformed option has been given.");
    else if (error == UD_ERROR_MISSING_DRIVER)
        return QObject::tr("The kernel driver for this filesystem type is not available.");
    else
        return QObject::tr("An unspecified error has occurred.");
}

VHardware::ErrorType UDisksDevice::errorToVibeHardwareError(const QString &error) const
{
    if (error == UD_ERROR_BUSY)
        return VHardware::DeviceBusy;
    else if (error == UD_ERROR_FAILED)
        return VHardware::OperationFailed;
    else if (error == UD_ERROR_CANCELED)
        return VHardware::UserCanceled;
    else if (error == UD_ERROR_INVALID_OPTION)
        return VHardware::InvalidOption;
    else if (error == UD_ERROR_MISSING_DRIVER)
        return VHardware::MissingDriver;
    else
        return VHardware::UnauthorizedOperation;
}