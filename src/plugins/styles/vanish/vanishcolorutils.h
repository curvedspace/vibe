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

#ifndef VANISHCOLORUTILS_H
#define VANISHCOLORUTILS_H

#include <QColor>

const qreal kLighten1Tint = 0.590;
const qreal kLighten2Tint = 0.385;
const qreal kLightenMaxTint = 0.0;

const qreal kNoTint = 1.0;

const qreal kDarken1Tint = 1.147;
const qreal kDarken2Tint = 1.295;
const qreal kDarken3Tint = 1.407;
const qreal kDarken4Tint = 1.555;
const qreal kDarkenMaxTint = 2.0;

const qreal kDisabledLabelTint = kDarken3Tint;
const qreal kDisabledMarkTint = kLighten2Tint;
const qreal kHighlightBackgroundTint = kDarken2Tint;

QColor shadeColor(const QColor &color, qreal factor, qreal alpha = 1.0);
QColor mergedColors(const QColor &colorA, const QColor &colorB, int factor = 50);
QColor tintColor(const QColor &base, qreal tint);
QColor alphaColor(const QColor &color, qreal alpha);

#endif // VANISHCOLORUTILS_H
