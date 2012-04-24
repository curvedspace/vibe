/*
    Copyright 2006 Kevin Ottens <ervin@kde.org>

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

#ifndef VHARDWARE_IFACES_BATTERY_H
#define VHARDWARE_IFACES_BATTERY_H

#include <VibeHardware/VBattery>
#include <ifaces/deviceinterface.h>

namespace VHardware
{
    namespace Ifaces
    {
        /**
         * This device interface is available on batteries.
         */
        class VIBE_EXPORT Battery : virtual public DeviceInterface
        {
        public:
            /**
             * Destroys a Battery object.
             */
            virtual ~Battery();

            /**
             * Indicates if this battery is plugged.
             *
             * @return true if the battery is plugged, false otherwise
             */
            virtual bool isPlugged() const = 0;

            /**
             * Retrieves the type of device holding this battery.
             *
             * @return the type of device holding this battery
             * @see VBattery::BatteryType
             */
            virtual VBattery::BatteryType type() const = 0;



            /**
             * Retrieves the current charge level of the battery normalised
             * to percent.
             *
             * @return the current charge level normalised to percent
             */
            virtual int chargePercent() const = 0;


            /**
             * Indicates if the battery is rechargeable.
             *
             * @return true if the battery is rechargeable, false otherwise (one time usage)
             */
            virtual bool isRechargeable() const = 0;

            /**
             * Retrieves the current charge state of the battery. It can be in a stable
             * state (no charge), charging or discharging.
             *
             * @return the current battery charge state
             * @see VBattery::ChargeState
             */
            virtual VBattery::ChargeState chargeState() const = 0;

        protected:
            //Q_SIGNALS:
            /**
             * This signal is emitted when the charge percent value of this
             * battery has changed.
             *
             * @param value the new charge percent value of the battery
             * @param udi the UDI of the battery with the new charge percent
             */
            virtual void chargePercentChanged(int value, const QString &udi) = 0;

            /**
             * This signal is emitted when the charge state of this battery
             * has changed.
             *
             * @param newState the new charge state of the battery, it's one of
             * the type VBattery::ChargeState
             * @see VBattery::ChargeState
             * @param udi the UDI of the battery with the new charge state
             */
            virtual void chargeStateChanged(int newState, const QString &udi) = 0;

            /**
             * This signal is emitted if the battery get plugged in/out of the
             * battery bay.
             *
             * @param newState the new plugging state of the battery, type is boolean
             * @param udi the UDI of the battery with thew new plugging state
             */
            virtual void plugStateChanged(bool newState, const QString &udi) = 0;

        };
    }
}

Q_DECLARE_INTERFACE(VHardware::Ifaces::Battery, "org.maui.Hardware.Ifaces.Battery/0.1")

#endif // VHARDWARE_IFACES_BATTERY_H
