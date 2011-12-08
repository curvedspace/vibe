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

#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>

#include "vnavbaritem.h"
#include "vnavbaritem_p.h"

/*
 * VNavBarItemPrivate
 */

void VNavBarItemPrivate::initialize(VNavBarItem *item)
{
    // Initialize members
    layout = new QHBoxLayout();
    labelIcon = new QLabel();
    labelText = new QLabel();

    // Setup text label
    labelText->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelIcon->setFixedWidth(18);

    // Setup font
    QFont font = labelText->font();
    font.setPointSizeF(font.pointSizeF() * 0.75f);
    labelText->setFont(font);

    // Add items to layout
    layout->addWidget(labelIcon);
    layout->addWidget(labelText);

    // Setup layout
    layout->setContentsMargins(1, 1, 1, 1);
    item->setLayout(layout);
}

/*
 * VNavBarItem
 */

VNavBarItem::VNavBarItem(QWidget *parent)
    : VSelectableWidget(parent),
      d_ptr(new VNavBarItemPrivate())
{
    Q_D(VNavBarItem);

    d->initialize(this);
}

VNavBarItem::VNavBarItem(const QString &text, QWidget *parent)
    : VSelectableWidget(parent),
      d_ptr(new VNavBarItemPrivate())
{
    Q_D(VNavBarItem);

    d->initialize(this);
    d->labelText->setText(text);
}

VNavBarItem::VNavBarItem(const QPixmap &icon, const QString &text, QWidget *parent)
    : VSelectableWidget(parent),
      d_ptr(new VNavBarItemPrivate())
{
    Q_D(VNavBarItem);

    d->initialize(this);
    setIcon(icon);
    d->labelText->setText(text);
}

void VNavBarItem::addSpacing(int size)
{
    Q_D(VNavBarItem);
    d->layout->addSpacing(size);
}

void VNavBarItem::addWidget(QWidget *widget, int stretch)
{
    Q_D(VNavBarItem);
    d->layout->addWidget(widget, stretch);
}

void VNavBarItem::insertSpacing(int index, int size)
{
    Q_D(VNavBarItem);
    d->layout->insertSpacing(index, size);
}

void VNavBarItem::insertWidget(int index, QWidget *widget, int stretch)
{
    Q_D(VNavBarItem);
    d->layout->insertWidget(index, widget, stretch);
}

QString VNavBarItem::text() const
{
    Q_D(const VNavBarItem);
    return d->labelText->text();
}

const QPixmap *VNavBarItem::icon() const
{
    Q_D(const VNavBarItem);
    return d->labelIcon->pixmap();
}

void VNavBarItem::setIcon(const QPixmap &icon)
{
    Q_D(VNavBarItem);

    if (icon.height() > 20)
        d->labelIcon->setPixmap(icon.scaledToHeight(20, Qt::SmoothTransformation));
    else
        d->labelIcon->setPixmap(icon);
}

void VNavBarItem::setText(const QString &text)
{
    Q_D(VNavBarItem);
    d->labelText->setText(text);
}

void VNavBarItem::setTextColor(const QColor &color)
{
    Q_D(VNavBarItem);

    QPalette palette = d->labelText->palette();
    palette.setColor(QPalette::WindowText, color);
    d->labelText->setPalette(palette);
}

#include "vnavbaritem.moc"
