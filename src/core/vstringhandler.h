/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 1999 Ian Zepp
 * Copyright (c) 2000 Rik Hemsley (rikkus)
 * Copyright (c) 2006 by Dominic Battre
 * Copyright (c) 2006 by Martin Pool
 * Copyright (c) 2010-2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Ian Zepp <icszepp@islc.net>
 *    Rik Hemsley (rikkus) <rik@kde.org>
 *    Dominic Battre <dominic@battre.de>
 *    Martin Pool <mbp@canonical.com>
 *
 * $BEGIN_LICENSE:LGPL2$
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef VSTRINGHANDLER_H
#define VSTRINGHANDLER_H

#include <QString>

#include <VibeCore/VibeCoreExport>

namespace VStringHandler
{
    VIBECORE_EXPORT int naturalCompare(const QString &a, const QString &b,
                                   Qt::CaseSensitivity caseSensitivity = Qt::CaseSensitive);
}

#endif // VSTRINGHANDLER_H
