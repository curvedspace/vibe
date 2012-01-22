/*
    Copyright 2007 Kevin Ottens <ervin@kde.org>
    Copyright 2007 Christoph Pfister <christophpfister@gmail.com>
    Copyright 2010 Pino Toscano <pino@kde.org>

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

#include "udevdvbinterface.h"

#include "udevdevice.h"

using namespace VHardware::Backends::UDev;

DvbInterface::DvbInterface(UDevDevice *device)
    : DeviceInterface(device)
{

}

DvbInterface::~DvbInterface()
{

}

QString DvbInterface::device() const
{
    return m_device->property("DEVNAME").toString();
}

int DvbInterface::deviceAdapter() const
{
    bool ok;
    int adapter = m_device->property("DVB_ADAPTER_NUM").toString().toInt(&ok, 10);
    if (ok)
        return adapter;
    else
        return -1;
}

VDvbInterface::DeviceType DvbInterface::deviceType() const
{
    VDvbInterface::DeviceType type = VDvbInterface::DvbUnknown;
    const QString typeString = m_device->property("DVB_DEVICE_TYPE").toString();

    if (typeString == QLatin1String("audio")) {
        type = VDvbInterface::DvbAudio;
    } else if (typeString == QLatin1String("ca")) {
        type = VDvbInterface::DvbCa;
    } else if (typeString == QLatin1String("demux")) {
        type = VDvbInterface::DvbDemux;
    } else if (typeString == QLatin1String("dvr")) {
        type = VDvbInterface::DvbDvr;
    } else if (typeString == QLatin1String("frontend")) {
        type = VDvbInterface::DvbFrontend;
    } else if (typeString == QLatin1String("net")) {
        type = VDvbInterface::DvbNet;
    } else if (typeString == QLatin1String("osd")) {
        type = VDvbInterface::DvbOsd;
    } else if (typeString == QLatin1String("sec")) {
        type = VDvbInterface::DvbSec;
    } else if (typeString == QLatin1String("video")) {
        type = VDvbInterface::DvbVideo;
    }

    return type;
}

int DvbInterface::deviceIndex() const
{
    bool ok;
    int index = m_device->property("DVB_DEVICE_NUM").toString().toInt(&ok, 10);
    if (ok)
        return index;
    else
        return -1;
}

#include "backends/udev/moc_udevdvbinterface.cpp"
