/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2008 Matteo Bertozzi
 * Copyright (c) 2010-2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Matteo Bertozzi <theo.bertozzi@gmail.com>
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

#include <QtGui/QMouseEvent>

#include "vselectablewidget.h"
#include "vselectablewidget_p.h"

VSelectableWidget::VSelectableWidget(QWidget *parent)
    : QWidget(parent),
      d_ptr(new VSelectableWidgetPrivate())
{
    Q_D(VSelectableWidget);
    d->isSelected = false;
}

bool VSelectableWidget::isSelected() const
{
    Q_D(const VSelectableWidget);
    return d->isSelected;
}

void VSelectableWidget::select()
{
    select(true);
}

void VSelectableWidget::select(bool isSelected)
{
    Q_D(VSelectableWidget);

    if (d->isSelected == isSelected)
        return;

    d->isSelected = isSelected;

    // If is selected raise event
    if (isSelected) {
        emit selected();
        emit selected(this);
    }

    update();
}

void VSelectableWidget::unselect()
{
    select(false);
}

void VSelectableWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_D(VSelectableWidget);

    QWidget::mouseReleaseEvent(event);

    // Left Click, Select/Unselect Item
    if (event->button() == Qt::LeftButton)
        select(!d->isSelected);
}

#include "vselectablewidget.moc"
