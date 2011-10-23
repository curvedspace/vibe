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

#include "networking.h"
#include "networking_p.h"

#include "soliddefs_p.h"
#include "org_vision_hardware_networking_client.h"

SOLID_GLOBAL_STATIC(Qube::Hardware::NetworkingPrivate, globalNetworkManager)

Qube::Hardware::NetworkingPrivate::NetworkingPrivate()
    : netStatus(Qube::Hardware::Networking::Unknown),
      connectPolicy(Qube::Hardware::Networking::Managed),
      disconnectPolicy(Qube::Hardware::Networking::Managed),
      iface(new OrgVisionQubeHardwareNetworkingClientInterface("org.kde.kded",
              "/modules/networkstatus",
              QDBusConnection::sessionBus(),
              this))
{
    //connect( iface, SIGNAL( statusChanged( uint ) ), globalNetworkManager, SIGNAL( statusChanged( Networking::Status ) ) );
    connect(iface, SIGNAL(statusChanged(uint)), this, SLOT(serviceStatusChanged(uint)));
    QDBusServiceWatcher *watcher = new QDBusServiceWatcher("org.kde.kded", QDBusConnection::sessionBus(),
            QDBusServiceWatcher::WatchForOwnerChange, this);
    connect(watcher, SIGNAL(serviceOwnerChanged(QString,QString,QString)),
            this, SLOT(serviceOwnerChanged(QString,QString,QString)));

    initialize();
}

Qube::Hardware::NetworkingPrivate::~NetworkingPrivate()
{
}

Qube::Hardware::Networking::Notifier::Notifier()
{
}

void Qube::Hardware::NetworkingPrivate::initialize()
{
    QDBusPendingReply<uint> reply = iface->status();
    reply.waitForFinished();
    if (reply.isValid()) {
        netStatus = ( Qube::Hardware::Networking::Status )reply.value();
    } else {
        netStatus = Qube::Hardware::Networking::Unknown;
    }
}

uint Qube::Hardware::NetworkingPrivate::status() const
{
    return netStatus;
}

/*=========================================================================*/

Qube::Hardware::Networking::Status Qube::Hardware::Networking::status()
{
    return static_cast<Qube::Hardware::Networking::Status>( globalNetworkManager->status() );
}

Qube::Hardware::Networking::Notifier *Qube::Hardware::Networking::notifier()
{
    return globalNetworkManager;
}

void Qube::Hardware::NetworkingPrivate::serviceStatusChanged( uint status )
{
//    kDebug( 921 ) ;
    netStatus = ( Qube::Hardware::Networking::Status )status;
    switch ( netStatus ) {
    case Qube::Hardware::Networking::Unknown:
        break;
    case Qube::Hardware::Networking::Unconnected:
    case Qube::Hardware::Networking::Disconnecting:
    case Qube::Hardware::Networking::Connecting:
        if ( disconnectPolicy == Qube::Hardware::Networking::Managed ) {
            emit globalNetworkManager->shouldDisconnect();
        } else if ( disconnectPolicy == Qube::Hardware::Networking::OnNextStatusChange ) {
            setDisconnectPolicy( Qube::Hardware::Networking::Manual );
            emit globalNetworkManager->shouldDisconnect();
        }
        break;
    case Qube::Hardware::Networking::Connected:
        if ( disconnectPolicy == Qube::Hardware::Networking::Managed ) {
            emit globalNetworkManager->shouldConnect();
        } else if ( disconnectPolicy == Qube::Hardware::Networking::OnNextStatusChange ) {
            setConnectPolicy( Qube::Hardware::Networking::Manual );
            emit globalNetworkManager->shouldConnect();
        }
        break;
//      default:
//        kDebug( 921 ) <<  "Unrecognised status code!";
    }
    emit globalNetworkManager->statusChanged( netStatus );
}

void Qube::Hardware::NetworkingPrivate::serviceOwnerChanged( const QString & name, const QString & oldOwner, const QString & newOwner )
{
    Q_UNUSED(name)
    Q_UNUSED(oldOwner)
    if ( newOwner.isEmpty() ) {
        // kded quit on us
        netStatus = Qube::Hardware::Networking::Unknown;
        emit globalNetworkManager->statusChanged( netStatus );

    } else {
        // kded was replaced or started
        initialize();
        emit globalNetworkManager->statusChanged( netStatus );
        serviceStatusChanged( netStatus );
    }
}

Qube::Hardware::Networking::ManagementPolicy Qube::Hardware::Networking::connectPolicy()
{
    return globalNetworkManager->connectPolicy;
}

void Qube::Hardware::Networking::setConnectPolicy( Qube::Hardware::Networking::ManagementPolicy policy )
{
    globalNetworkManager->connectPolicy = policy;
}

Qube::Hardware::Networking::ManagementPolicy Qube::Hardware::Networking::disconnectPolicy()
{
    return globalNetworkManager->disconnectPolicy;
}

void Qube::Hardware::Networking::setDisconnectPolicy( Qube::Hardware::Networking::ManagementPolicy policy )
{
    globalNetworkManager->disconnectPolicy = policy;
}
