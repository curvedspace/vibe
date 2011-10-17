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

SOLID_GLOBAL_STATIC(QubeHardware::NetworkingPrivate, globalNetworkManager)

QubeHardware::NetworkingPrivate::NetworkingPrivate()
    : netStatus(QubeHardware::Networking::Unknown),
      connectPolicy(QubeHardware::Networking::Managed),
      disconnectPolicy(QubeHardware::Networking::Managed),
      iface(new OrgVisionHardwareNetworkingClientInterface("org.kde.kded",
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

QubeHardware::NetworkingPrivate::~NetworkingPrivate()
{
}

QubeHardware::Networking::Notifier::Notifier()
{
}

void QubeHardware::NetworkingPrivate::initialize()
{
    QDBusPendingReply<uint> reply = iface->status();
    reply.waitForFinished();
    if (reply.isValid()) {
        netStatus = ( QubeHardware::Networking::Status )reply.value();
    } else {
        netStatus = QubeHardware::Networking::Unknown;
    }
}

uint QubeHardware::NetworkingPrivate::status() const
{
    return netStatus;
}

/*=========================================================================*/

QubeHardware::Networking::Status QubeHardware::Networking::status()
{
    return static_cast<QubeHardware::Networking::Status>( globalNetworkManager->status() );
}

QubeHardware::Networking::Notifier *QubeHardware::Networking::notifier()
{
    return globalNetworkManager;
}

void QubeHardware::NetworkingPrivate::serviceStatusChanged( uint status )
{
//    kDebug( 921 ) ;
    netStatus = ( QubeHardware::Networking::Status )status;
    switch ( netStatus ) {
    case QubeHardware::Networking::Unknown:
        break;
    case QubeHardware::Networking::Unconnected:
    case QubeHardware::Networking::Disconnecting:
    case QubeHardware::Networking::Connecting:
        if ( disconnectPolicy == QubeHardware::Networking::Managed ) {
            emit globalNetworkManager->shouldDisconnect();
        } else if ( disconnectPolicy == QubeHardware::Networking::OnNextStatusChange ) {
            setDisconnectPolicy( QubeHardware::Networking::Manual );
            emit globalNetworkManager->shouldDisconnect();
        }
        break;
    case QubeHardware::Networking::Connected:
        if ( disconnectPolicy == QubeHardware::Networking::Managed ) {
            emit globalNetworkManager->shouldConnect();
        } else if ( disconnectPolicy == QubeHardware::Networking::OnNextStatusChange ) {
            setConnectPolicy( QubeHardware::Networking::Manual );
            emit globalNetworkManager->shouldConnect();
        }
        break;
//      default:
//        kDebug( 921 ) <<  "Unrecognised status code!";
    }
    emit globalNetworkManager->statusChanged( netStatus );
}

void QubeHardware::NetworkingPrivate::serviceOwnerChanged( const QString & name, const QString & oldOwner, const QString & newOwner )
{
    Q_UNUSED(name)
    Q_UNUSED(oldOwner)
    if ( newOwner.isEmpty() ) {
        // kded quit on us
        netStatus = QubeHardware::Networking::Unknown;
        emit globalNetworkManager->statusChanged( netStatus );

    } else {
        // kded was replaced or started
        initialize();
        emit globalNetworkManager->statusChanged( netStatus );
        serviceStatusChanged( netStatus );
    }
}

QubeHardware::Networking::ManagementPolicy QubeHardware::Networking::connectPolicy()
{
    return globalNetworkManager->connectPolicy;
}

void QubeHardware::Networking::setConnectPolicy( QubeHardware::Networking::ManagementPolicy policy )
{
    globalNetworkManager->connectPolicy = policy;
}

QubeHardware::Networking::ManagementPolicy QubeHardware::Networking::disconnectPolicy()
{
    return globalNetworkManager->disconnectPolicy;
}

void QubeHardware::Networking::setDisconnectPolicy( QubeHardware::Networking::ManagementPolicy policy )
{
    globalNetworkManager->disconnectPolicy = policy;
}
