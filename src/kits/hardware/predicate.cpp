/*
    Copyright 2006 Kevin Ottens <ervin@kde.org>

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

#include "predicate.h"

#include <Qube/Hardware/device.h>
#include <Qube/Hardware/deviceinterface.h>
#include <QtCore/QStringList>
#include <QtCore/QMetaEnum>

namespace Qube
{
    namespace Hardware
    {
        class Predicate::Private
        {
        public:

            Private() : isValid(false), type(PropertyCheck),
                compOperator(Predicate::Equals),
                operand1(0), operand2(0) {}

            bool isValid;
            Type type;

            DeviceInterface::Type ifaceType;
            QString property;
            QVariant value;
            Predicate::ComparisonOperator compOperator;

            Predicate *operand1;
            Predicate *operand2;
        };
    }
}


Qube::Hardware::Predicate::Predicate()
    : d(new Private())
{
}

Qube::Hardware::Predicate::Predicate(const Predicate &other)
    : d(new Private())
{
    *this = other;
}

Qube::Hardware::Predicate::Predicate(const DeviceInterface::Type &ifaceType,
                                     const QString &property, const QVariant &value,
                                     ComparisonOperator compOperator)
    : d(new Private())
{
    d->isValid = true;
    d->ifaceType = ifaceType;
    d->property = property;
    d->value = value;
    d->compOperator = compOperator;
}

Qube::Hardware::Predicate::Predicate(const QString &ifaceName,
                                     const QString &property, const QVariant &value,
                                     ComparisonOperator compOperator)
    : d(new Private())
{
    DeviceInterface::Type ifaceType = DeviceInterface::stringToType(ifaceName);

    if (((int)ifaceType)!=-1) {
        d->isValid = true;
        d->ifaceType = ifaceType;
        d->property = property;
        d->value = value;
        d->compOperator = compOperator;
    }
}

Qube::Hardware::Predicate::Predicate(const DeviceInterface::Type &ifaceType)
    : d(new Private())
{
    d->isValid = true;
    d->type = InterfaceCheck;
    d->ifaceType = ifaceType;
}

Qube::Hardware::Predicate::Predicate(const QString &ifaceName)
    : d(new Private())
{
    DeviceInterface::Type ifaceType = DeviceInterface::stringToType(ifaceName);

    if (((int)ifaceType)!=-1) {
        d->isValid = true;
        d->type = InterfaceCheck;
        d->ifaceType = ifaceType;
    }
}

Qube::Hardware::Predicate::~Predicate()
{
    if (d->type!=PropertyCheck && d->type!=InterfaceCheck) {
        delete d->operand1;
        delete d->operand2;
    }

    delete d;
}

Qube::Hardware::Predicate &Qube::Hardware::Predicate::operator=(const Predicate &other)
{
    d->isValid = other.d->isValid;
    d->type = other.d->type;

    if (d->type!=PropertyCheck && d->type!=InterfaceCheck) {
        Predicate* operand1 = new Predicate(*(other.d->operand1));
        delete d->operand1;
        d->operand1 = operand1;
        Predicate* operand2 = new Predicate(*(other.d->operand2));
        delete d->operand2;
        d->operand2 = operand2;
    } else {
        d->ifaceType = other.d->ifaceType;
        d->property = other.d->property;
        d->value = other.d->value;
        d->compOperator = other.d->compOperator;
    }

    return *this;
}

Qube::Hardware::Predicate Qube::Hardware::Predicate::operator &(const Predicate &other)
{
    Predicate result;

    result.d->isValid = true;
    result.d->type = Conjunction;
    result.d->operand1 = new Predicate(*this);
    result.d->operand2 = new Predicate(other);

    return result;
}

Qube::Hardware::Predicate &Qube::Hardware::Predicate::operator &=(const Predicate &other)
{
    *this = *this & other;
    return *this;
}

Qube::Hardware::Predicate Qube::Hardware::Predicate::operator|(const Predicate &other)
{
    Predicate result;

    result.d->isValid = true;
    result.d->type = Disjunction;
    result.d->operand1 = new Predicate(*this);
    result.d->operand2 = new Predicate(other);

    return result;
}

Qube::Hardware::Predicate &Qube::Hardware::Predicate::operator |=(const Predicate &other)
{
    *this = *this | other;
    return *this;
}

bool Qube::Hardware::Predicate::isValid() const
{
    return d->isValid;
}

bool Qube::Hardware::Predicate::matches(const Device &device) const
{
    if (!d->isValid) return false;

    switch(d->type) {
    case Disjunction:
        return d->operand1->matches(device)
               || d->operand2->matches(device);
    case Conjunction:
        return d->operand1->matches(device)
               && d->operand2->matches(device);
    case PropertyCheck: {
        const DeviceInterface *iface = device.asDeviceInterface(d->ifaceType);

        if (iface!=0) {
            const int index = iface->metaObject()->indexOfProperty(d->property.toLatin1());
            QMetaProperty metaProp = iface->metaObject()->property(index);
            QVariant value = metaProp.isReadable() ? metaProp.read(iface) : QVariant();
            QVariant expected = d->value;

            if (metaProp.isEnumType() && expected.type()==QVariant::String) {
                QMetaEnum metaEnum = metaProp.enumerator();
                int value = metaEnum.keysToValue(d->value.toString().toLatin1());
                if (value>=0) { // No value found for these keys, resetting expected to invalid
                    expected = value;
                } else {
                    expected = QVariant();
                }
            }

            if (d->compOperator==Mask) {
                bool v_ok;
                int v = value.toInt(&v_ok);
                bool e_ok;
                int e = expected.toInt(&e_ok);

                return (e_ok && v_ok && (v &e));
            } else {
                return (value == expected);
            }
        }
        break;
    }
    case InterfaceCheck:
        return device.isDeviceInterface(d->ifaceType);
    }

    return false;
}

QSet<Qube::Hardware::DeviceInterface::Type> Qube::Hardware::Predicate::usedTypes() const
{
    QSet<DeviceInterface::Type> res;

    if (d->isValid) {

        switch(d->type) {
        case Disjunction:
        case Conjunction:
            res+= d->operand1->usedTypes();
            res+= d->operand2->usedTypes();
            break;
        case PropertyCheck:
        case InterfaceCheck:
            res << d->ifaceType;
            break;
        }

    }

    return res;
}


QString Qube::Hardware::Predicate::toString() const
{
    if (!d->isValid) return "False";

    if (d->type!=PropertyCheck && d->type!=InterfaceCheck) {
        QString op = " AND ";
        if (d->type==Disjunction) op = " OR ";

        return '['+d->operand1->toString()+op+d->operand2->toString()+']';
    } else {
        QString ifaceName = DeviceInterface::typeToString(d->ifaceType);

        if (ifaceName.isEmpty()) ifaceName = "Unknown";

        if (d->type==InterfaceCheck) {
            return "IS "+ifaceName;
        }

        QString value;

        switch (d->value.type()) {
        case QVariant::StringList: {
            value = '{';

            const QStringList list = d->value.toStringList();

            QStringList::ConstIterator it = list.begin();
            QStringList::ConstIterator end = list.end();

            for (; it!=end; ++it) {
                value+= '\''+ *it+'\'';

                if (it+1!=end) {
                    value+= ", ";
                }
            }

            value+= '}';
            break;
        }
        case QVariant::Bool:
            value = (d->value.toBool()?"true":"false");
            break;
        case QVariant::Int:
        case QVariant::UInt:
        case QVariant::LongLong:
        case QVariant::ULongLong:
            value = d->value.toString();
            break;
        default:
            value = '\''+d->value.toString()+'\'';
            break;
        }

        QString str_operator = "==";
        if (d->compOperator!=Equals) str_operator = " &";


        return ifaceName+'.'+d->property+' '+str_operator+' '+value;
    }
}

Qube::Hardware::Predicate::Type Qube::Hardware::Predicate::type() const
{
    return d->type;
}

Qube::Hardware::DeviceInterface::Type Qube::Hardware::Predicate::interfaceType() const
{
    return d->ifaceType;
}

QString Qube::Hardware::Predicate::propertyName() const
{
    return d->property;
}

QVariant Qube::Hardware::Predicate::matchingValue() const
{
    return d->value;
}

Qube::Hardware::Predicate::ComparisonOperator Qube::Hardware::Predicate::comparisonOperator() const
{
    return d->compOperator;
}

Qube::Hardware::Predicate Qube::Hardware::Predicate::firstOperand() const
{
    if( d->operand1 ) {
        return *d->operand1;
    }
    return Predicate();
}

Qube::Hardware::Predicate Qube::Hardware::Predicate::secondOperand() const
{
    if( d->operand2 ) {
        return *d->operand2;
    }
    return Predicate();
}

