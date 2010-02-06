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

#include <QPaintEvent>
#include <QVBoxLayout>
#include <QPainter>

#include "qnavbar.h"

class QNavBar::Private
{
public:
    QVBoxLayout *layout;

    // Colors
    QColor colorBackground;
    QColor colorSelection;

    // Current Selection
    QNavBarGroup *groupSelected;
    QNavBarItem *itemSelected;
};

QNavBar::QNavBar(QWidget *parent)
    : QWidget(parent),
    d(new QNavBar::Private)
{
    // Initialize Members
    d->layout = new QVBoxLayout();
    d->itemSelected = NULL;
    d->groupSelected = NULL;

    // Setup Colors
    d->colorBackground = QColor(0xdf, 0xe4, 0xea);
    d->colorSelection = QColor(0xa6, 0xa6, 0xa6);

    // Setup Layout
    d->layout->addStretch(2);
    setLayout(d->layout);
}

QNavBar::~QNavBar()
{
    delete d;
    d = NULL;
}

void QNavBar::addGroup(QNavBarGroup *group)
{
    // Set Font
    group->setTitleColor(QColor(0x65, 0x71, 0x80));

    // Add Events
    connect(group, SIGNAL(selected(QNavBarGroup *, QNavBarItem *)),
            this, SLOT(onItemSelected(QNavBarGroup *, QNavBarItem *)));

    // Add to Layout
    d->layout->insertWidget(d->layout->count() - 1, group);
}

QNavBarGroup *QNavBar::addGroup(const QString& title)
{
    QNavBarGroup *group = new QNavBarGroup(title);
    addGroup(group);
    return group;
}

bool QNavBar::containsGroup(QNavBarGroup *group)
{
    int itemCount = d->layout->count();

    for (int i = 0; i < itemCount; ++i) {
        QWidget *widget = d->layout->itemAt(i)->widget();
        if (widget == group)
            return true;
    }
	
    return false;
}

QColor QNavBar::backgroundColor() const
{
    return d->colorBackground;
}

QColor QNavBar::selectionColor() const
{
    return d->colorSelection;
}

void QNavBar::setBackgroundColor(const QColor& bgColor)
{
    d->colorBackground = bgColor;
}

void QNavBar::setSelectionColor(const QColor& selectionColor)
{
    d->colorSelection = selectionColor;
}

void QNavBar::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    // Recall Update() if painter area is not complete!
    if (event->rect().x() > 0 || event->rect().y() > 0)
        update();

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(event->rect(), d->colorBackground);

    // Draw Selection
    if (d->groupSelected != NULL && d->groupSelected->isExpanded() && d->itemSelected != NULL) {
        QPoint pos = d->groupSelected->pos() + d->itemSelected->pos();
        int width = geometry().width();

        int r = d->colorSelection.red();
        int g = d->colorSelection.green();
        int b = d->colorSelection.blue();

        p.fillRect(0, pos.y() - 1, width, 1, QColor(r - 0x26, g - 0x26, b - 0x26));

        QLinearGradient linearGrad(QPointF(0, pos.y()), QPointF(0, pos.y() + d->itemSelected->height()));
        linearGrad.setColorAt(0, d->colorSelection);
        linearGrad.setColorAt(1, QColor(r - 0x3b, g - 0x3b, b - 0x3b));
        p.fillRect(0, pos.y(), width, d->itemSelected->height(), linearGrad);
    }

    p.end();
}

void QNavBar::onItemSelected(QNavBarGroup *group, QNavBarItem *item)
{
    if (d->itemSelected != NULL && d->itemSelected != item) {
        d->itemSelected->setFont(item->font());
        d->itemSelected->setTextColor(Qt::black);
        d->itemSelected->unselect();
    }

    d->groupSelected = group;
    d->itemSelected = item;

    QFont font = d->itemSelected->font();
    font.setPixelSize(9);
    font.setBold(true);
    d->itemSelected->setFont(font);
    d->itemSelected->setTextColor(Qt::white);

    update();
}

void QNavBar::onGroupExpanded(QNavBarGroup *group)
{
    Q_UNUSED(group)
    update();
}
