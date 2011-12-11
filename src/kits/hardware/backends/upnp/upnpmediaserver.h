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

#ifndef VHARDWARE_BACKENDS_UPNP_STORAGEACCESS_H
#define VHARDWARE_BACKENDS_UPNP_STORAGEACCESS_H

#include <backends/upnp/upnpdeviceinterface.h>
#include <ifaces/storageaccess.h>

namespace VHardware
{
    namespace Backends
    {
        namespace UPnP
        {
            class UPnPMediaServer : public VHardware::Backends::UPnP::UPnPDeviceInterface, virtual public VHardware::Ifaces::StorageAccess
            {
                Q_OBJECT
                Q_INTERFACES(VHardware::Ifaces::StorageAccess)
            public:
                explicit UPnPMediaServer(UPnPDevice *device);

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

                void setupDone(VHardware::ErrorType error, QVariant data, const QString &udi);

                void teardownDone(VHardware::ErrorType error, QVariant data, const QString &udi);

                void setupRequested(const QString &udi);

                void teardownRequested(const QString &udi);
            };
        }
    }
}

#endif // VHARDWARE_BACKENDS_UPNP_STORAGEACCESS_H
