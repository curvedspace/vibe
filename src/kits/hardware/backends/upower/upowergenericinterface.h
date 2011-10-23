/*
    Copyright 2009 Pino Toscano <pino@kde.org>

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

#ifndef SOLID_BACKENDS_UPOWER_GENERICINTERFACE_H
#define SOLID_BACKENDS_UPOWER_GENERICINTERFACE_H

#include <ifaces/genericinterface.h>
#include <Qube/Hardware/genericinterface.h>
#include "upowerdeviceinterface.h"

namespace Qube
{
    namespace Hardware
    {
        namespace Backends
        {
            namespace UPower
            {
                class UPowerDevice;

                class GenericInterface : public DeviceInterface, virtual public Qube::Hardware::Ifaces::GenericInterface
                {
                    Q_OBJECT
                    Q_INTERFACES(Qube::Hardware::Ifaces::GenericInterface)

                public:
                    GenericInterface(UPowerDevice *device);
                    virtual ~GenericInterface();

                    virtual QVariant property(const QString &key) const;
                    virtual QMap<QString, QVariant> allProperties() const;
                    virtual bool propertyExists(const QString &key) const;

                Q_SIGNALS:
                    void propertyChanged(const QMap<QString, int> &changes);
                    void conditionRaised(const QString &condition, const QString &reason);
                };
            }
        }
    }
}

#endif // SOLID_BACKENDS_UPOWER_GENERICINTERFACE_H
