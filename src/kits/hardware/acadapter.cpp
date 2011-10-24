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

#include "acadapter.h"
#include "acadapter_p.h"

#include "call_p.h"
#include <ifaces/acadapter.h>

Qube::Hardware::AcAdapter::AcAdapter(QObject *backendObject)
    : DeviceInterface(*new AcAdapterPrivate(), backendObject)
{
    connect(backendObject, SIGNAL(plugStateChanged(bool, const QString &)),
            this, SIGNAL(plugStateChanged(bool, const QString &)));
}

Qube::Hardware::AcAdapter::~AcAdapter()
{

}

bool Qube::Hardware::AcAdapter::isPlugged() const
{
    Q_D(const AcAdapter);
    return_QUBE_HARDWARE_CALL(Ifaces::AcAdapter *, d->backendObject(), false, isPlugged());
}

#include "acadapter.moc"
