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

#include "battery.h"
#include "battery_p.h"

#include "call_p.h"
#include <ifaces/battery.h>

Qube::Hardware::Battery::Battery(QObject *backendObject)
    : DeviceInterface(*new BatteryPrivate(), backendObject)
{
    connect(backendObject, SIGNAL(chargePercentChanged(int, const QString &)),
            this, SIGNAL(chargePercentChanged(int, const QString &)));

    connect(backendObject, SIGNAL(chargeStateChanged(int, const QString &)),
            this, SIGNAL(chargeStateChanged(int, const QString &)));

    connect(backendObject, SIGNAL(plugStateChanged(bool, const QString &)),
            this, SIGNAL(plugStateChanged(bool, const QString &)));
}

Qube::Hardware::Battery::~Battery()
{

}

bool Qube::Hardware::Battery::isPlugged() const
{
    Q_D(const Battery);
    return_QUBE_HARDWARE_CALL(Ifaces::Battery *, d->backendObject(), false, isPlugged());
}

Qube::Hardware::Battery::BatteryType Qube::Hardware::Battery::type() const
{
    Q_D(const Battery);
    return_QUBE_HARDWARE_CALL(Ifaces::Battery *, d->backendObject(), UnknownBattery, type());
}

int Qube::Hardware::Battery::chargePercent() const
{
    Q_D(const Battery);
    return_QUBE_HARDWARE_CALL(Ifaces::Battery *, d->backendObject(), 0, chargePercent());
}

bool Qube::Hardware::Battery::isRechargeable() const
{
    Q_D(const Battery);
    return_QUBE_HARDWARE_CALL(Ifaces::Battery *, d->backendObject(), false, isRechargeable());
}

Qube::Hardware::Battery::ChargeState Qube::Hardware::Battery::chargeState() const
{
    Q_D(const Battery);
    return_QUBE_HARDWARE_CALL(Ifaces::Battery *, d->backendObject(), NoCharge, chargeState());
}

#include "battery.moc"
