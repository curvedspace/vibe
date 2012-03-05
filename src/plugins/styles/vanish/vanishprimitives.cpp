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

#include <QToolButton>

#include "vanishprimitives.h"
#include "vanishcolorutils.h"
#include "vanishutils.h"

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
                       ArrowSize size)
{
    QPolygonF a;

    switch (orientation) {
        case ArrowUp:
            if (size == ArrowTiny)
                a << QPointF(-1.75, 1.125) << QPointF(0.5, -1.125) << QPointF(2.75, 1.125) << QPointF(-1.75, 1.125);
            else if (size == ArrowSmall)
                a << QPointF(-2, 1.5) << QPointF(0.5, -1.5) << QPointF(3, 1.5) << QPointF(-2, 1.5);
            else
                a << QPointF(-3, 2.5) << QPointF(0.5, -1.5) << QPointF(4, 2.5) << QPointF(-3, 2.5);
            break;
        case ArrowDown:
            if (size == ArrowTiny)
                a << QPointF(-1.75, -1.125) << QPointF(0.5, 1.125) << QPointF(2.75, -1.125) << QPointF(-1.75, -1.125);
            else if (size == ArrowSmall)
                a << QPointF(-2, -1.5) << QPointF(0.5, 1.5) << QPointF(3, -1.5) << QPointF(-2, -1.5);
            else
                a << QPointF(-3, -1.5) << QPointF(0.5, 2.5) << QPointF(4, -1.5) << QPointF(-3, -1.5);
            break;
        case ArrowLeft:
            if (size == ArrowTiny)
                a << QPointF(1.125, -1.75) << QPointF(-1.125, 0.5) << QPointF(1.125, 2.75) << QPointF(1.125, -1.75);
            else if (size == ArrowSmall)
                a << QPointF(1.5, -2) << QPointF(-1.5, 0.5) << QPointF(1.5, 3) << QPointF(1.5, -2);
            else
                a << QPointF(2.5, -3) << QPointF(-1.5, 0.5) << QPointF(2.5, 4) << QPointF(2.5, -3);
            break;
        case ArrowRight:
            if (size == ArrowTiny)
                a << QPointF(-1.125, -1.75) << QPointF(1.125, 0.5) << QPointF(-1.125, 2.75) << QPointF(-1.125, -1.75);
            else if (size == ArrowSmall)
                a << QPointF(-1.5, -2) << QPointF(1.5, 0.5) << QPointF(-1.5, 3) << QPointF(-1.5, -2);
            else
                a << QPointF(-1.5, -3) << QPointF(2.5, 0.5) << QPointF(-1.5, 4) << QPointF(-1.5, -3);
            break;
        default:
            break;
    }

    return a;
}

void drawArrow(ArrowOrientation orient, const QStyleOption *option,
               QPainter *painter, const QWidget *widget)
{
    QRect rect(option->rect);
    const QPolygonF a = genericArrow(orient, ArrowNormal);
    QColor color;

    const QToolButton *toolButton = qobject_cast<const QToolButton *>(widget);
    if (toolButton && toolButton->arrowType() != Qt::NoArrow) {
        // Arrow painted on a tool button need a re-centered rectangle
        rect.translate(1, 0);
        color = option->palette.color(toolButton->autoRaise()
                                      ? QPalette::WindowText : QPalette::ButtonText);
    } else
        color = option->palette.windowText().color();

    // Draw with antialiasing
    painter->setRenderHint(QPainter::Antialiasing);

    // Center the arrow
    painter->translate(rect.center());

    // Fill arrow polygon
    painter->setPen(QPen(color, 1.6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->setBrush(color);
    painter->drawPolygon(a);
}

void drawLineEditFrame(QPainter *painter, const QStyleOption *option)
{
    if (const QStyleOptionFrame *opt = qstyleoption_cast<const QStyleOptionFrame *>(option)) {
        painter->save();

        QRect rect(opt->rect);

        if (opt->state & QStyle::State_HasFocus) {
            // Draw highlighted border 2 pixels wide
            QBrush focusBorder = option->palette.highlight();
            qBrushSetAlphaF(&focusBorder, qreal(0.45));

            painter->setPen(QPen(focusBorder, 4.0));
            painter->drawRect(rect);

            rect.adjust(2, 2, -3, -3);

            // Draw border
            painter->setPen(QPen(option->palette.highlight().color(), 0));
            painter->drawRect(rect);
        } else {
            rect.adjust(2, 2, -2, -2);

            // Left and top borders are darker
            painter->setPen(shadeColor(opt->palette.window().color(), 0.68));
            const QLine leftTopLines[2] = {
                QLine(rect.topLeft(), QPoint(rect.left(), rect.bottom() - 1)),
                QLine(rect.topLeft(), rect.topRight())
            };
            painter->drawLines(leftTopLines, 2);

            // Right and bottom borders are lighter
            painter->setPen(shadeColor(opt->palette.window().color(), 0.89));
            const QLine rightBottomLines[2] = {
                QLine(QPoint(rect.right(), rect.top() + 1), rect.bottomRight()),
                QLine(rect.bottomRight(), rect.bottomLeft())
            };
            painter->drawLines(rightBottomLines, 2);
        }

        painter->restore();
    }
}

void drawAaPoint(QPainter *p, int x, int y)
{
    p->drawPoint(QPointF(x + 0.5, y + 0.5));
}

void drawDots(QPainter *p, const QRect &r, bool horiz, int nLines, int offset,
              const QColor *cols, int startOffset, int dark)
{
    int space((nLines * 2) + (nLines - 1)),
        x(horiz ? r.x() : r.x() + ((r.width() - space) >> 1)),
        y(horiz ? r.y() + ((r.height() - space) >> 1) : r.y()),
        i, j,
        numDots((horiz ? (r.width() - (2 * offset)) / 3 : (r.height() - (2 * offset)) / 3) + 1);

    p->setRenderHint(QPainter::Antialiasing);
    if (horiz) {
        if (startOffset && y + startOffset > 0)
            y += startOffset;

        p->setPen(cols[dark]);
        for (i = 0; i < space; i += 3)
            for (j = 0; j < numDots; j++)
                drawAaPoint(p, x + offset + (3 * j), y + i);

        p->setPen(cols[0]);
        for (i = 1; i < space; i += 3)
            for (j = 0; j < numDots; j++)
                drawAaPoint(p, x + offset + 1 + (3 * j), y + i);
    } else {
        if (startOffset && x + startOffset > 0)
            x += startOffset;

        p->setPen(cols[dark]);
        for (i = 0; i < space; i += 3)
            for (j = 0; j < numDots; j++)
                drawAaPoint(p, x + i, y + offset + (3 * j));

        p->setPen(cols[0]);
        for (i = 1; i < space; i += 3)
            for (j = 0; j < numDots; j++)
                drawAaPoint(p, x + i, y + offset + 1 + (3 * j));
    }
    p->setRenderHint(QPainter::Antialiasing, false);
}
