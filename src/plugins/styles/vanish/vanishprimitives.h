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

#ifndef VANISHPRIMITIVES_H
#define VANISHPRIMITIVES_H

#include <QStyleOption>
#include <QPainter>
#include <QWidget>

enum ArrowOrientation {
    ArrowNone,
    ArrowUp,
    ArrowDown,
    ArrowLeft,
    ArrowRight
};

enum ArrowSize {
    ArrowNormal,
    ArrowSmall,
    ArrowTiny
};

/*
 * genericArrow() is based on Oxygen code.
 *
 * Copyright (c) 2009-2010 Hugo Pereira Da Costa <hugo@oxygen-icons.org>
 * Copyright (c) 2008 Long Huynh Huu <long.upcase@googlemail.com>
 * Copyright (c) 2007-2008 Casper Boemann <cbr@boemann.dk>
 * Copyright (c) 2007 Matthew Woehlke <mw_triad@users.sourceforge.net>
 * Copyright (c) 2003-2005 Sandro Giessl <sandro@giessl.com>
 */
QPolygonF genericArrow(ArrowOrientation orientation,
                       ArrowSize size);

void drawArrow(ArrowOrientation orient, const QStyleOption *option,
               QPainter *painter, const QWidget *widget);
void drawLineEditFrame(QPainter *painter, const QStyleOption *option);

void drawAaPoint(QPainter *p, int x, int y);
void drawDots(QPainter *p, const QRect &r, bool horiz, int nLines, int offset,
              const QColor *cols, int startOffset, int dark);

#endif // VANISHPRIMITIVES_H
