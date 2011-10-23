/****************************************************************************
 * This file is part of Qube.
 *
 * Copyright (c) 2006-2007 Kevin Ottens
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *	Kevin Ottens <ervin@kde.org>
 *
 * Qube is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Qube is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Qube.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef QUBE_HARDWARE_CALL_P_H
#define QUBE_HARDWARE_CALL_P_H

#include <QtCore/QObject>

#define return_QUBE_HARDWARE_CALL(Type, Object, Default, Method) \
    Type t = qobject_cast<Type>(Object);                         \
    if (t != 0)                                                  \
         return t->Method;                                       \
    return Default;

#define QUBE_HARDWARE_CALL(Type, Object, Method)                 \
    Type t = qobject_cast<Type>(Object);                         \
    if (t != 0)                                                  \
         t->Method;

#endif // QUBE_HARDWARE_CALL_P_H
