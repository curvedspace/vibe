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

#ifndef VPREDICATEPARSE_H
#define VPREDICATEPARSE_H

void VPredicateLexer_unknownToken(const char *text);

void VPredicateParse_setResult(void *result);
void VPredicateParse_errorDetected(const char *error);
void VPredicateParse_destroy(void *pred);

void *VPredicateParse_newAtom(char *interface, char *property, void *value);
void *VPredicateParse_newMaskAtom(char *interface, char *property, void *value);
void *VPredicateParse_newIsAtom(char *interface);
void *VPredicateParse_newAnd(void *pred1, void *pred2);
void *VPredicateParse_newOr(void *pred1, void *pred2);
void *VPredicateParse_newStringValue(char *val);
void *VPredicateParse_newBoolValue(int val);
void *VPredicateParse_newNumValue(int val);
void *VPredicateParse_newDoubleValue(double val);
void *VPredicateParse_newEmptyStringListValue();
void *VPredicateParse_newStringListValue(char *name);
void *VPredicateParse_appendStringListValue(char *name, void *list);

#endif // VPREDICATEPARSE_H
