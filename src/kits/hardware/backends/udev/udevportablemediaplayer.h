/*
    Copyright 2010 Rafael Fernández López <ereslibre@kde.org>

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

#ifndef QUBE_HARDWARE_BACKENDS_UDEV_PORTABLEMEDIAPLAYER_H
#define QUBE_HARDWARE_BACKENDS_UDEV_PORTABLEMEDIAPLAYER_H

#include <ifaces/portablemediaplayer.h>
#include "udevdeviceinterface.h"

#include <QtCore/QStringList>

namespace Qube
{
    namespace Hardware
    {
        namespace Backends
        {
            namespace UDev
            {
                class UDevDevice;

                class PortableMediaPlayer : public DeviceInterface, virtual public Qube::Hardware::Ifaces::PortableMediaPlayer
                {
                    Q_OBJECT
                    Q_INTERFACES(Qube::Hardware::Ifaces::PortableMediaPlayer)

                public:
                    PortableMediaPlayer(UDevDevice *device);
                    virtual ~PortableMediaPlayer();

                    virtual QStringList supportedProtocols() const;
                    virtual QStringList supportedDrivers(QString protocol = QString()) const;
                    virtual QVariant driverHandle(const QString &driver) const;
                };
            }
        }
    }
}

#endif // QUBE_HARDWARE_BACKENDS_UDEV_PORTABLEMEDIAPLAYER_H
