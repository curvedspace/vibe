/*
    Copyright 2011 Mario Bensi <mbensi@ipsquad.net>

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

#ifndef VHARDWARE_BACKENDS_FSTAB_NETWORKSHARE_H
#define VHARDWARE_BACKENDS_FSTAB_NETWORKSHARE_H

#include <ifaces/networkshare.h>

#include <QtCore/QObject>

namespace VHardware
{
    namespace Backends
    {
        namespace Fstab
        {
            class FstabDevice;

            class FstabNetworkShare : public QObject, public VHardware::Ifaces::NetworkShare
            {
                Q_OBJECT
                Q_INTERFACES(VHardware::Ifaces::NetworkShare)
            public:
                explicit FstabNetworkShare(VHardware::Backends::Fstab::FstabDevice *device);

                virtual ~FstabNetworkShare();

                virtual VNetworkShare::ShareType type() const;

                virtual QUrl url() const;

            public:
                const VHardware::Backends::Fstab::FstabDevice *fstabDevice() const;

            private:
                VHardware::Backends::Fstab::FstabDevice *m_fstabDevice;
                VNetworkShare::ShareType m_type;
                QUrl m_url;
            };

        }
    }
}

#endif // VHARDWARE_BACKENDS_FSTAB_NETWORKSHARE_H
