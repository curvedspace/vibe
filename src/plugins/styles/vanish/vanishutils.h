/****************************************************************************
 * This file is part of Vanish.
 *
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
 * Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies)
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vanish is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1
 * as published by the Free Software Foundation.
 *
 * Vanish is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Vanish.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef VANISHUTILS_H
#define VANISHUTILS_H

#include <QtCore/qglobal.h>
#include <QLinearGradient>
#include <QBrush>

qreal dpiScaled(qreal value);

QLinearGradient qMapGradientToRect(const QLinearGradient &gradient, const QRectF &rect);
QBrush qMapBrushToRect(const QBrush &brush, const QRectF &rect);
void qBrushSetAlphaF(QBrush *brush, qreal alpha);
QBrush qBrushLight(QBrush brush, int light);
QBrush qBrushDark(QBrush brush, int dark);

#endif // VANISHUTILS_H
