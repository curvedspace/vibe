/*
    Copyright 2006 Davide Bettio <davbet@aliceposta.it>
    Copyright 2007 Kevin Ottens <ervin@kde.org>

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

#include "vbutton.h"
#include "vbutton_p.h"
#include "vcall_p.h"

#include <ifaces/button.h>

VButton::VButton(QObject *backendObject)
    : VDeviceInterface(*new VButtonPrivate(), backendObject)
{
    connect(backendObject, SIGNAL(pressed(VButton::ButtonType, const QString &)),
            this, SIGNAL(pressed(VButton::ButtonType, const QString &)));
}

VButton::~VButton()
{
}

VButton::ButtonType VButton::type() const
{
    Q_D(const VButton);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::Button *, d->backendObject(), UnknownButtonType, type());
}

bool VButton::hasState() const
{
    Q_D(const VButton);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::Button *, d->backendObject(), false, hasState());
}

bool VButton::stateValue() const
{
    Q_D(const VButton);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::Button *, d->backendObject(), false, stateValue());
}

#include "vbutton.moc"
