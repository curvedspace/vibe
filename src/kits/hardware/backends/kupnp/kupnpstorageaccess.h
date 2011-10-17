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

#ifndef SOLID_BACKENDS_KUPNP_STORAGEACCESS_H
#define SOLID_BACKENDS_KUPNP_STORAGEACCESS_H

// backend
#include "kupnpdeviceinterface.h"
// QubeHardware
#include <ifaces/storageaccess.h>


namespace QubeHardware
{
    namespace Backends
    {
        namespace KUPnP
        {

            class StorageAccess : public DeviceInterface, virtual public QubeHardware::Ifaces::StorageAccess
            {
                Q_OBJECT
                Q_INTERFACES(QubeHardware::Ifaces::StorageAccess)

            public:
                explicit StorageAccess(KUPnPDevice* device);
                virtual ~StorageAccess();

            public: // QubeHardware::Ifaces::StorageAccess API
                virtual bool isAccessible() const;
                virtual QString filePath() const;
                virtual bool isIgnored() const;
                virtual bool setup();
                virtual bool teardown();

            Q_SIGNALS: // QubeHardware::Ifaces::StorageAccess API
                void accessibilityChanged(bool accessible, const QString &udi);
                void setupDone(QubeHardware::ErrorType error, QVariant errorData, const QString &udi);
                void teardownDone(QubeHardware::ErrorType error, QVariant errorData, const QString &udi);
                void setupRequested(const QString &udi);
                void teardownRequested(const QString &udi);
            };

        }
    }
}

#endif
