/*
    Copyright 2009 Pino Toscano <pino@kde.org>
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

#include "upowerbattery.h"

using namespace QubeHardware::Backends::UPower;

Battery::Battery(UPowerDevice *device)
    : DeviceInterface(device)
{
    connect(device, SIGNAL(changed()), this, SLOT(slotChanged()));

    updateCache();
}

Battery::~Battery()
{
}

bool Battery::isPlugged() const
{
    return m_device->prop("IsPresent").toBool();
}

QubeHardware::Battery::BatteryType Battery::type() const
{
    QubeHardware::Battery::BatteryType result = QubeHardware::Battery::UnknownBattery;
    const uint t = m_device->prop("Type").toUInt();
    switch (t) {
    case 1: // TODO "Line Power"
        break;
    case 2:
        result = QubeHardware::Battery::PrimaryBattery;
        break;
    case 3:
        result = QubeHardware::Battery::UpsBattery;
        break;
    case 4:
        result = QubeHardware::Battery::MonitorBattery;
        break;
    case 5:
        result = QubeHardware::Battery::MouseBattery;
        break;
    case 6:
        result = QubeHardware::Battery::KeyboardBattery;
        break;
    case 7:
        result = QubeHardware::Battery::PdaBattery;
        break;
    case 8:
        result = QubeHardware::Battery::PhoneBattery;
        break;
    }
    return result;
}

int Battery::chargePercent() const
{
    return qRound(m_device->prop("Percentage").toDouble());
}

bool Battery::isRechargeable() const
{
    return m_device->prop("IsRechargeable").toBool();
}

QubeHardware::Battery::ChargeState Battery::chargeState() const
{
    QubeHardware::Battery::ChargeState result = QubeHardware::Battery::NoCharge;
    const uint state = m_device->prop("State").toUInt();
    switch (state) {
    case 0:
        result = QubeHardware::Battery::NoCharge; // stable or unknown
        break;
    case 1:
        result = QubeHardware::Battery::Charging;
        break;
    case 2:
        result = QubeHardware::Battery::Discharging;
        break;
    case 3: // TODO "Empty"
        break;
    case 4: // TODO "Fully charged"
        break;
    case 5: // TODO "Pending charge"
        break;
    case 6: // TODO "Pending discharge"
        break;
    }
    return result;
}

void Battery::slotChanged()
{
    if (!m_device)
        return;

    const bool old_isPlugged = m_isPlugged;
    const int old_chargePercent = m_chargePercent;
    const QubeHardware::Battery::ChargeState old_chargeState = m_chargeState;
    updateCache();

    if (old_chargePercent != m_chargePercent) {
        emit chargePercentChanged(m_chargePercent, m_device->udi());
    }

    if (old_chargeState != m_chargeState) {
        emit chargeStateChanged(m_chargeState, m_device->udi());
    }

    if (old_isPlugged != m_isPlugged) {
        emit plugStateChanged(m_isPlugged, m_device->udi());
    }
}

void Battery::updateCache()
{
    m_isPlugged = isPlugged();
    m_chargePercent = chargePercent();
    m_chargeState = chargeState();
}
