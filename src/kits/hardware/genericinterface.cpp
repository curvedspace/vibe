/*
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

#include "genericinterface.h"
#include "genericinterface_p.h"
#include "call_p.h"

#include <ifaces/genericinterface.h>

Qube::Hardware::GenericInterface::GenericInterface(QObject *backendObject)
    : DeviceInterface(*new GenericInterfacePrivate(), backendObject)
{
    if (backendObject) {
        connect(backendObject, SIGNAL(propertyChanged(const QMap<QString,int> &)),
                this, SIGNAL(propertyChanged(const QMap<QString,int> &)));
        connect(backendObject, SIGNAL(conditionRaised(const QString &, const QString &)),
                this, SIGNAL(conditionRaised(const QString &, const QString &)));
    }
}


Qube::Hardware::GenericInterface::~GenericInterface()
{

}

QVariant Qube::Hardware::GenericInterface::property(const QString &key) const
{
    Q_D(const GenericInterface);
    return_QUBE_HARDWARE_CALL(Ifaces::GenericInterface *, d->backendObject(), QVariant(), property(key));
}

QMap<QString, QVariant> Qube::Hardware::GenericInterface::allProperties() const
{
    Q_D(const GenericInterface);
    return_QUBE_HARDWARE_CALL(Ifaces::GenericInterface *, d->backendObject(), QVariantMap(), allProperties());
}

bool Qube::Hardware::GenericInterface::propertyExists(const QString &key) const
{
    Q_D(const GenericInterface);
    return_QUBE_HARDWARE_CALL(Ifaces::GenericInterface *, d->backendObject(), false, propertyExists(key));
}
