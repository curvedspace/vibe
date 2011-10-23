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

#ifndef QUBE_NAVBARITEM_H
#define QUBE_NAVBARITEM_H

#include <QtGui/QWidget>

#include <Qube/Gui/SelectableWidget>

namespace Qube
{
    namespace Gui
    {
        class NavBarItemPrivate;

        class NavBarItem : public SelectableWidget
        {
            Q_OBJECT
            Q_DECLARE_PRIVATE(NavBarItem)
            Q_PROPERTY(QString text READ text WRITE setText)
            Q_PROPERTY(QPixmap icon READ icon WRITE setIcon)
        public:
            explicit NavBarItem(QWidget *parent = 0);
            NavBarItem(const QString &text, QWidget *parent = 0);
            NavBarItem(const QPixmap &icon, const QString &text, QWidget *parent = 0);

            void addSpacing(int size);
            void addWidget(QWidget *widget, int stretch = 0);

            void insertSpacing(int index, int size);
            void insertWidget(int index, QWidget *widget, int stretch = 0);

            QString text() const;
            void setText(const QString &text);
            void setTextColor(const QColor &color);

            const QPixmap *icon() const;
            void setIcon(const QPixmap &icon);

        private:
            NavBarItemPrivate *d_ptr;
        };
    }
}

#endif // QUBE_NAVBARITEM_H
