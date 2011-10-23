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

#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>

#include "navbaritem.h"

namespace Qube
{
    namespace Gui
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
}
