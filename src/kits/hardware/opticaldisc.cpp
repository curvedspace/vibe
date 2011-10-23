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

#include "opticaldisc.h"
#include "opticaldisc_p.h"

#include "soliddefs_p.h"
#include <ifaces/opticaldisc.h>

Qube::Hardware::OpticalDisc::OpticalDisc(QObject *backendObject)
    : StorageVolume(*new OpticalDiscPrivate(), backendObject)
{
}

Qube::Hardware::OpticalDisc::~OpticalDisc()
{

}

Qube::Hardware::OpticalDisc::ContentTypes Qube::Hardware::OpticalDisc::availableContent() const
{
    Q_D(const OpticalDisc);
    return_SOLID_CALL(Ifaces::OpticalDisc *, d->backendObject(), ContentTypes(), availableContent());
}

Qube::Hardware::OpticalDisc::DiscType Qube::Hardware::OpticalDisc::discType() const
{
    Q_D(const OpticalDisc);
    return_SOLID_CALL(Ifaces::OpticalDisc *, d->backendObject(), UnknownDiscType, discType());
}

bool Qube::Hardware::OpticalDisc::isAppendable() const
{
    Q_D(const OpticalDisc);
    return_SOLID_CALL(Ifaces::OpticalDisc *, d->backendObject(), false, isAppendable());
}

bool Qube::Hardware::OpticalDisc::isBlank() const
{
    Q_D(const OpticalDisc);
    return_SOLID_CALL(Ifaces::OpticalDisc *, d->backendObject(), false, isBlank());
}

bool Qube::Hardware::OpticalDisc::isRewritable() const
{
    Q_D(const OpticalDisc);
    return_SOLID_CALL(Ifaces::OpticalDisc *, d->backendObject(), false, isRewritable());
}

qulonglong Qube::Hardware::OpticalDisc::capacity() const
{
    Q_D(const OpticalDisc);
    return_SOLID_CALL(Ifaces::OpticalDisc *, d->backendObject(), 0, capacity());
}
