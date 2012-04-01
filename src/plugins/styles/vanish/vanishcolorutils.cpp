/****************************************************************************
 * This file is part of Vanish Style.
 *
 * Copyright (c) 2011-2012 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vanish Style is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2
 * as published by the Free Software Foundation.
 *
 * Vanish Style is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Vanish Style.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "vanishcolorutils.h"

QColor shadeColor(const QColor &color, qreal factor, qreal alpha)
{
    QColor tmp = color;
    tmp.setRedF(color.redF() * factor);
    tmp.setGreenF(color.greenF() * factor);
    tmp.setBlueF(color.blueF() * factor);
    tmp.setAlphaF(alpha);
    return tmp;
}

QColor mergedColors(const QColor &colorA, const QColor &colorB, int factor)
{
    const int maxFactor = 100;
    QColor tmp = colorA;
    tmp.setRed((tmp.red() * factor) / maxFactor + (colorB.red() *(maxFactor - factor)) / maxFactor);
    tmp.setGreen((tmp.green() * factor) / maxFactor + (colorB.green() *(maxFactor - factor)) / maxFactor);
    tmp.setBlue((tmp.blue() * factor) / maxFactor + (colorB.blue() *(maxFactor - factor)) / maxFactor);
    return tmp;
}

QColor tintColor(const QColor &base, qreal tint)
{
    QColor tmp = base;

#define lighten(x) (1.0 - (1.0 - x) * tint)
#define darken(x) (x * (2.0 - tint))

    if (tint < 1.0) {
        tmp.setRedF(lighten(tmp.redF()));
        tmp.setGreenF(lighten(tmp.greenF()));
        tmp.setBlueF(lighten(tmp.blueF()));
    } else {
        tmp.setRedF(darken(tmp.redF()));
        tmp.setGreenF(darken(tmp.greenF()));
        tmp.setBlueF(darken(tmp.blueF()));
    }

#undef lighten
#undef darken

    return tmp;
}

QColor alphaColor(const QColor &color, qreal alpha)
{
    QColor tmp = color;
    tmp.setAlphaF(alpha);
    return tmp;
}
