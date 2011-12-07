/*
    Copyright 2010 Mario Bensi <mbensi@ipsquad.net>

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

#ifndef VHARDWARE_BACKENDS_FSTAB_STORAGEACCESS_H
#define VHARDWARE_BACKENDS_FSTAB_STORAGEACCESS_H

#include <ifaces/storageaccess.h>

#include <QtCore/QObject>
#include <QtCore/QProcess>

namespace VHardware
{
    namespace Backends
    {
        namespace Fstab
        {
            class FstabDevice;

            class FstabStorageAccess : public QObject, public VHardware::Ifaces::StorageAccess
            {
                Q_OBJECT
                Q_INTERFACES(VHardware::Ifaces::StorageAccess)
            public:
                explicit FstabStorageAccess(VHardware::Backends::Fstab::FstabDevice *device);

                virtual ~FstabStorageAccess();

                virtual bool isAccessible() const;

                virtual QString filePath() const;

                virtual bool isIgnored() const;

                virtual bool setup();

                virtual bool teardown();

            public:
                const VHardware::Backends::Fstab::FstabDevice *fstabDevice() const;

            Q_SIGNALS:
                void accessibilityChanged(bool accessible, const QString &udi);

                void setupDone(VHardware::ErrorType error, QVariant data, const QString &udi);

                void teardownDone(VHardware::ErrorType error, QVariant data, const QString &udi);

                void setupRequested(const QString &udi);

                void teardownRequested(const QString &udi);

            private Q_SLOTS:
                void slotSetupFinished(int exitCode, QProcess::ExitStatus exitStatus);
                void slotTeardownFinished(int exitCode, QProcess::ExitStatus exitStatus);
                void onMtabChanged();
                void connectDBusSignals();

                void slotSetupRequested();
                void slotSetupDone(int error, const QString &errorString);
                void slotTeardownRequested();
                void slotTeardownDone(int error, const QString &errorString);

            private:
                VHardware::Backends::Fstab::FstabDevice *m_fstabDevice;
                QProcess *m_process;
                QStringList m_currentMountPoints;
            };

        }
    }
}

#endif // VHARDWARE_BACKENDS_FSTAB_DEVICE_INTERFACE_H
