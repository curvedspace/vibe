/*
    Copyright 2006-2007 Will Stephenson <wstephenson@kde.org>
    Copyright 2006-2007 Kevin Ottens <ervin@kde.org>

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

#ifndef VNETWORKING_P_H
#define VNETWORKING_P_H

#include <QtCore/QMap>

#include "vnetworking.h"

class OrgVisionVibeHardwareNetworkingClientInterface;
class QAbstractSocket;
class QTimer;

class VManagedSocketContainer;

class VIBE_EXPORT VNetworkingPrivate : public VHardware::Networking::Notifier
{
    Q_OBJECT
    Q_PROPERTY(uint Status  READ status)
    Q_CLASSINFO("D-Bus Interface", "org.vision.Vibe.Hardware.Networking.Client")
public:
    VNetworkingPrivate();
    ~VNetworkingPrivate();
    void shouldConnect() {
        VHardware::Networking::Notifier::shouldConnect();
    }
    void shouldDisconnect() {
        VHardware::Networking::Notifier::shouldDisconnect();
    }
    VHardware::Networking::Status netStatus;
    VHardware::Networking::ManagementPolicy connectPolicy;
    VHardware::Networking::ManagementPolicy disconnectPolicy;

public Q_SLOTS:
    uint status() const;

    /**
     * Called on DBus signal from the network status service
     */
    void serviceStatusChanged(uint status);

    /**
     * Detects when kded restarts, and sets status to NoNetworks so that apps
     * may proceed
     */
    void serviceOwnerChanged(const QString &, const QString &, const QString &);

private:
    void initialize();
    OrgVisionVibeHardwareNetworkingClientInterface *iface;
};

#endif // VNETWORKING_P_H
