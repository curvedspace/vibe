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

extern "C"
{
#include "predicateparse.h"

    void PredicateParse_mainParse(const char *_code);
}

#include "predicate.h"
#include "soliddefs_p.h"

#include <stdlib.h>

#include <QtCore/QStringList>
#include <QtCore/QThreadStorage>

namespace Qube
{
    namespace Hardware
    {
        namespace PredicateParse
        {

            struct ParsingData {
                ParsingData()
                    : result(0)
                {}

                Qube::Hardware::Predicate *result;
                QByteArray buffer;
            };

        }
    }
}

SOLID_GLOBAL_STATIC(QThreadStorage<Qube::Hardware::PredicateParse::ParsingData *>, s_parsingData)

Qube::Hardware::Predicate Qube::Hardware::Predicate::fromString(const QString &predicate)
{
    Qube::Hardware::PredicateParse::ParsingData *data = new Qube::Hardware::PredicateParse::ParsingData();
    s_parsingData->setLocalData(data);
    data->buffer = predicate.toAscii();
    PredicateParse_mainParse(data->buffer.constData());
    Predicate result;
    if (data->result) {
        result = Predicate(*data->result);
        delete data->result;
    }
    s_parsingData->setLocalData(0);
    return result;
}


void PredicateParse_setResult(void *result)
{
    Qube::Hardware::PredicateParse::ParsingData *data = s_parsingData->localData();
    data->result = (Qube::Hardware::Predicate *) result;
}

void PredicateParse_errorDetected(const char* s)
{
    qWarning("ERROR from solid predicate parser: %s", s);
    s_parsingData->localData()->result = 0;
}

void PredicateParse_destroy(void *pred)
{
    Qube::Hardware::PredicateParse::ParsingData *data = s_parsingData->localData();
    Qube::Hardware::Predicate *p = (Qube::Hardware::Predicate *) pred;
    if (p != data->result) {
        delete p;
    }
}

void *PredicateParse_newAtom(char *interface, char *property, void *value)
{
    QString iface(interface);
    QString prop(property);
    QVariant *val = (QVariant *)value;

    Qube::Hardware::Predicate *result = new Qube::Hardware::Predicate(iface, prop, *val);

    delete val;
    free(interface);
    free(property);

    return result;
}

void *PredicateParse_newMaskAtom(char *interface, char *property, void *value)
{
    QString iface(interface);
    QString prop(property);
    QVariant *val = (QVariant *)value;

    Qube::Hardware::Predicate *result = new Qube::Hardware::Predicate(iface, prop, *val, Qube::Hardware::Predicate::Mask);

    delete val;
    free(interface);
    free(property);

    return result;
}


void *PredicateParse_newIsAtom(char *interface)
{
    QString iface(interface);

    Qube::Hardware::Predicate *result = new Qube::Hardware::Predicate(iface);

    free(interface);

    return result;
}


void *PredicateParse_newAnd(void *pred1, void *pred2)
{
    Qube::Hardware::Predicate *result = new Qube::Hardware::Predicate();

    Qube::Hardware::PredicateParse::ParsingData *data = s_parsingData->localData();
    Qube::Hardware::Predicate *p1 = (Qube::Hardware::Predicate *)pred1;
    Qube::Hardware::Predicate *p2 = (Qube::Hardware::Predicate *)pred2;

    if (p1==data->result || p2==data->result) {
        data->result = 0;
    }

    *result = *p1 & *p2;

    delete p1;
    delete p2;

    return result;
}


void *PredicateParse_newOr(void *pred1, void *pred2)
{
    Qube::Hardware::Predicate *result = new Qube::Hardware::Predicate();

    Qube::Hardware::PredicateParse::ParsingData *data = s_parsingData->localData();
    Qube::Hardware::Predicate *p1 = (Qube::Hardware::Predicate *)pred1;
    Qube::Hardware::Predicate *p2 = (Qube::Hardware::Predicate *)pred2;

    if (p1==data->result || p2==data->result) {
        data->result = 0;
    }

    *result = *p1 | *p2;

    delete p1;
    delete p2;

    return result;
}


void *PredicateParse_newStringValue(char *val)
{
    QString s(val);

    free(val);

    return new QVariant(s);
}


void *PredicateParse_newBoolValue(int val)
{
    bool b = (val != 0);
    return new QVariant(b);
}


void *PredicateParse_newNumValue(int val)
{
    return new QVariant(val);
}


void *PredicateParse_newDoubleValue(double val)
{
    return new QVariant(val);
}


void *PredicateParse_newEmptyStringListValue()
{
    return new QVariant(QStringList());
}


void *PredicateParse_newStringListValue(char *name)
{
    QStringList list;
    list << QString(name);

    free(name);

    return new QVariant(list);
}


void *PredicateParse_appendStringListValue(char *name, void *list)
{
    QVariant *variant = (QVariant *)list;

    QStringList new_list = variant->toStringList();

    new_list << QString(name);

    delete variant;
    free(name);

    return new QVariant(new_list);
}

void PredicateLexer_unknownToken(const char* text)
{
    qWarning("ERROR from solid predicate parser: unrecognized token '%s' in predicate '%s'\n",
             text, s_parsingData->localData()->buffer.constData());
}
