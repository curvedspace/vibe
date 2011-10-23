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

#ifndef QUBE_NAVBARGROUP_H
#define QUBE_NAVBARGROUP_H

#include <QtGui/QWidget>

#include <Qube/Gui/NavBarItem>

namespace Qube
{
    namespace Gui
    {
        class NavBarGroupPrivate;

        class NavBarGroup : public QWidget
        {
            Q_OBJECT
            Q_DECLARE_PRIVATE(NavBarGroup)
            Q_PROPERTY(QString title READ title WRITE setTitle)
            Q_PROPERTY(bool isStatic READ isStatic WRITE setStatic)
            Q_PROPERTY(bool isExpanded READ isExpanded WRITE expand)
        public:
            explicit NavBarGroup(QWidget *parent = 0);
            NavBarGroup(const QString &title, QWidget *parent = 0);

            void addItem(NavBarItem *item);
            void addItem(NavBarItem *item, int index);

            NavBarItem *addItem(const QString &text);
            NavBarItem *addItem(const QPixmap &icon, const QString &text);
            NavBarItem *addItem(const QPixmap &icon, const QString &text, int index);

            bool containsItem(NavBarItem *item);

            QString title() const;
            void setTitle(const QString &title);

            bool isExpanded() const;

            bool isStatic() const;
            void setStatic(bool flag);

            void setTitleColor(const QColor &color);

        signals:
            void selected(NavBarGroup *group, NavBarItem *item);
            void expanded(NavBarGroup *group);

        public slots:
            void expand(bool expand);

        private slots:
            void itemSelected(SelectableWidget *item);
            void titleClicked();

        private:
            NavBarGroupPrivate *d_ptr;
        };
    }
}

#endif // QUBE_NAVBARGROUP_H
