/****************************************************************************
 *
 * Copyright (c) 2010 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 * Copyright (c) 2008 Matteo Bertozzi <theo.bertozzi@gmail.com>
 * All rights reserved.
 *
 * Contact: Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * GNU Lesser General Public License Usage
 * This file may be used under the terms of the GNU Lesser
 * General Public License version 2.1 as published by the Free Software
 * Foundation and appearing in the file LICENSE.LGPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU Lesser General Public License version 2.1 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 ***************************************************************************/

#include <QtGui/QMouseEvent>

#include "selectablewidget.h"

namespace QubeGui
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

#include "selectablewidget.moc"
