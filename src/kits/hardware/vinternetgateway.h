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

#ifndef VINTERNETGATEWAY_H
#define VINTERNETGATEWAY_H

#include <QtCore/QStringList>

#include <VibeCore/VGlobal>
#include <VibeHardware/VDeviceInterface>
#include <VibeHardware/VHardware>

class VInternetGatewayPrivate;
class VDevice;

class VIBE_EXPORT VInternetGateway : public VDeviceInterface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(VInternetGateway)
    friend class VDevice;

private:
    explicit VInternetGateway(QObject *backendObject);

public:
    enum InternetStatus { InternetEnabled = 0, InternetDisabled = 1, UnknownStatus = 2 };

    enum NetworkProtocol { TCP = 0, UDP = 1 };

    virtual ~VInternetGateway();

    static Type deviceInterfaceType() {
        return VDeviceInterface::InternetGateway;
    }

    void requestCurrentConnections() const;

    QStringList currentConnections() const;

    void addPortMapping(const QString &remoteHost, qint16 externalPort, const NetworkProtocol &mappingProtocol,
                        qint16 internalPort, const QString &internalClient);

    void deletePortMapping(const QString &remoteHost, qint16 externalPort, const NetworkProtocol &mappingProtocol);

    InternetStatus isEnabledForInternet() const;

    void setEnabledForInternet(bool enabled);

Q_SIGNALS:
    void portMappingAdded(const QString &remoteHost, qint16 externalPort, const NetworkProtocol &mappingProtocol,
                          qint16 internalPort, const QString &internalClient);

    void portMappingDeleted(const QString &remoteHost, qint16 externalPort, const NetworkProtocol &mappingProtocol);

    void enabledForInternet(bool enabled);

    void currentConnectionsDataIsReady(QStringList currentConnections);

protected:
    VInternetGateway(VInternetGatewayPrivate &dd, QObject *backendObject);
};

#endif // VINTERNETGATEWAY_H
