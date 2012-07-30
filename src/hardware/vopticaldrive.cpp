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

#include "vopticaldrive.h"
#include "vopticaldrive_p.h"
#include "vcall_p.h"

#include <ifaces/opticaldrive.h>

VOpticalDrive::VOpticalDrive(QObject *backendObject)
    : VStorageDrive(*new VOpticalDrivePrivate(), backendObject)
{
    connect(backendObject, SIGNAL(ejectPressed(const QString &)),
            this, SIGNAL(ejectPressed(const QString &)));
    connect(backendObject, SIGNAL(ejectDone(VHardware::ErrorType, QVariant, const QString &)),
            this, SIGNAL(ejectDone(VHardware::ErrorType, QVariant, const QString &)));
    connect(backendObject, SIGNAL(ejectRequested(const QString &)),
            this, SIGNAL(ejectRequested(const QString &)));
}

VOpticalDrive::~VOpticalDrive()
{
}

VOpticalDrive::MediumTypes VOpticalDrive::supportedMedia() const
{
    Q_D(const VOpticalDrive);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::OpticalDrive *, d->backendObject(), MediumTypes(), supportedMedia());
}

int VOpticalDrive::readSpeed() const
{
    Q_D(const VOpticalDrive);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::OpticalDrive *, d->backendObject(), 0, readSpeed());
}

int VOpticalDrive::writeSpeed() const
{
    Q_D(const VOpticalDrive);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::OpticalDrive *, d->backendObject(), 0, writeSpeed());
}

QList<int> VOpticalDrive::writeSpeeds() const
{
    Q_D(const VOpticalDrive);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::OpticalDrive *, d->backendObject(), QList<int>(), writeSpeeds());
}

bool VOpticalDrive::eject()
{
    Q_D(VOpticalDrive);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::OpticalDrive *, d->backendObject(), false, eject());
}

#include "moc_vopticaldrive.cpp"
