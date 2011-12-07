/*
    Copyright 2010 Michael Zanetti <mzanetti@kde.org>
    Copyright 2010 Lukas Tinkl <ltinkl@redhat.com>

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

#include "udisksstoragedrive.h"

#include <QtCore/QDebug>

using namespace VHardware::Backends::UDisks;

UDisksStorageDrive::UDisksStorageDrive(UDisksDevice *device)
    : Block(device)
{

}

UDisksStorageDrive::~UDisksStorageDrive()
{

}

qulonglong UDisksStorageDrive::size() const
{
    return m_device->prop("DeviceSize").toULongLong();
}

bool UDisksStorageDrive::isHotpluggable() const
{
    return m_device->prop("DriveCanDetach").toBool();
}

bool UDisksStorageDrive::isRemovable() const
{
    return m_device->prop("DeviceIsRemovable").toBool() ||
           !m_device->prop("DeviceIsSystemInternal").toBool();
}

VStorageDrive::DriveType UDisksStorageDrive::driveType() const
{
    const QStringList mediaTypes = m_device->prop("DriveMediaCompatibility").toStringList();
    bool isHardDisk = m_device->prop("DeviceIsSystemInternal").toBool();

    if (isHardDisk) {
        return VStorageDrive::HardDisk;
    } else if (!mediaTypes.filter("optical").isEmpty()) {     // optical disks
        return VStorageDrive::CdromDrive;
    } else if (mediaTypes.contains("floppy")) {
        return VStorageDrive::Floppy;
    }
#if 0 // TODO add to VibeHardware
    else if (mediaTypes.contains("floppy_jaz")) {
        return VStorageDrive::Jaz;
    } else if (mediaTypes.contains("floppy_zip")) {
        return VStorageDrive::Zip;
    }
#endif
    /*
    else if (type=="tape")      // FIXME: DK doesn't know about tapes
    {
        return VStorageDrive::Tape;
    }
    */
#if 0 // TODO add to VibeHardware
    else if (mediaTypes.contains("flash")) {
        return VStorageDrive::Flash;
    }
#endif
    else if (mediaTypes.contains("flash_cf")) {
        return VStorageDrive::CompactFlash;
    } else if (mediaTypes.contains("flash_ms")) {
        return VStorageDrive::MemoryStick;
    } else if (mediaTypes.contains("flash_sm")) {
        return VStorageDrive::SmartMedia;
    } else if (mediaTypes.contains("flash_sd") || mediaTypes.contains("flash_sdhc") || mediaTypes.contains("flash_mmc")) {
        return VStorageDrive::SdMmc;
    }
    // FIXME: DK doesn't know about xD cards either
    else {
        return VStorageDrive::HardDisk;
    }
}

VStorageDrive::Bus UDisksStorageDrive::bus() const
{
    const QString bus = m_device->prop("DriveConnectionInterface").toString();

    if (bus == "ata" || bus == "ata_parallel") {   // parallel (classical) ATA
        return VStorageDrive::Ide;
    } else if (bus == "usb") {
        return VStorageDrive::Usb;
    } else if (bus == "firewire") {
        return VStorageDrive::Ieee1394;
    } else if (bus == "scsi") {
        return VStorageDrive::Scsi;
    } else if (bus == "ata_serial" || bus == "ata_serial_esata") {   // serial ATA
        return VStorageDrive::Sata;
    }
#if 0  // TODO add these to VibeHardware
    else if (bus == "sdio") {
        return VStorageDrive::SDIO;
    } else if (bus == "virtual") {
        return VStorageDrive::Virtual;
    }
#endif
    else
        return VStorageDrive::Platform;
}

