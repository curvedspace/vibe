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

#ifndef VHARDWARE_BACKENDS_UPOWER_BATTERY_H
#define VHARDWARE_BACKENDS_UPOWER_BATTERY_H

#include <ifaces/battery.h>
#include "upowerdeviceinterface.h"

namespace VHardware
{
    namespace Backends
    {
        namespace UPower
        {
            class Battery : public DeviceInterface, virtual public VHardware::Ifaces::Battery
            {
                Q_OBJECT
                Q_INTERFACES(VHardware::Ifaces::Battery)
            public:
                Battery(UPowerDevice *device);
                virtual ~Battery();

                virtual bool isPlugged() const;
                virtual VBattery::BatteryType type() const;

                virtual int chargePercent() const;

                virtual bool isRechargeable() const;
                virtual VBattery::ChargeState chargeState() const;

                // TODO report stuff like capacity, technology, time-to-full, time-to-empty, energy rates, vendor, etc.

            Q_SIGNALS:
                void chargePercentChanged(int value, const QString &udi);
                void chargeStateChanged(int newState, const QString &udi);
                void plugStateChanged(bool newState, const QString &udi);

            private Q_SLOTS:
                void slotChanged();

            private:
                void updateCache();

                bool m_isPlugged;
                int m_chargePercent;
                VBattery::ChargeState m_chargeState;
            };
        }
    }
}

#endif // VHARDWARE_BACKENDS_UPOWER_BATTERY_H