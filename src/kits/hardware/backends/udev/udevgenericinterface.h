/*
    Copyright 2010 Kevin Ottens <ervin@kde.org>

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

#ifndef QUBE_HARDWARE_BACKENDS_UDEV_GENERICINTERFACE_H
#define QUBE_HARDWARE_BACKENDS_UDEV_GENERICINTERFACE_H

#include <ifaces/genericinterface.h>
#include <Qube/Hardware/genericinterface.h>
#include "udevdeviceinterface.h"

namespace Qube
{
    namespace Hardware
    {
        namespace Backends
        {
            namespace UDev
            {
                class UDevDevice;

                class GenericInterface : public DeviceInterface, virtual public Qube::Hardware::Ifaces::GenericInterface
                {
                    Q_OBJECT
                    Q_INTERFACES(Qube::Hardware::Ifaces::GenericInterface)

                public:
                    GenericInterface(UDevDevice *device);
                    virtual ~GenericInterface();

                    virtual QVariant property(const QString &key) const;
                    virtual QMap<QString, QVariant> allProperties() const;
                    virtual bool propertyExists(const QString &key) const;

                Q_SIGNALS:
                    void propertyChanged(const QMap<QString,int> &changes);
                    void conditionRaised(const QString &condition, const QString &reason);
                };
            }
        }
    }
}

#endif
