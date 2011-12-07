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

#include "vstorageaccess.h"
#include "vstorageaccess_p.h"
#include "vcall_p.h"

#include <ifaces/storageaccess.h>

VStorageAccess::VStorageAccess(QObject *backendObject)
    : VDeviceInterface(*new VStorageAccessPrivate(), backendObject)
{
    connect(backendObject, SIGNAL(setupDone(VHardware::ErrorType, QVariant, const QString &)),
            this, SIGNAL(setupDone(VHardware::ErrorType, QVariant, const QString &)));
    connect(backendObject, SIGNAL(teardownDone(VHardware::ErrorType, QVariant, const QString &)),
            this, SIGNAL(teardownDone(VHardware::ErrorType, QVariant, const QString &)));
    connect(backendObject, SIGNAL(setupRequested(const QString &)),
            this, SIGNAL(setupRequested(const QString &)));
    connect(backendObject, SIGNAL(teardownRequested(const QString &)),
            this, SIGNAL(teardownRequested(const QString &)));

    connect(backendObject, SIGNAL(accessibilityChanged(bool, const QString &)),
            this, SIGNAL(accessibilityChanged(bool, const QString &)));
}

VStorageAccess::VStorageAccess(VStorageAccessPrivate &dd, QObject *backendObject)
    : VDeviceInterface(dd, backendObject)
{
    connect(backendObject, SIGNAL(setupDone(VStorageAccess::SetupResult, QVariant, const QString &)),
            this, SIGNAL(setupDone(VStorageAccess::SetupResult, QVariant, const QString &)));
    connect(backendObject, SIGNAL(teardownDone(VStorageAccess::TeardownResult, QVariant, const QString &)),
            this, SIGNAL(teardownDone(VStorageAccess::TeardownResult, QVariant, const QString &)));
    connect(backendObject, SIGNAL(setupRequested(const QString &)),
            this, SIGNAL(setupRequested(const QString &)));
    connect(backendObject, SIGNAL(teardownRequested(const QString &)),
            this, SIGNAL(teardownRequested(const QString &)));
    connect(backendObject, SIGNAL(accessibilityChanged(bool, const QString &)),
            this, SIGNAL(accessibilityChanged(bool, const QString &)));
}

VStorageAccess::~VStorageAccess()
{
}

bool VStorageAccess::isAccessible() const
{
    Q_D(const VStorageAccess);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::StorageAccess *, d->backendObject(), false, isAccessible());
}

QString VStorageAccess::filePath() const
{
    Q_D(const VStorageAccess);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::StorageAccess *, d->backendObject(), QString(), filePath());
}

bool VStorageAccess::setup()
{
    Q_D(VStorageAccess);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::StorageAccess *, d->backendObject(), false, setup());
}

bool VStorageAccess::teardown()
{
    Q_D(VStorageAccess);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::StorageAccess *, d->backendObject(), false, teardown());
}

bool VStorageAccess::isIgnored() const
{
    Q_D(const VStorageAccess);
    return_VIBE_HARDWARE_CALL(VHardware::Ifaces::StorageAccess *, d->backendObject(), true, isIgnored());
}

#include "vstorageaccess.moc"
