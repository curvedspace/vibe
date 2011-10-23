/****************************************************************************
 * This file is part of Qube.
 *
 * Copyright (c) 2008 Matteo Bertozzi
 * Copyright (c) 2010-2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Matteo Bertozzi <theo.bertozzi@gmail.com>
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Qube is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Qube is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Qube.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QtGui/QMouseEvent>

#include "selectablewidget.h"

namespace Qube
{
    namespace Gui
    {
        class SelectableWidgetPrivate
        {
        public:
            bool isSelected;
        };

        SelectableWidget::SelectableWidget(QWidget *parent)
            : QWidget(parent),
              d_ptr(new SelectableWidgetPrivate())
        {
            Q_D(SelectableWidget);
            d->isSelected = false;
        }

        bool SelectableWidget::isSelected() const
        {
            Q_D(const SelectableWidget);
            return d->isSelected;
        }

        void SelectableWidget::select()
        {
            select(true);
        }

        void SelectableWidget::select(bool isSelected)
        {
            Q_D(SelectableWidget);

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

        void SelectableWidget::unselect()
        {
            select(false);
        }

        void SelectableWidget::mouseReleaseEvent(QMouseEvent *event)
        {
            Q_D(SelectableWidget);

            QWidget::mouseReleaseEvent(event);

            // Left Click, Select/Unselect Item
            if (event->button() == Qt::LeftButton)
                select(!d->isSelected);
        }
    }
}
