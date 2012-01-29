/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
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

#include <QGraphicsView>
#include <QToolTip>
#include <QApplication>
#include <QGraphicsSceneEvent>

#include "tooltiparea.h"

TooltipArea::TooltipArea(QQuickItem *parent) :
    QQuickItem(parent),
    m_containsMouse(false)
{
    setAcceptHoverEvents(true);
    connect(&m_tiptimer, SIGNAL(timeout()), this, SLOT(timeout()));
    m_tiptimer.setSingleShot(true);
}

void TooltipArea::setText(const QString &t)
{
    if (t != m_text) {
        m_text = t;
        emit textChanged();
    }
}

void TooltipArea::showToolTip(const QString &str) const
{
    QToolTip::showText(QPoint(pos().x(), pos().y()), str);
}

void TooltipArea::hoverEnterEvent(QHoverEvent *event)
{
    m_tiptimer.start(1000);

    m_containsMouse = true;
    emit containsMouseChanged();
    QQuickItem::hoverEnterEvent(event);
}

void TooltipArea::hoverLeaveEvent(QHoverEvent *event)
{
    m_tiptimer.stop();
    m_containsMouse = false;
    emit containsMouseChanged();
    QQuickItem::hoverLeaveEvent(event);
}

void TooltipArea::timeout()
{
    showToolTip(m_text);
}

#include "moc_tooltiparea.cpp"
