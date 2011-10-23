/*
    Copyright 2010 Mario Bensi <mbensi@ipsquad.net>

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

#ifndef QUBE_HARDWARE_BACKENDS_FSTAB_FSTAB_DEVICE_H
#define QUBE_HARDWARE_BACKENDS_FSTAB_FSTAB_DEVICE_H

#include <QtCore/QStringList>

#include <ifaces/device.h>

namespace Qube
{
    namespace Hardware
    {
        namespace Backends
        {
            namespace Fstab
            {
                class FstabDevice : public Qube::Hardware::Ifaces::Device
                {
                    Q_OBJECT
                public:
                    FstabDevice(QString uid);

                    virtual ~FstabDevice();

                    virtual QString udi() const;

                    virtual QString parentUdi() const;

                    virtual QString vendor() const;

                    virtual QString product() const;

                    virtual QString icon() const;

                    virtual QStringList emblems() const;

                    virtual QString description() const;

                    virtual bool queryDeviceInterface(const Qube::Hardware::DeviceInterface::Type &type) const;

                    virtual QObject *createDeviceInterface(const Qube::Hardware::DeviceInterface::Type &type);

                    QString device() const;

                private:
                    QString m_uid;
                    QString m_device;
                    QString m_product;
                    QString m_vendor;
                    QString m_description;
                };
            }
        }
    }
}

#endif // QUBE_HARDWARE_BACKENDS_UPNP_UPNP_DEVICE_H
