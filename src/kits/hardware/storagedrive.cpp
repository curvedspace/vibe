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

#include "call_p.h"
#include <ifaces/storagedrive.h>
#include "predicate.h"
#include "storageaccess.h"
#include "device.h"
#include "device_p.h"

Qube::Hardware::StorageDrive::StorageDrive(QObject *backendObject)
    : DeviceInterface(*new StorageDrivePrivate(), backendObject)
{
}

Qube::Hardware::StorageDrive::StorageDrive(StorageDrivePrivate &dd, QObject *backendObject)
    : DeviceInterface(dd, backendObject)
{

}

Qube::Hardware::StorageDrive::~StorageDrive()
{

}

Qube::Hardware::StorageDrive::Bus Qube::Hardware::StorageDrive::bus() const
{
    Q_D(const StorageDrive);
    return_QUBE_HARDWARE_CALL(Ifaces::StorageDrive *, d->backendObject(), Platform, bus());
}

Qube::Hardware::StorageDrive::DriveType Qube::Hardware::StorageDrive::driveType() const
{
    Q_D(const StorageDrive);
    return_QUBE_HARDWARE_CALL(Ifaces::StorageDrive *, d->backendObject(), HardDisk, driveType());
}

bool Qube::Hardware::StorageDrive::isRemovable() const
{
    Q_D(const StorageDrive);
    return_QUBE_HARDWARE_CALL(Ifaces::StorageDrive *, d->backendObject(), false, isRemovable());
}

bool Qube::Hardware::StorageDrive::isHotpluggable() const
{
    Q_D(const StorageDrive);
    return_QUBE_HARDWARE_CALL(Ifaces::StorageDrive *, d->backendObject(), false, isHotpluggable());
}

qulonglong Qube::Hardware::StorageDrive::size() const
{
    Q_D(const StorageDrive);
    return_QUBE_HARDWARE_CALL(Ifaces::StorageDrive *, d->backendObject(), false, size());
}

bool Qube::Hardware::StorageDrive::isInUse() const
{
    Q_D(const StorageDrive);
    Predicate p(DeviceInterface::StorageAccess);
    QList<Device> devices = Device::listFromQuery(p, d->devicePrivate()->udi());

    bool inUse = false;
    foreach (const Device &dev, devices)	{
        if (dev.is<Qube::Hardware::StorageAccess>()) {
            const Qube::Hardware::StorageAccess* access = dev.as<Qube::Hardware::StorageAccess>();
            inUse |= (access->isAccessible());
        }
    }
    return inUse;
}

#include "storagedrive.moc"
