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

#include "storagedrive.h"
#include "storagedrive_p.h"

#include "soliddefs_p.h"
#include <ifaces/storagedrive.h>
#include "predicate.h"
#include "storageaccess.h"
#include "device.h"
#include "device_p.h"

QubeHardware::StorageDrive::StorageDrive(QObject *backendObject)
    : DeviceInterface(*new StorageDrivePrivate(), backendObject)
{
}

QubeHardware::StorageDrive::StorageDrive(StorageDrivePrivate &dd, QObject *backendObject)
    : DeviceInterface(dd, backendObject)
{

}

QubeHardware::StorageDrive::~StorageDrive()
{

}

QubeHardware::StorageDrive::Bus QubeHardware::StorageDrive::bus() const
{
    Q_D(const StorageDrive);
    return_SOLID_CALL(Ifaces::StorageDrive *, d->backendObject(), Platform, bus());
}

QubeHardware::StorageDrive::DriveType QubeHardware::StorageDrive::driveType() const
{
    Q_D(const StorageDrive);
    return_SOLID_CALL(Ifaces::StorageDrive *, d->backendObject(), HardDisk, driveType());
}

bool QubeHardware::StorageDrive::isRemovable() const
{
    Q_D(const StorageDrive);
    return_SOLID_CALL(Ifaces::StorageDrive *, d->backendObject(), false, isRemovable());
}

bool QubeHardware::StorageDrive::isHotpluggable() const
{
    Q_D(const StorageDrive);
    return_SOLID_CALL(Ifaces::StorageDrive *, d->backendObject(), false, isHotpluggable());
}

qulonglong QubeHardware::StorageDrive::size() const
{
    Q_D(const StorageDrive);
    return_SOLID_CALL(Ifaces::StorageDrive *, d->backendObject(), false, size());
}

bool QubeHardware::StorageDrive::isInUse() const
{
    Q_D(const StorageDrive);
    Predicate p(DeviceInterface::StorageAccess);
    QList<Device> devices = Device::listFromQuery(p, d->devicePrivate()->udi());

    bool inUse = false;
    foreach (const Device &dev, devices)	{
        if (dev.is<QubeHardware::StorageAccess>()) {
            const QubeHardware::StorageAccess* access = dev.as<QubeHardware::StorageAccess>();
            inUse |= (access->isAccessible());
        }
    }
    return inUse;
}
