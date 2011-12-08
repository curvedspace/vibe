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

#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtGui/QVBoxLayout>

#include "vnavbar.h"
#include "vnavbar_p.h"

VNavBar::VNavBar(QWidget *parent)
    : QWidget(parent),
      d_ptr(new VNavBarPrivate())
{
    Q_D(VNavBar);

    // Initialize members
    d->layout = new QVBoxLayout();
    d->itemSelected = NULL;
    d->groupSelected = NULL;

    // Setup Layout
    d->layout->addStretch(2);
    setLayout(d->layout);
}

void VNavBar::addGroup(VNavBarGroup *group)
{
    Q_D(VNavBar);

    // Set font
    group->setTitleColor(QColor(0x65, 0x71, 0x80));

    // Add events
    connect(group, SIGNAL(selected(VNavBarGroup *, VNavBarItem *)),
            this, SLOT(itemSelected(VNavBarGroup *, VNavBarItem *)));

    // Add to layout
    d->layout->insertWidget(d->layout->count() - 1, group);
}

VNavBarGroup *VNavBar::addGroup(const QString &title)
{
    VNavBarGroup *group = new VNavBarGroup(title);
    addGroup(group);
    return group;
}

void VNavBar::addGroups(QList<VNavBarGroup *>groups)
{
    for (int i = 0; i < groups.count(); ++i)
        addGroup(groups.at(i));
}

void VNavBar::removeGroup(VNavBarGroup *group)
{
    Q_D(VNavBar);
    d->layout->removeWidget(group);
}

void VNavBar::clear()
{
    Q_D(VNavBar);

    for (int i = 0; i < d->layout->count(); ++i) {
        QWidget *widget = d->layout->itemAt(i)->widget();
        d->layout->removeWidget(widget);
    }
}

QList<VNavBarGroup *> VNavBar::groups() const
{
    Q_D(const VNavBar);

    QList<VNavBarGroup *> list;
    for (int i = 0; i < d->layout->count(); ++i)
        list.append(qobject_cast<VNavBarGroup *>(d->layout->itemAt(i)->widget()));
}

bool VNavBar::containsGroup(VNavBarGroup *group)
{
    Q_D(VNavBar);

    for (int i = 0; i < d->layout->count(); ++i) {
        QWidget *widget = d->layout->itemAt(i)->widget();
        if (widget == group)
            return true;
    }
    return false;
}

void VNavBar::paintEvent(QPaintEvent *event)
{
    Q_D(VNavBar);

    QWidget::paintEvent(event);

    // Recall Update() if painter area is not complete!
    if (event->rect().x() > 0 || event->rect().y() > 0)
        update();

    //QColor colorBackground = palette().color(QPalette::Midlight);
    QColor colorBackground(0xdf, 0xe4, 0xea);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(event->rect(), colorBackground);

    // Draw Selection
    if (d->groupSelected != NULL && d->groupSelected->isExpanded() && d->itemSelected != NULL) {
        QPoint pos = d->groupSelected->pos() + d->itemSelected->pos();
        int width = geometry().width();

        QColor colorSelection = palette().color(QPalette::Highlight);
        int r = colorSelection.red();
        int g = colorSelection.green();
        int b = colorSelection.blue();
        p.fillRect(0, pos.y() - 1, width, 1, QColor(r - 0x26, g - 0x26, b - 0x26));

        QLinearGradient linearGrad(QPointF(0, pos.y()), QPointF(0, pos.y() + d->itemSelected->height()));
        linearGrad.setColorAt(0, colorSelection);
        linearGrad.setColorAt(1, QColor(r - 0x3b, g - 0x3b, b - 0x3b));
        p.fillRect(0, pos.y(), width, d->itemSelected->height(), linearGrad);
    }

    p.end();
}

void VNavBar::itemSelected(VNavBarGroup *group, VNavBarItem *item)
{
    Q_D(VNavBar);

    if (d->itemSelected != NULL && d->itemSelected != item) {
        d->itemSelected->setFont(item->font());
        d->itemSelected->setTextColor(palette().color(QPalette::WindowText));
        d->itemSelected->unselect();
    }

    d->groupSelected = group;
    d->itemSelected = item;

    QFont font = d->itemSelected->font();
    font.setPointSizeF(font.pointSizeF() * 0.65f);
    font.setBold(true);
    d->itemSelected->setFont(font);
    d->itemSelected->setTextColor(palette().color(QPalette::HighlightedText));

    update();
}

void VNavBar::groupExpanded(VNavBarGroup *)
{
    update();
}

#include "vnavbar.moc"
