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

#include "navbaritem.h"

namespace QubeGui
{
    class NavBarItemPrivate
    {
    public:
        QHBoxLayout *layout;
        QLabel *labelIcon;
        QLabel *labelText;

        void initialize(NavBarItem *item);
    };

    void NavBarItemPrivate::initialize(NavBarItem *item)
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

    NavBarItem::NavBarItem(QWidget *parent)
        : SelectableWidget(parent),
          d_ptr(new NavBarItemPrivate())
    {
        Q_D(NavBarItem);

        d->initialize(this);
    }

    NavBarItem::NavBarItem(const QString &text, QWidget *parent)
        : SelectableWidget(parent),
          d_ptr(new NavBarItemPrivate())
    {
        Q_D(NavBarItem);

        d->initialize(this);
        d->labelText->setText(text);
    }

    NavBarItem::NavBarItem(const QPixmap &icon, const QString &text, QWidget *parent)
        : SelectableWidget(parent),
          d_ptr(new NavBarItemPrivate())
    {
        Q_D(NavBarItem);

        d->initialize(this);
        setIcon(icon);
        d->labelText->setText(text);
    }

    void NavBarItem::addSpacing(int size)
    {
        Q_D(NavBarItem);
        d->layout->addSpacing(size);
    }

    void NavBarItem::addWidget(QWidget *widget, int stretch)
    {
        Q_D(NavBarItem);
        d->layout->addWidget(widget, stretch);
    }

    void NavBarItem::insertSpacing(int index, int size)
    {
        Q_D(NavBarItem);
        d->layout->insertSpacing(index, size);
    }

    void NavBarItem::insertWidget(int index, QWidget *widget, int stretch)
    {
        Q_D(NavBarItem);
        d->layout->insertWidget(index, widget, stretch);
    }

    QString NavBarItem::text() const
    {
        Q_D(const NavBarItem);
        return d->labelText->text();
    }

    const QPixmap *NavBarItem::icon() const
    {
        Q_D(const NavBarItem);
        return d->labelIcon->pixmap();
    }

    void NavBarItem::setIcon(const QPixmap &icon)
    {
        Q_D(NavBarItem);

        if (icon.height() > 20)
            d->labelIcon->setPixmap(icon.scaledToHeight(20, Qt::SmoothTransformation));
        else
            d->labelIcon->setPixmap(icon);
    }

    void NavBarItem::setText(const QString &text)
    {
        Q_D(NavBarItem);
        d->labelText->setText(text);
    }

    void NavBarItem::setTextColor(const QColor &color)
    {
        Q_D(NavBarItem);

        QPalette palette = d->labelText->palette();
        palette.setColor(QPalette::WindowText, color);
        d->labelText->setPalette(palette);
    }
}
