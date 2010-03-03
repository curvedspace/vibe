/* 
 *  This file is part of Quartica.
 *
 *  Copyright (c) 2008 Matteo Bertozzi <theo.bertozzi@gmail.com>
 *
 *  Quartica is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Quartica is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Quartica.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QMouseEvent>

#include "qselectablewidget.h"

class QSelectableWidget::Private
{
public:
    bool isSelected;
};

QSelectableWidget::QSelectableWidget(QWidget *parent)
    : QWidget(parent), d(new QSelectableWidget::Private)
{
    d->isSelected = false;
}

QSelectableWidget::~QSelectableWidget()
{
    delete d;
    d = NULL;
}

bool QSelectableWidget::isSelected() const
{
    return d->isSelected;
}

void QSelectableWidget::select()
{
    select(true);
}

void QSelectableWidget::select(bool isSelected)
{
    if (d->isSelected == isSelected)
        return;

    d->isSelected = isSelected;

    // if is selected raise event
    if (isSelected) {
        emit selected();
        emit selected(this);
    }

    update();
}

void QSelectableWidget::unselect()
{
    select(false);
}

void QSelectableWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);

    // Left Click, Select/Unselect Item
    if (event->button() == Qt::LeftButton)
        select(!d->isSelected);
}

#include "qselectablewidget.moc"
