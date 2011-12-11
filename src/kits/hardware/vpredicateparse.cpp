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
#include "vpredicateparse.h"

    void VPredicateParse_mainParse(const char *_code);
}

#include "vpredicate.h"

#include <stdlib.h>

#include <QtCore/QStringList>
#include <QtCore/QThreadStorage>

#include <VibeCore/VGlobal>

namespace VPredicateParse
{
    struct ParsingData {
        ParsingData()
            : result(0)
        {}

        VPredicate *result;
        QByteArray buffer;
    };
}

VIBE_GLOBAL_STATIC(QThreadStorage<VPredicateParse::ParsingData *>, s_parsingData)

VPredicate VPredicate::fromString(const QString &predicate)
{
    VPredicateParse::ParsingData *data = new VPredicateParse::ParsingData();
    s_parsingData->setLocalData(data);
    data->buffer = predicate.toAscii();
    VPredicateParse_mainParse(data->buffer.constData());
    VPredicate result;
    if (data->result) {
        result = VPredicate(*data->result);
        delete data->result;
    }
    s_parsingData->setLocalData(0);
    return result;
}

void VPredicateParse_setResult(void *result)
{
    VPredicateParse::ParsingData *data = s_parsingData->localData();
    data->result = (VPredicate *) result;
}

void VPredicateParse_errorDetected(const char *s)
{
    qWarning("ERROR from VibeHardware predicate parser: %s", s);
    s_parsingData->localData()->result = 0;
}

void VPredicateParse_destroy(void *pred)
{
    VPredicateParse::ParsingData *data = s_parsingData->localData();
    VPredicate *p = (VPredicate *) pred;
    if (p != data->result) {
        delete p;
    }
}

void *VPredicateParse_newAtom(char *interface, char *property, void *value)
{
    QString iface(interface);
    QString prop(property);
    QVariant *val = (QVariant *)value;

    VPredicate *result = new VPredicate(iface, prop, *val);

    delete val;
    free(interface);
    free(property);

    return result;
}

void *VPredicateParse_newMaskAtom(char *interface, char *property, void *value)
{
    QString iface(interface);
    QString prop(property);
    QVariant *val = (QVariant *)value;

    VPredicate *result = new VPredicate(iface, prop, *val, VPredicate::Mask);

    delete val;
    free(interface);
    free(property);

    return result;
}

void *VPredicateParse_newIsAtom(char *interface)
{
    QString iface(interface);

    VPredicate *result = new VPredicate(iface);

    free(interface);

    return result;
}

void *VPredicateParse_newAnd(void *pred1, void *pred2)
{
    VPredicate *result = new VPredicate();

    VPredicateParse::ParsingData *data = s_parsingData->localData();
    VPredicate *p1 = (VPredicate *)pred1;
    VPredicate *p2 = (VPredicate *)pred2;

    if (p1 == data->result || p2 == data->result) {
        data->result = 0;
    }

    *result = *p1 & *p2;

    delete p1;
    delete p2;

    return result;
}

void *VPredicateParse_newOr(void *pred1, void *pred2)
{
    VPredicate *result = new VPredicate();

    VPredicateParse::ParsingData *data = s_parsingData->localData();
    VPredicate *p1 = (VPredicate *)pred1;
    VPredicate *p2 = (VPredicate *)pred2;

    if (p1 == data->result || p2 == data->result)
        data->result = 0;

    *result = *p1 | *p2;

    delete p1;
    delete p2;

    return result;
}

void *VPredicateParse_newStringValue(char *val)
{
    QString s(val);

    free(val);

    return new QVariant(s);
}

void *VPredicateParse_newBoolValue(int val)
{
    bool b = (val != 0);
    return new QVariant(b);
}

void *VPredicateParse_newNumValue(int val)
{
    return new QVariant(val);
}

void *VPredicateParse_newDoubleValue(double val)
{
    return new QVariant(val);
}

void *VPredicateParse_newEmptyStringListValue()
{
    return new QVariant(QStringList());
}

void *VPredicateParse_newStringListValue(char *name)
{
    QStringList list;
    list << QString(name);

    free(name);

    return new QVariant(list);
}

void *VPredicateParse_appendStringListValue(char *name, void *list)
{
    QVariant *variant = (QVariant *)list;

    QStringList new_list = variant->toStringList();

    new_list << QString(name);

    delete variant;
    free(name);

    return new QVariant(new_list);
}

void VPredicateLexer_unknownToken(const char *text)
{
    qWarning("ERROR from VibeHardware predicate parser: unrecognized token '%s' in predicate '%s'\n",
             text, s_parsingData->localData()->buffer.constData());
}
