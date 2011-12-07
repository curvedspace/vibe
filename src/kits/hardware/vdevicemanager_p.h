/*
    Copyright 2005-2007 Kevin Ottens <ervin@kde.org>

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

#ifndef VDEVICEMANAGER_P_H
#define VDEVICEMANAGER_P_H

#include <QtCore/QMap>
#include <QtCore/QWeakPointer>
#include <QtCore/QSharedData>
#include <QtCore/QThreadStorage>

#include "vmanagerbase_p.h"
#include "vdevicenotifier.h"

namespace VHardware
{
    namespace Ifaces
    {
        class Device;
    }
}

class VDevicePrivate;

class VIBE_EXPORT VDeviceManagerPrivate : public VDeviceNotifier, public VManagerBasePrivate
{
    Q_OBJECT
public:
    VDeviceManagerPrivate();
    ~VDeviceManagerPrivate();

    VDevicePrivate *findRegisteredDevice(const QString &udi);

private Q_SLOTS:
    void _q_deviceAdded(const QString &udi);
    void _q_deviceRemoved(const QString &udi);
    void _q_destroyed(QObject *object);

private:
    VHardware::Ifaces::Device *createBackendObject(const QString &udi);

    QExplicitlySharedDataPointer<VDevicePrivate> m_nullDevice;
    QMap<QString, QWeakPointer<VDevicePrivate> > m_devicesMap;
    QMap<QObject *, QString> m_reverseMap;
};

class VIBE_EXPORT VDeviceManagerStorage
{
public:
    VDeviceManagerStorage();

    QList<QObject *> managerBackends();
    VDeviceNotifier *notifier();

private:
    void ensureManagerCreated();

    QThreadStorage<VDeviceManagerPrivate *> m_storage;
};

#endif // VDEVICEMANAGER_P_H
