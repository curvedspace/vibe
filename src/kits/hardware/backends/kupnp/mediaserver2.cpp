/*
    Copyright 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "mediaserver2.h"

#include <QtCore/QStringList>
#include <QtCore/QSet>

#include "kupnpstorageaccess.h"

namespace VHardware
{
    namespace Backends
    {
        namespace KUPnP
        {
            static const char MediaServer2Udn[] = "urn:schemas-upnp-org:device:MediaServer:2";

            MediaServer2Factory::MediaServer2Factory() {}

            void MediaServer2Factory::addSupportedInterfaces(QSet<VDeviceInterface::Type>& interfaces) const
            {
                interfaces << VDeviceInterface::StorageAccess;
            }

            QStringList MediaServer2Factory::typeNames(VDeviceInterface::Type type) const
            {
                QStringList result;

                if (type == VDeviceInterface::StorageAccess)
                    result << QLatin1String(MediaServer2Udn);

                return result;
            }


            QObject *MediaServer2Factory::tryCreateDevice(const Cagibi::Device &device) const
            {
                return (device.type() == QLatin1String(MediaServer2Udn)) ?
                       new MediaServer2(device) : 0;
            }


            MediaServer2::MediaServer2(const Cagibi::Device &device)
                : KUPnPDevice(device)
            {
            }

            MediaServer2::~MediaServer2()
            {
            }

            QString MediaServer2::icon() const
            {
                return QString::fromLatin1("folder-remote");
            }


            QString MediaServer2::description() const
            {
                return QObject::tr("UPnP Media Server v2");
            }


            bool MediaServer2::queryDeviceInterface(const VDeviceInterface::Type &type) const
            {
                bool result = false;

                if (type == VDeviceInterface::StorageAccess)
                    result = true;

                return result;
            }

            QObject *MediaServer2::createDeviceInterface(const VDeviceInterface::Type &type)
            {
                DeviceInterface *interface = 0;

                if (type == VDeviceInterface::StorageAccess)
                    interface = new StorageAccess(this);

                return interface;
            }
        }
    }
}
