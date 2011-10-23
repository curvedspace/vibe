/*
    Copyright 2006 Kevin Ottens <ervin@kde.org>

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

#ifndef QUBE_HARDWARE_IFACES_AUDIOINTERFACE_H
#define QUBE_HARDWARE_IFACES_AUDIOINTERFACE_H

#include <ifaces/deviceinterface.h>
#include <Qube/Hardware/audiointerface.h>

namespace Qube
{
    namespace Hardware
    {
        namespace Ifaces
        {
            /**
             * This device interface is available on interfaces exposed by sound cards.
             */
            class AudioInterface : virtual public DeviceInterface
            {
            public:
                /**
                 * Destroys an AudioInterface object.
                 */
                virtual ~AudioInterface();



                /**
                 * Retrieves the audio driver that should be used to access the device.
                 *
                 * @return the driver needed to access the device
                 * @see Qube::Hardware::AudioInterface::AudioDriver
                 */
                virtual Qube::Hardware::AudioInterface::AudioDriver driver() const = 0;

                /**
                 * Retrieves a driver specific string allowing to access the device.
                 *
                 * For example for Alsa devices it is of the form "hw:0,0"
                 * while for OSS it is "/dev/foo".
                 *
                 * @return the driver specific string to handle this device
                 */
                virtual QVariant driverHandle() const = 0;



                /**
                 * Retrieves the name of this audio interface.
                 *
                 * @return the name of the audio interface if available, QString() otherwise
                 */
                virtual QString name() const = 0;

                /**
                 * Retrieves the type of this audio interface.
                 *
                 * @return the type of this audio interface
                 * @see Qube::Hardware::AudioInterface::AudioInterfaceTypes
                 */
                virtual Qube::Hardware::AudioInterface::AudioInterfaceTypes deviceType() const = 0;

                /**
                 * Retrieves the type of soundcard (internal/headset/...).
                 *
                 * @return the type of soundcard
                 * @see Qube::Hardware::AudioInterface::SoundcardType
                 */
                virtual Qube::Hardware::AudioInterface::SoundcardType soundcardType() const = 0;
            };
        }
    }
}

Q_DECLARE_INTERFACE(Qube::Hardware::Ifaces::AudioInterface, "org.vision.Qube.Hardware.Ifaces.AudioInterface/0.1.1")

#endif // QUBE_HARDWARE_IFACES_AUDIOINTERFACE_H
