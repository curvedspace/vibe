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

#include "vstoragevolume.h"
#include "vstoragevolume_p.h"
#include "vcall_p.h"

#include <ifaces/storagevolume.h>
#include <VibeHardware/VDevice>

VStorageVolume::VStorageVolume(QObject *backendObject)
    : VDeviceInterface(*new VStorageVolumePrivate(), backendObject)
{
}

VStorageVolume::VStorageVolume(VStorageVolumePrivate &dd, QObject *backendObject)
    : VDeviceInterface(dd, backendObject)
{
}

VStorageVolume::~VStorageVolume()
{
}

bool VStorageVolume::isIgnored() const
{
    Q_D(const VStorageVolume);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::StorageVolume *, d->backendObject(), true, isIgnored());
}

VStorageVolume::UsageType VStorageVolume::usage() const
{
    Q_D(const VStorageVolume);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::StorageVolume *, d->backendObject(), Unused, usage());
}

QString VStorageVolume::fsType() const
{
    Q_D(const VStorageVolume);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::StorageVolume *, d->backendObject(), QString(), fsType());
}

QString VStorageVolume::label() const
{
    Q_D(const VStorageVolume);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::StorageVolume *, d->backendObject(), QString(), label());
}

QString VStorageVolume::uuid() const
{
    Q_D(const VStorageVolume);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::StorageVolume *, d->backendObject(), QString(), uuid().toLower());
}

qulonglong VStorageVolume::size() const
{
    Q_D(const VStorageVolume);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::StorageVolume *, d->backendObject(), 0, size());
}

VDevice VStorageVolume::encryptedContainer() const
{
    Q_D(const VStorageVolume);

    VHardware::Ifaces::StorageVolume *iface =
        qobject_cast<VHardware::Ifaces::StorageVolume *>(d->backendObject());

    if (iface != 0)
        return VDevice(iface->encryptedContainerUdi());
    else
        return VDevice();
}

#include "moc_vstoragevolume.cpp"
