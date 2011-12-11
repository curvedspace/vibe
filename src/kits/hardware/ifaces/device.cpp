/*
    Copyright 2005 Kevin Ottens <ervin@kde.org>

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

#include "ifaces/device.h"

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>

VHardware::Ifaces::Device::Device(QObject *parent)
    : QObject(parent)
{

}

VHardware::Ifaces::Device::~Device()
{

}

QString VHardware::Ifaces::Device::parentUdi() const
{
    return QString();
}

void VHardware::Ifaces::Device::registerAction(const QString &actionName,
                                               QObject *dest,
                                               const char *requestSlot,
                                               const char *doneSlot) const
{
    QDBusConnection::sessionBus().connect(QString(), deviceDBusPath(),
                                          "org.vision.Vibe.Hardware.Device", actionName + "Requested",
                                          dest, requestSlot);

    QDBusConnection::sessionBus().connect(QString(), deviceDBusPath(),
                                          "org.vision.Vibe.Hardware.Device", actionName + "Done",
                                          dest, doneSlot);
}

void VHardware::Ifaces::Device::broadcastActionDone(const QString &actionName,
                                                    int error, const QString &errorString) const
{
    QDBusMessage signal = QDBusMessage::createSignal(deviceDBusPath(), "org.vision.Vibe.Hardware.Device", actionName + "Done");
    signal << error << errorString;

    QDBusConnection::sessionBus().send(signal);
}

void VHardware::Ifaces::Device::broadcastActionRequested(const QString &actionName) const
{
    QDBusMessage signal = QDBusMessage::createSignal(deviceDBusPath(), "org.vision.Vibe.Hardware.Device", actionName + "Requested");
    QDBusConnection::sessionBus().send(signal);
}

QString VHardware::Ifaces::Device::deviceDBusPath() const
{
    const QByteArray encodedUdi = udi().toUtf8().toPercentEncoding(QByteArray(), ".~", '_');
    return QString("/org/vision/Vibe/Hardware/Device_") + QString::fromLatin1(encodedUdi);
}

#include "ifaces/device.moc"
