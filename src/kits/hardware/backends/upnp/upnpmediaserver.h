/*
    Copyright 2010 Paulo Romulo Alves Barros <paulo.romulo@kdemail.net>

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

#ifndef QUBE_HARDWARE_BACKENDS_UPNP_STORAGEACCESS_H
#define QUBE_HARDWARE_BACKENDS_UPNP_STORAGEACCESS_H

#include <ifaces/storageaccess.h>
#include <Qube/Hardware/backends/upnp/upnpdeviceinterface.h>

namespace Qube
{
    namespace Hardware
    {
        namespace Backends
        {
            namespace UPnP
            {
                class UPnPMediaServer : public Qube::Hardware::Backends::UPnP::UPnPDeviceInterface, virtual public Qube::Hardware::Ifaces::StorageAccess
                {
                    Q_OBJECT
                    Q_INTERFACES(Qube::Hardware::Ifaces::StorageAccess)
                public:
                    explicit UPnPMediaServer(UPnPDevice* device);

                    virtual ~UPnPMediaServer();

                    virtual bool isAccessible() const;

                    virtual QString filePath() const;

                    virtual bool isIgnored() const;

                    virtual bool setup();

                    virtual bool teardown();

                private Q_SLOTS:
                    void onSetupTimeout();

                    void onTeardownTimeout();

                Q_SIGNALS:
                    void accessibilityChanged(bool accessible, const QString &udi);

                    void setupDone(Qube::Hardware::ErrorType error, QVariant data, const QString &udi);

                    void teardownDone(Qube::Hardware::ErrorType error, QVariant data, const QString &udi);

                    void setupRequested(const QString &udi);

                    void teardownRequested(const QString &udi);
                };
            }
        }
    }
}

#endif // QUBE_HARDWARE_BACKENDS_UPNP_STORAGEACCESS_H
