/*
    Copyright 2006-2007 Kevin Ottens <ervin@kde.org>

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

#include "vstoragedrive.h"
#include "vstoragedrive_p.h"

#include "vcall_p.h"
#include <ifaces/storagedrive.h>
#include "vpredicate.h"
#include "vstorageaccess.h"
#include "vdevice.h"
#include "vdevice_p.h"

VStorageDrive::VStorageDrive(QObject *backendObject)
    : VDeviceInterface(*new VStorageDrivePrivate(), backendObject)
{
}

VStorageDrive::VStorageDrive(VStorageDrivePrivate &dd, QObject *backendObject)
    : VDeviceInterface(dd, backendObject)
{

}

VStorageDrive::~VStorageDrive()
{

}

VStorageDrive::Bus VStorageDrive::bus() const
{
    Q_D(const VStorageDrive);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::StorageDrive *, d->backendObject(), Platform, bus());
}

VStorageDrive::DriveType VStorageDrive::driveType() const
{
    Q_D(const VStorageDrive);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::StorageDrive *, d->backendObject(), HardDisk, driveType());
}

bool VStorageDrive::isRemovable() const
{
    Q_D(const VStorageDrive);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::StorageDrive *, d->backendObject(), false, isRemovable());
}

bool VStorageDrive::isHotpluggable() const
{
    Q_D(const VStorageDrive);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::StorageDrive *, d->backendObject(), false, isHotpluggable());
}

qulonglong VStorageDrive::size() const
{
    Q_D(const VStorageDrive);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::StorageDrive *, d->backendObject(), false, size());
}

bool VStorageDrive::isInUse() const
{
    Q_D(const VStorageDrive);
    VPredicate p(VDeviceInterface::StorageAccess);
    QList<VDevice> devices = VDevice::listFromQuery(p, d->devicePrivate()->udi());

    bool inUse = false;
    foreach(const VDevice & dev, devices) {
        if (dev.is<VStorageAccess>()) {
            const VStorageAccess *access = dev.as<VStorageAccess>();
            inUse |= (access->isAccessible());
        }
    }
    return inUse;
}

#include "vstoragedrive.moc"
