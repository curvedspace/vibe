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

#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>

#include "qnavbaritem.h"

class QNavBarItem::Private
{
public:
    QHBoxLayout *layout;
    QLabel *labelIcon;
    QLabel *labelText;

    void initialize(QNavBarItem *item);
};

void QNavBarItem::Private::initialize(QNavBarItem *item)
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

QNavBarItem::QNavBarItem(QWidget *parent)
    : QSelectableWidget(parent),
      d(new QNavBarItem::Private())
{
    d->initialize(this);
}

QNavBarItem::QNavBarItem(const QString &text, QWidget *parent)
    : QSelectableWidget(parent),
      d(new QNavBarItem::Private())
{
    d->initialize(this);
    d->labelText->setText(text);
}

QNavBarItem::QNavBarItem(const QPixmap &icon, const QString &text, QWidget *parent)
    : QSelectableWidget(parent),
      d(new QNavBarItem::Private())
{
    d->initialize(this);
    setIcon(icon);
    d->labelText->setText(text);
}

QNavBarItem::~QNavBarItem()
{
    delete d;
}

void QNavBarItem::addSpacing(int size)
{
    d->layout->addSpacing(size);
}

void QNavBarItem::addWidget(QWidget *widget, int stretch)
{
    d->layout->addWidget(widget, stretch);
}

void QNavBarItem::insertSpacing(int index, int size)
{
    d->layout->insertSpacing(index, size);
}

void QNavBarItem::insertWidget(int index, QWidget *widget, int stretch)
{
    d->layout->insertWidget(index, widget, stretch);
}

QString QNavBarItem::text() const
{
    return d->labelText->text();
}

const QPixmap *QNavBarItem::icon() const
{
    return d->labelIcon->pixmap();
}

void QNavBarItem::setIcon(const QPixmap &icon)
{
    if (icon.height() > 20)
        d->labelIcon->setPixmap(icon.scaledToHeight(20, Qt::SmoothTransformation));
    else
        d->labelIcon->setPixmap(icon);
}

void QNavBarItem::setText(const QString &text)
{
    d->labelText->setText(text);
}

void QNavBarItem::setTextColor(const QColor &color)
{
    QPalette palette = d->labelText->palette();
    palette.setColor(QPalette::WindowText, color);
    d->labelText->setPalette(palette);
}

#include "qnavbaritem.moc"
