/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2010-2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * Vibe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Vibe.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "vstringhandler.h"

namespace VStringHandler
{
    QString elide(const QString &str, Qt::TextElideMode mode, int maxlen)
    {
        switch (mode) {
            case Qt::ElideLeft:
                if (str.length() > maxlen) {
                    int part = maxlen - 3;
                    return QString::fromLatin1("...") + str.right(part);
                }
                break;
            case Qt::ElideRight:
                if (str.length() > maxlen) {
                    int part = maxlen - 3;
                    return str.left(part) + QLatin1String("...");
                }
                break;
            case Qt::ElideMiddle:
                if (str.length() > maxlen) {
                    const int part = (maxlen - 3) / 2;
                    return str.left(part) + QLatin1String("...") + str.right(part);
                }
                break;
            default:
                break;
        }

        return str;
    }
}
