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

#include "audiointerface.h"
#include "audiointerface_p.h"

#include "soliddefs_p.h"
#include <ifaces/audiointerface.h>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

QubeHardware::AudioInterface::AudioInterface(QObject *backendObject)
    : DeviceInterface(*new AudioInterfacePrivate(), backendObject)
{
}

QubeHardware::AudioInterface::~AudioInterface()
{

}


QubeHardware::AudioInterface::AudioDriver QubeHardware::AudioInterface::driver() const
{
    Q_D(const AudioInterface);
    return_SOLID_CALL(Ifaces::AudioInterface *, d->backendObject(), UnknownAudioDriver, driver());
}

QVariant QubeHardware::AudioInterface::driverHandle() const
{
    Q_D(const AudioInterface);
    return_SOLID_CALL(Ifaces::AudioInterface *, d->backendObject(), QVariant(), driverHandle());
}

QString QubeHardware::AudioInterface::name() const
{
    Q_D(const AudioInterface);
    return_SOLID_CALL(Ifaces::AudioInterface *, d->backendObject(), QString(), name());
}

QubeHardware::AudioInterface::AudioInterfaceTypes QubeHardware::AudioInterface::deviceType() const
{
    Q_D(const AudioInterface);
    return_SOLID_CALL(Ifaces::AudioInterface *, d->backendObject(), UnknownAudioInterfaceType, deviceType());
}

QubeHardware::AudioInterface::SoundcardType QubeHardware::AudioInterface::soundcardType() const
{
    Q_D(const AudioInterface);
    return_SOLID_CALL(Ifaces::AudioInterface *, d->backendObject(), InternalSoundcard, soundcardType());
}
