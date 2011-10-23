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

#ifndef QUBE_HARDWARE_IFACES_INTERNETGATEWAY_H
#define QUBE_HARDWARE_IFACES_INTERNETGATEWAY_H

#include <QtCore/QStringList>

#include "deviceinterface.h"
#include "Qube/Hardware/internetgateway.h"

namespace Qube
{
    namespace Hardware
    {
        namespace Ifaces
        {

            class InternetGateway : virtual public Qube::Hardware::Ifaces::DeviceInterface
            {
            public:
                virtual ~InternetGateway();

                virtual void requestCurrentConnections() = 0;

                virtual QStringList currentConnections() const = 0;

                virtual void addPortMapping(const QString& remoteHost, qint16 externalPort, const Qube::Hardware::InternetGateway::NetworkProtocol& mappingProtocol,
                                            qint16 internalPort, const QString& internalClient) = 0;

                virtual void deletePortMapping(const QString& remoteHost, qint16 externalPort, const Qube::Hardware::InternetGateway::NetworkProtocol& mappingProtocol) = 0;

                virtual Qube::Hardware::InternetGateway::InternetStatus isEnabledForInternet() const = 0;

                virtual void setEnabledForInternet(bool enabled) const = 0;

            protected: //Q_SIGNALS:
                virtual void portMappingAdded(const QString& remoteHost, qint16 externalPort, const Qube::Hardware::InternetGateway::NetworkProtocol& mappingProtocol,
                                              qint16 internalPort, const QString& internalClient) = 0;

                virtual void portMappingDeleted(const QString& remoteHost, qint16 externalPort, const Qube::Hardware::InternetGateway::NetworkProtocol& mappingProtocol) = 0;

                virtual void enabledForInternet(bool enabled) = 0;

                virtual void currentConnectionsDataIsReady(QStringList currentConnections) = 0;
            };

        }
    }
}

Q_DECLARE_INTERFACE(Qube::Hardware::Ifaces::InternetGateway, "org.vision.Qube.Hardware.Ifaces.InternetGateway/0.1")

#endif // QUBE_HARDWARE_IFACES_INTERNETGATEWAY_H
