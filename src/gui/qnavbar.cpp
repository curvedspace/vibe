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

#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtGui/QVBoxLayout>

#include "qnavbar.h"

class QNavBar::Private
{
public:
    QVBoxLayout *layout;

    // Current selection
    QNavBarGroup *groupSelected;
    QNavBarItem *itemSelected;
};

QNavBar::QNavBar(QWidget *parent)
    : QWidget(parent),
      d(new QNavBar::Private)
{
    // Initialize members
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
}

void QNavBar::addGroup(QNavBarGroup *group)
{
    // Set font
    group->setTitleColor(QColor(0x65, 0x71, 0x80));
    // Add events
    connect(group, SIGNAL(selected(QNavBarGroup *, QNavBarItem *)),
            this, SLOT(itemSelected(QNavBarGroup *, QNavBarItem *)));
    // Add to layout
    d->layout->insertWidget(d->layout->count() - 1, group);
}

QNavBarGroup *QNavBar::addGroup(const QString &title)
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

void QNavBar::itemSelected(QNavBarGroup *group, QNavBarItem *item)
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

void QNavBar::groupExpanded(QNavBarGroup *)
{
    update();
}

#include "qnavbar.moc"
