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

#include "vbattery.h"
#include "vbattery_p.h"
#include "vcall_p.h"

#include <ifaces/battery.h>

VBattery::VBattery(QObject *backendObject)
    : VDeviceInterface(*new VBatteryPrivate(), backendObject)
{
    connect(backendObject, SIGNAL(chargePercentChanged(int, const QString &)),
            this, SIGNAL(chargePercentChanged(int, const QString &)));

    connect(backendObject, SIGNAL(chargeStateChanged(int, const QString &)),
            this, SIGNAL(chargeStateChanged(int, const QString &)));

    connect(backendObject, SIGNAL(plugStateChanged(bool, const QString &)),
            this, SIGNAL(plugStateChanged(bool, const QString &)));
}

VBattery::~VBattery()
{
}

bool VBattery::isPlugged() const
{
    Q_D(const VBattery);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::Battery *, d->backendObject(), false, isPlugged());
}

VBattery::BatteryType VBattery::type() const
{
    Q_D(const VBattery);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::Battery *, d->backendObject(), UnknownBattery, type());
}

int VBattery::chargePercent() const
{
    Q_D(const VBattery);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::Battery *, d->backendObject(), 0, chargePercent());
}

bool VBattery::isRechargeable() const
{
    Q_D(const VBattery);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::Battery *, d->backendObject(), false, isRechargeable());
}

VBattery::ChargeState VBattery::chargeState() const
{
    Q_D(const VBattery);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::Battery *, d->backendObject(), NoCharge, chargeState());
}

#include "vbattery.moc"
