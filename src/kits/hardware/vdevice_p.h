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

#ifndef VDEVICE_P_H
#define VDEVICE_P_H

#include <QtCore/QObject>
#include <QtCore/QSharedData>
#include <QtCore/QWeakPointer>

#include <VibeCore/VGlobal>

namespace VHardware
{
    namespace Ifaces
    {
        class Device;
    }
}

class VIBE_EXPORT VDevicePrivate : public QObject, public QSharedData
{
    Q_OBJECT
public:
    explicit VDevicePrivate(const QString &udi);
    ~VDevicePrivate();

    QString udi() const {
        return m_udi;
    }

    VHardware::Ifaces::Device *backendObject() const {
        return m_backendObject.data();
    }
    void setBackendObject(VHardware::Ifaces::Device *object);

    VDeviceInterface *interface(const VDeviceInterface::Type &type) const;
    void setInterface(const VDeviceInterface::Type &type, VDeviceInterface *interface);

public Q_SLOTS:
    void _q_destroyed(QObject *object);

private:
    QString m_udi;
    QWeakPointer<VHardware::Ifaces::Device> m_backendObject;
    QMap<VDeviceInterface::Type, VDeviceInterface *> m_ifaces;
};

#endif // VDEVICE_P_H
