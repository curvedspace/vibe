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

void QNavBar::addGroups(QList<QNavBarGroup *>groups)
{
    for (int i = 0; i < groups.count(); ++i)
        addGroup(groups.at(i));
}

void QNavBar::removeGroup(QNavBarGroup *group)
{
    d->layout->removeWidget(group);
}

void QNavBar::clear()
{
    for (int i = 0; i < d->layout->count(); ++i) {
        QWidget *widget = d->layout->itemAt(i)->widget();
        d->layout->removeWidget(widget);
    }
}

QList<QNavBarGroup *> QNavBar::groups() const
{
    QList<QNavBarGroup *> list;

    for (int i = 0; i < d->layout->count(); ++i)
        list.append(qobject_cast<QNavBarGroup *>(d->layout->itemAt(i)->widget()));
}

bool QNavBar::containsGroup(QNavBarGroup *group)
{
    for (int i = 0; i < d->layout->count(); ++i) {
        QWidget *widget = d->layout->itemAt(i)->widget();
        if (widget == group)
            return true;
    }
	
    return false;
}

void QNavBar::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    // Recall Update() if painter area is not complete!
    if (event->rect().x() > 0 || event->rect().y() > 0)
        update();

    //QColor colorBackground = palette().color(QPalette::Midlight);
    QColor colorBackground(0xdf, 0xe4, 0xea);
    QColor colorSelection = palette().color(QPalette::Highlight);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(event->rect(), colorBackground);

    // Draw Selection
    if (d->groupSelected != NULL && d->groupSelected->isExpanded() && d->itemSelected != NULL) {
        QPoint pos = d->groupSelected->pos() + d->itemSelected->pos();
        int width = geometry().width();

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

void QNavBar::onItemSelected(QNavBarGroup *group, QNavBarItem *item)
{
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

void QNavBar::onGroupExpanded(QNavBarGroup *group)
{
    Q_UNUSED(group)
    update();
}

#include "qnavbar.moc"
