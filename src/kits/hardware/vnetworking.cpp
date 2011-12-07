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

//#include <QtCore/QDebug>

#include <VibeCore/VGlobal>

#include "vnetworking.h"
#include "vnetworking_p.h"

#include "org_vision_vibe_hardware_networking_client.h"

VIBE_GLOBAL_STATIC(VNetworkingPrivate, globalNetworkManager)

VNetworkingPrivate::VNetworkingPrivate()
    : netStatus(VHardware::Networking::Unknown),
      connectPolicy(VHardware::Networking::Managed),
      disconnectPolicy(VHardware::Networking::Managed),
      iface(new OrgVisionVibeHardwareNetworkingClientInterface("org.kde.kded",
              "/modules/networkstatus",
              QDBusConnection::sessionBus(),
              this))
{
    //connect( iface, SIGNAL( statusChanged( uint ) ), globalNetworkManager, SIGNAL( statusChanged( Networking::Status ) ) );
    connect(iface, SIGNAL(statusChanged(uint)), this, SLOT(serviceStatusChanged(uint)));
    QDBusServiceWatcher *watcher = new QDBusServiceWatcher("org.kde.kded", QDBusConnection::sessionBus(),
            QDBusServiceWatcher::WatchForOwnerChange, this);
    connect(watcher, SIGNAL(serviceOwnerChanged(QString, QString, QString)),
            this, SLOT(serviceOwnerChanged(QString, QString, QString)));

    initialize();
}

VNetworkingPrivate::~VNetworkingPrivate()
{
}

VHardware::Networking::Notifier::Notifier()
{
}

void VNetworkingPrivate::initialize()
{
    QDBusPendingReply<uint> reply = iface->status();
    reply.waitForFinished();
    if (reply.isValid())
        netStatus = (VHardware::Networking::Status)reply.value();
    else
        netStatus = VHardware::Networking::Unknown;
}

uint VNetworkingPrivate::status() const
{
    return netStatus;
}

/*=========================================================================*/

VHardware::Networking::Status VHardware::Networking::status()
{
    return static_cast<VHardware::Networking::Status>(globalNetworkManager->status());
}

VHardware::Networking::Notifier *VHardware::Networking::notifier()
{
    return globalNetworkManager;
}

void VNetworkingPrivate::serviceStatusChanged(uint status)
{
    //    kDebug( 921 ) ;
    netStatus = (VHardware::Networking::Status)status;
    switch (netStatus) {
        case VHardware::Networking::Unknown:
            break;
        case VHardware::Networking::Unconnected:
        case VHardware::Networking::Disconnecting:
        case VHardware::Networking::Connecting:
            if (disconnectPolicy == VHardware::Networking::Managed) {
                emit globalNetworkManager->shouldDisconnect();
            } else if (disconnectPolicy == VHardware::Networking::OnNextStatusChange) {
                setDisconnectPolicy(VHardware::Networking::Manual);
                emit globalNetworkManager->shouldDisconnect();
            }
            break;
        case VHardware::Networking::Connected:
            if (disconnectPolicy == VHardware::Networking::Managed) {
                emit globalNetworkManager->shouldConnect();
            } else if (disconnectPolicy == VHardware::Networking::OnNextStatusChange) {
                setConnectPolicy(VHardware::Networking::Manual);
                emit globalNetworkManager->shouldConnect();
            }
            break;
            //      default:
            //        kDebug( 921 ) <<  "Unrecognised status code!";
    }
    emit globalNetworkManager->statusChanged(netStatus);
}

void VNetworkingPrivate::serviceOwnerChanged(const QString &name, const QString &oldOwner, const QString &newOwner)
{
    Q_UNUSED(name)
    Q_UNUSED(oldOwner)
    if (newOwner.isEmpty()) {
        // kded quit on us
        netStatus = VHardware::Networking::Unknown;
        emit globalNetworkManager->statusChanged(netStatus);

    } else {
        // kded was replaced or started
        initialize();
        emit globalNetworkManager->statusChanged(netStatus);
        serviceStatusChanged(netStatus);
    }
}

VHardware::Networking::ManagementPolicy VHardware::Networking::connectPolicy()
{
    return globalNetworkManager->connectPolicy;
}

void VHardware::Networking::setConnectPolicy(VHardware::Networking::ManagementPolicy policy)
{
    globalNetworkManager->connectPolicy = policy;
}

VHardware::Networking::ManagementPolicy VHardware::Networking::disconnectPolicy()
{
    return globalNetworkManager->disconnectPolicy;
}

void VHardware::Networking::setDisconnectPolicy(VHardware::Networking::ManagementPolicy policy)
{
    globalNetworkManager->disconnectPolicy = policy;
}

#include "vnetworking_p.moc"
#include "vnetworking.moc"
