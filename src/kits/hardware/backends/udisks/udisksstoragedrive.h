/*
    Copyright 2010 Michael Zanetti <mzanetti@kde.org>
    Copyright 2010 Lukas Tinkl <ltinkl@redhat.com>

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

#ifndef UDISKSSTORAGEDRIVE_H
#define UDISKSSTORAGEDRIVE_H

#include <ifaces/storagedrive.h>
#include "udisksblock.h"

namespace VHardware
{
    namespace Backends
    {
        namespace UDisks
        {
            class UDisksStorageDrive: public Block, virtual public VHardware::Ifaces::StorageDrive
            {
                Q_OBJECT
                Q_INTERFACES(VHardware::Ifaces::StorageDrive)
            public:
                UDisksStorageDrive(UDisksDevice *device);
                virtual ~UDisksStorageDrive();

                virtual qulonglong size() const;
                virtual bool isHotpluggable() const;
                virtual bool isRemovable() const;
                virtual VStorageDrive::DriveType driveType() const;
                virtual VStorageDrive::Bus bus() const;
            };
        }
    }
}

#endif // UDISKSSTORAGEDRIVE_H
