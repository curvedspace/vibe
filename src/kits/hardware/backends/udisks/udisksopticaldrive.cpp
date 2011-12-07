/*
    Copyright 2010 Michael Zanetti <mzanetti@kde.org>
    Copyright 2010-2011 Lukas Tinkl <ltinkl@redhat.com>

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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include <QtCore/QFile>
#include <QtCore/QDebug>

#include "udisksopticaldrive.h"
#include "udisks.h"
#include "udisksdevice.h"

using namespace VHardware::Backends::UDisks;

UDisksOpticalDrive::UDisksOpticalDrive(UDisksDevice *device)
    : UDisksStorageDrive(device), m_ejectInProgress(false), m_readSpeed(0), m_writeSpeed(0), m_speedsInit(false)
{
    m_device->registerAction("eject", this,
                             SLOT(slotEjectRequested()),
                             SLOT(slotEjectDone(int, const QString &)));

    connect(m_device, SIGNAL(changed()), this, SLOT(slotChanged()));
}

UDisksOpticalDrive::~UDisksOpticalDrive()
{

}

bool UDisksOpticalDrive::eject()
{
    if (m_ejectInProgress)
        return false;
    m_ejectInProgress = true;
    m_device->broadcastActionRequested("eject");

    QDBusConnection c = QDBusConnection::systemBus();

    QString path = m_device->udi();

    // check if the device is mounted and call umount if needed
    if (m_device->prop("DeviceIsMounted").toBool()) {
        QDBusMessage msg = QDBusMessage::createMethodCall(UD_DBUS_SERVICE, path, UD_DBUS_INTERFACE_DISKS_DEVICE, "FilesystemUnmount");
        msg << QStringList();   // options, unused now
        c.call(msg, QDBus::NoBlock);
    }

    QDBusMessage msg = QDBusMessage::createMethodCall(UD_DBUS_SERVICE, path, UD_DBUS_INTERFACE_DISKS_DEVICE, "DriveEject");
    msg << QStringList();
    return c.callWithCallback(msg, this, SLOT(slotDBusReply(const QDBusMessage &)), SLOT(slotDBusError(const QDBusError &)));
}

void UDisksOpticalDrive::slotDBusReply(const QDBusMessage &/*reply*/)
{
    m_ejectInProgress = false;
    m_device->broadcastActionDone("eject");
}

void UDisksOpticalDrive::slotDBusError(const QDBusError &error)
{
    m_ejectInProgress = false;
    m_device->broadcastActionDone("eject", m_device->errorToVibeHardwareError(error.name()),
                                  m_device->errorToString(error.name()) + ": " + error.message());
}

void UDisksOpticalDrive::slotEjectRequested()
{
    m_ejectInProgress = true;
    emit ejectRequested(m_device->udi());
}

void UDisksOpticalDrive::slotEjectDone(int error, const QString &errorString)
{
    m_ejectInProgress = false;
    emit ejectDone(static_cast<VHardware::ErrorType>(error), errorString, m_device->udi());
}

void UDisksOpticalDrive::initReadWriteSpeeds() const
{
#if 0
    int read_speed, write_speed;
    char *write_speeds = 0;
    QByteArray device_file = QFile::encodeName(m_device->property("DeviceFile").toString());

    //qDebug("Doing open (\"%s\", O_RDONLY | O_NONBLOCK)", device_file.constData());
    int fd = open(device_file, O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        qWarning("Cannot open %s: %s", device_file.constData(), strerror(errno));
        return;
    }

    if (get_read_write_speed(fd, &read_speed, &write_speed, &write_speeds) >= 0) {
        m_readSpeed = read_speed;
        m_writeSpeed = write_speed;

        QStringList list = QString::fromLatin1(write_speeds).split(',', QString::SkipEmptyParts);
        foreach(const QString & speed, list)
        m_writeSpeeds.append(speed.toInt());

        free(write_speeds);

        m_speedsInit = true;
    }

    close(fd);
#endif
}

QList<int> UDisksOpticalDrive::writeSpeeds() const
{
    if (!m_speedsInit)
        initReadWriteSpeeds();
    //qDebug() << "VibeHardware write speeds:" << m_writeSpeeds;
    return m_writeSpeeds;
}

int UDisksOpticalDrive::writeSpeed() const
{
    if (!m_speedsInit)
        initReadWriteSpeeds();
    return m_writeSpeed;
}

int UDisksOpticalDrive::readSpeed() const
{
    if (!m_speedsInit)
        initReadWriteSpeeds();
    return m_readSpeed;
}

VOpticalDrive::MediumTypes UDisksOpticalDrive::supportedMedia() const
{
    const QStringList mediaTypes = m_device->prop("DriveMediaCompatibility").toStringList();
    VOpticalDrive::MediumTypes supported;

    QMap<VOpticalDrive::MediumType, QString> map;
    map[VOpticalDrive::Cdr] = "optical_cd_r";
    map[VOpticalDrive::Cdrw] = "optical_cd_rw";
    map[VOpticalDrive::Dvd] = "optical_dvd";
    map[VOpticalDrive::Dvdr] = "optical_dvd_r";
    map[VOpticalDrive::Dvdrw] = "optical_dvd_rw";
    map[VOpticalDrive::Dvdram] = "optical_dvd_ram";
    map[VOpticalDrive::Dvdplusr] = "optical_dvd_plus_r";
    map[VOpticalDrive::Dvdplusrw] = "optical_dvd_plus_rw";
    map[VOpticalDrive::Dvdplusdl] = "optical_dvd_plus_r_dl";
    map[VOpticalDrive::Dvdplusdlrw] = "optical_dvd_plus_rw_dl";
    map[VOpticalDrive::Bd] = "optical_bd";
    map[VOpticalDrive::Bdr] = "optical_bd_r";
    map[VOpticalDrive::Bdre] = "optical_bd_re";
    map[VOpticalDrive::HdDvd] = "optical_hddvd";
    map[VOpticalDrive::HdDvdr] = "optical_hddvd_r";
    map[VOpticalDrive::HdDvdrw] = "optical_hddvd_rw";
    // TODO add these to VibeHardware
    //map[VOpticalDrive::Mo] ="optical_mo";
    //map[VOpticalDrive::Mr] ="optical_mrw";
    //map[VOpticalDrive::Mrw] ="optical_mrw_w";

    foreach(const VOpticalDrive::MediumType & type, map.keys()) {
        if (mediaTypes.contains(map[type])) {
            supported |= type;
        }
    }

    return supported;
}

void UDisksOpticalDrive::slotChanged()
{
    m_speedsInit = false; // reset the read/write speeds, changes eg. with an inserted media
}
