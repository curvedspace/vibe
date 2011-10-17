/*
    Copyright 2009 Harald Fernengel <harry@kdevelop.org>

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

#include "serialinterface.h"
#include "serialinterface_p.h"

#include <QtCore/qvariant.h>

#include "soliddefs_p.h"
#include <ifaces/serialinterface.h>

QubeHardware::SerialInterface::SerialInterface(QObject *backendObject)
    : DeviceInterface(*new SerialInterfacePrivate(), backendObject)
{
}

QubeHardware::SerialInterface::~SerialInterface()
{

}

QVariant QubeHardware::SerialInterface::driverHandle() const
{
    Q_D(const SerialInterface);
    return_SOLID_CALL(Ifaces::SerialInterface *, d->backendObject(), QVariant(), driverHandle());
}

QubeHardware::SerialInterface::SerialType QubeHardware::SerialInterface::serialType() const
{
    Q_D(const SerialInterface);
    return_SOLID_CALL(Ifaces::SerialInterface *, d->backendObject(), Unknown, serialType());
}

int QubeHardware::SerialInterface::port() const
{
    Q_D(const SerialInterface);
    return_SOLID_CALL(Ifaces::SerialInterface *, d->backendObject(), -1, port());
}
