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

#include "opticaldrive.h"
#include "opticaldrive_p.h"

#include "soliddefs_p.h"
#include <ifaces/opticaldrive.h>

QubeHardware::OpticalDrive::OpticalDrive(QObject *backendObject)
    : StorageDrive(*new OpticalDrivePrivate(), backendObject)
{
    connect(backendObject, SIGNAL(ejectPressed(const QString &)),
            this, SIGNAL(ejectPressed(const QString &)));
    connect(backendObject, SIGNAL(ejectDone(QubeHardware::ErrorType, QVariant, const QString &)),
            this, SIGNAL(ejectDone(QubeHardware::ErrorType, QVariant, const QString &)));
    connect(backendObject, SIGNAL(ejectRequested(const QString &)),
            this, SIGNAL(ejectRequested(const QString &)));
}

QubeHardware::OpticalDrive::~OpticalDrive()
{

}

QubeHardware::OpticalDrive::MediumTypes QubeHardware::OpticalDrive::supportedMedia() const
{
    Q_D(const OpticalDrive);
    return_SOLID_CALL(Ifaces::OpticalDrive *, d->backendObject(), MediumTypes(), supportedMedia());
}

int QubeHardware::OpticalDrive::readSpeed() const
{
    Q_D(const OpticalDrive);
    return_SOLID_CALL(Ifaces::OpticalDrive *, d->backendObject(), 0, readSpeed());
}

int QubeHardware::OpticalDrive::writeSpeed() const
{
    Q_D(const OpticalDrive);
    return_SOLID_CALL(Ifaces::OpticalDrive *, d->backendObject(), 0, writeSpeed());
}

QList<int> QubeHardware::OpticalDrive::writeSpeeds() const
{
    Q_D(const OpticalDrive);
    return_SOLID_CALL(Ifaces::OpticalDrive *, d->backendObject(), QList<int>(), writeSpeeds());
}

bool QubeHardware::OpticalDrive::eject()
{
    Q_D(OpticalDrive);
    return_SOLID_CALL(Ifaces::OpticalDrive *, d->backendObject(), false, eject());
}
