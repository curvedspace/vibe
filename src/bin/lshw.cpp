/****************************************************************************
 * This file is part of Qube.
 *
 * Copyright (c) 2006 Kevin Ottens
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *	Kevin Ottens <ervin@kde.org>
 *
 * Qube is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Qube is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Qube.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QString>
#include <QStringList>
#include <QMetaProperty>
#include <QMetaEnum>
#include <QTimer>

#include <Qube/Hardware/Device>
#include <Qube/Hardware/GenericInterface>
#include <Qube/Hardware/StorageAccess>
#include <Qube/Hardware/OpticalDrive>
#include <Qube/Core/CommandOptions>

#include "lshw.h"

#include <iostream>
using namespace std;

std::ostream &operator<<(std::ostream &out, const QString &msg)
{
    return (out << msg.toLocal8Bit().constData());
}

std::ostream &operator<<(std::ostream &out, const QVariant &value)
{
    switch (value.type()) {
    case QVariant::StringList: {
        out << "{";

        const QStringList list = value.toStringList();

        QStringList::ConstIterator it = list.constBegin();
        QStringList::ConstIterator end = list.constEnd();

        for (; it!=end; ++it) {
            out << "'" << *it << "'";

            if (it+1!=end) {
                out << ", ";
            }
        }

        out << "}  (string list)";
        break;
    }
    case QVariant::Bool:
        out << (value.toBool()?"true":"false") << "  (bool)";
        break;
    case QVariant::Int:
    case QVariant::LongLong:
        out << value.toString()
            << "  (0x" << QString::number(value.toLongLong(), 16) << ")  (" << QVariant::typeToName(value.type()) << ")";
        break;
    case QVariant::UInt:
    case QVariant::ULongLong:
        out << value.toString()
            << "  (0x" << QString::number(value.toULongLong(), 16) << ")  (" << QVariant::typeToName(value.type()) << ")";
        break;
    case QVariant::UserType: {
        //qDebug() << "got variant type:" << value.typeName();
        if (value.canConvert<QList<int> >()) {
            QList<int> intlist = value.value<QList<int> >();
            QStringList tmp;
            foreach (int val, intlist)
            tmp.append(QString::number(val));
            out << "{" << tmp.join(",") << "} (int list)";
        }
        break;
    }
    default:
        out << "'" << value.toString() << "'  (string)";
        break;
    }

    return out;
}

std::ostream &operator<<(std::ostream &out, const Qube::Hardware::Device &device)
{
    out << "  parent = " << QVariant(device.parentUdi()) << endl;
    out << "  vendor = " << QVariant(device.vendor()) << endl;
    out << "  product = " << QVariant(device.product()) << endl;
    out << "  description = " << QVariant(device.description()) << endl;

    int index = Qube::Hardware::DeviceInterface::staticMetaObject.indexOfEnumerator("Type");
    QMetaEnum typeEnum = Qube::Hardware::DeviceInterface::staticMetaObject.enumerator(index);

    for (int i=0; i<typeEnum.keyCount(); i++) {
        Qube::Hardware::DeviceInterface::Type type = (Qube::Hardware::DeviceInterface::Type)typeEnum.value(i);
        const Qube::Hardware::DeviceInterface *interface = device.asDeviceInterface(type);

        if (interface) {
            const QMetaObject *meta = interface->metaObject();

            for (int i=meta->propertyOffset(); i<meta->propertyCount(); i++) {
                QMetaProperty property = meta->property(i);
                out << "  " << QString(meta->className()).mid(7) << "." << property.name()
                    << " = ";

                QVariant value = property.read(interface);

                if (property.isEnumType()) {
                    QMetaEnum metaEnum = property.enumerator();
                    if (metaEnum.isFlag()) {
                        out << "'" << metaEnum.valueToKeys(value.toInt()).constData() << "'"
                            << "  (0x" << QString::number(value.toInt(), 16) << ")  (flag)";
                    } else {
                        out << "'" << metaEnum.valueToKey(value.toInt()) << "'"
                            << "  (0x" << QString::number(value.toInt(), 16) << ")  (enum)";
                    }
                    out << endl;
                } else {
                    out << value << endl;
                }
            }
        }
    }

    return out;
}

std::ostream &operator<<(std::ostream &out, const QMap<QString,QVariant> &properties)
{
    foreach (const QString &key, properties.keys()) {
        out << "  " << key << " = " << properties[key] << endl;
    }

    return out;
}

int main(int argc, char **argv)
{
    HardwareConsole shell(argc, argv);

    Qube::Core::CommandOptions options;
    options.setParamStyle(Qube::Core::CommandOptions::Equals);
    options.add("list", QT_TRANSLATE_NOOP("HardwareShell",
                                          "list the hardware available in the system"));
    options.alias("list", "l");
    options.add("list-capabilities", QT_TRANSLATE_NOOP("HardwareShell",
                "list the hardware available in the system, the device interfaces and "
                "the corresponding properties are listed in a platform natural fashion"));
    options.add("list-properties", QT_TRANSLATE_NOOP("HardwareShell",
                "list the hardware available in the system, the device properties are "
                "listed (be careful, in this case property names are backend dependent)"));
    options.add("capabilities", QT_TRANSLATE_NOOP("HardwareShell", "display all the interfaces "
                "and properties of the device corresponding to 'x' in a platform neutral fashion"),
                Qube::Core::CommandOptions::ValueRequired);
    options.alias("capabilities", "c");
    options.add("properties", QT_TRANSLATE_NOOP("HardwareShell",
                "display all the properties of the device corresponding to 'x' (be careful, "
                "in this case property names are backend dependent)"),
                Qube::Core::CommandOptions::ValueRequired);
    options.alias("properties", "p");
    options.add("query", QT_TRANSLATE_NOOP("HardwareShell",
                                           "list the UDI of devices corresponding to 'x'"),
                Qube::Core::CommandOptions::ValueRequired);
    options.alias("query", "q");
    options.add("parent", QT_TRANSLATE_NOOP("HardwareShell",
                                            "specify parent 'x' to restrict the search to the branch of the "
                                            "corresponding device, while performing a query"),
                Qube::Core::CommandOptions::ValueRequired);
    options.add("mount", QT_TRANSLATE_NOOP("HardwareShell",
                                           "mount the device corresponding to 'x'"),
                Qube::Core::CommandOptions::ValueRequired);
    options.add("unmount", QT_TRANSLATE_NOOP("HardwareShell",
                "unmount the device corresponding to 'x'"),
                Qube::Core::CommandOptions::ValueRequired);
    options.add("eject", QT_TRANSLATE_NOOP("HardwareShell",
                                           "eject the device corresponding to 'x'"),
                Qube::Core::CommandOptions::ValueRequired);
    options.add("help", QT_TRANSLATE_NOOP("HardwareShell", "show this help text"));
    options.alias("help", "h");
    options.parse(QCoreApplication::arguments());

    if (options.count("help") || options.showUnrecognizedWarning()) {
        options.showUsage();
        return 1;
    }

    if (options.count("list") || options.count("list-capabilities")
        || options.count("list-properties")) {
        bool details = options.count("list-capabilities") > 0;
        bool nonportableinfo = options.count("list-properties") > 0;
        return shell.hwList(details, nonportableinfo) ? 0 : 1;
    } else if (options.count("capabilities")) {
        return shell.hwCapabilities(options.value("capabilities").toString()) ? 0 : 1;
    } else if (options.count("properties")) {
        return shell.hwProperties(options.value("properties").toString()) ? 0 : 1;
    } else if (options.count("query")) {
        QString parent = options.count("parent") ? options.value("parent").toString() : QString::null;
        QString query = options.value("query").toString();
        return shell.hwQuery(parent, query) ? 0 : 1;
    } else if (options.count("mount")) {
        return shell.hwVolumeCall(HardwareConsole::Mount, options.value("mount").toString()) ? 0 : 1;
    } else if (options.count("unmount")) {
        return shell.hwVolumeCall(HardwareConsole::Unmount, options.value("unmount").toString()) ? 0 : 1;
    } else if (options.count("eject")) {
        return shell.hwVolumeCall(HardwareConsole::Eject, options.value("eject").toString()) ? 0 : 1;
    } else {
        options.showUsage();
        return 1;
    }

    return 0;
}

HardwareConsole::HardwareConsole(int &argc, char *argv[]) :
    QCoreApplication(argc, argv),
    m_error(0)
{
}

bool HardwareConsole::hwList(bool interfaces, bool system)
{
    const QList<Qube::Hardware::Device> all = Qube::Hardware::Device::allDevices();

    foreach (const Qube::Hardware::Device &device, all) {
        cout << "udi = '" << device.udi() << "'" << endl;

        if (interfaces) {
            cout << device << endl;
        } else if (system && device.is<Qube::Hardware::GenericInterface>()) {
            QMap<QString,QVariant> properties = device.as<Qube::Hardware::GenericInterface>()->allProperties();
            cout << properties << endl;
        }
    }

    return true;
}

bool HardwareConsole::hwCapabilities(const QString &udi)
{
    const Qube::Hardware::Device device(udi);

    cout << "udi = '" << device.udi() << "'" << endl;
    cout << device << endl;

    return true;
}

bool HardwareConsole::hwProperties(const QString &udi)
{
    const Qube::Hardware::Device device(udi);

    cout << "udi = '" << device.udi() << "'" << endl;
    if (device.is<Qube::Hardware::GenericInterface>()) {
        QMap<QString,QVariant> properties = device.as<Qube::Hardware::GenericInterface>()->allProperties();
        cout << properties << endl;
    }

    return true;
}

bool HardwareConsole::hwQuery(const QString &parentUdi, const QString &query)
{
    const QList<Qube::Hardware::Device> devices =
        Qube::Hardware::Device::listFromQuery(query, parentUdi);

    foreach (const Qube::Hardware::Device &device, devices) {
        cout << "udi = '" << device.udi() << "'" << endl;
    }

    return true;
}

bool HardwareConsole::hwVolumeCall(HardwareConsole::VolumeCallType type, const QString &udi)
{
    Qube::Hardware::Device device(udi);

    if (!device.is<Qube::Hardware::StorageAccess>() && type!=Eject) {
        cerr << tr("Error: %1 does not have the interface StorageAccess.").arg(udi) << endl;
        return false;
    } else if (!device.is<Qube::Hardware::OpticalDrive>() && type==Eject) {
        cerr << tr("Error: %1 does not have the interface OpticalDrive.").arg(udi) << endl;
        return false;
    }

    switch (type) {
    case Mount:
        connect(device.as<Qube::Hardware::StorageAccess>(),
                SIGNAL(setupDone(Qube::Hardware::ErrorType, QVariant, const QString &)),
                this,
                SLOT(slotStorageResult(Qube::Hardware::ErrorType, QVariant)));
        device.as<Qube::Hardware::StorageAccess>()->setup();
        break;
    case Unmount:
        connect(device.as<Qube::Hardware::StorageAccess>(),
                SIGNAL(teardownDone(Qube::Hardware::ErrorType, QVariant, const QString &)),
                this,
                SLOT(slotStorageResult(Qube::Hardware::ErrorType, QVariant)));
        device.as<Qube::Hardware::StorageAccess>()->teardown();
        break;
    case Eject:
        connect(device.as<Qube::Hardware::OpticalDrive>(),
                SIGNAL(ejectDone(Qube::Hardware::ErrorType, QVariant, const QString &)),
                this,
                SLOT(slotStorageResult(Qube::Hardware::ErrorType, QVariant)));
        device.as<Qube::Hardware::OpticalDrive>()->eject();
        break;
    }

    m_loop.exec();

    if (m_error) {
        cerr << tr("Error: %1").arg(m_errorString) << endl;
        return false;
    }

    return true;
}

void HardwareConsole::slotStorageResult(Qube::Hardware::ErrorType error, const QVariant &errorData)
{
    if (error) {
        m_error = 1;
        m_errorString = errorData.toString();
    }

    m_loop.exit();
}

#include "lshw.moc"
