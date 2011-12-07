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

#ifndef VHARDWARE_BACKENDS_UPNP_UPNPINTERNETGATEWAY_H
#define VHARDWARE_BACKENDS_UPNP_UPNPINTERNETGATEWAY_H

#include <ifaces/internetgateway.h>

#include "upnpdeviceinterface.h"

#include <HUpnpCore/HClientActionOp>
#include <HUpnpCore/HClientDevice>
#include <HUpnpCore/HClientService>

namespace VHardware
{
    namespace Backends
    {
        namespace UPnP
        {
            class UPnPInternetGateway : public VHardware::Backends::UPnP::UPnPDeviceInterface, virtual public VHardware::Ifaces::InternetGateway
            {
                Q_OBJECT
                Q_INTERFACES(VHardware::Ifaces::InternetGateway)
            public:
                explicit UPnPInternetGateway(UPnPDevice *device);

                virtual ~UPnPInternetGateway();

                virtual void setEnabledForInternet(bool enabled) const;

                virtual VInternetGateway::InternetStatus isEnabledForInternet() const;

                virtual void deletePortMapping(const QString &remoteHost, qint16 externalPort, const VInternetGateway::NetworkProtocol &mappingProtocol);

                virtual void addPortMapping(const QString &remoteHost, qint16 externalPort, const VInternetGateway::NetworkProtocol &mappingProtocol,
                                            qint16 internalPort, const QString &internalClient);

                virtual void requestCurrentConnections();

                virtual QStringList currentConnections() const;

            private:
                Herqq::Upnp::HClientDevice *getDevice(const QString typePreffix, Herqq::Upnp::HClientDevices &devices) const;

                Herqq::Upnp::HClientService *getWANConnectionService(Herqq::Upnp::HClientDevice *device) const;

                int getNumberOfActiveConnections();

                QStringList activeConnections;

                int numberOfConnections;

            private slots:
                void setEnabledForInternetInvokeCallback(Herqq::Upnp::HClientAction *action, const Herqq::Upnp::HClientActionOp &invocationID);

                void deletePortMappingInvokeCallback(Herqq::Upnp::HClientAction *action, const Herqq::Upnp::HClientActionOp &invocationID);

                void addPortMappingInvokeCallback(Herqq::Upnp::HClientAction *action, const Herqq::Upnp::HClientActionOp &invocationID);

                void getActiveConnectionActionInvokeCallback(Herqq::Upnp::HClientAction *action, const Herqq::Upnp::HClientActionOp &invocationID);

            Q_SIGNALS:
                void enabledForInternet(bool enabled);

                void portMappingDeleted(const QString &remoteHost, qint16 externalPort, const VInternetGateway::NetworkProtocol &mappingProtocol);

                void portMappingAdded(const QString &remoteHost, qint16 externalPort, const VInternetGateway::NetworkProtocol &mappingProtocol,
                                      qint16 internalPort, const QString &internalClient);

                void currentConnectionsDataIsReady(QStringList currentConnections);
            };
        }
    }
}

#endif // VHARDWARE_BACKENDS_UPNP_UPNPINTERNETGATEWAY_H
