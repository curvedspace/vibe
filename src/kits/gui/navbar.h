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

#ifndef QUBE_NAVBAR_H
#define QUBE_NAVBAR_H

#include <QtGui/QWidget>

#include <Qube/Gui/NavBarGroup>

namespace Qube
{
    namespace Gui
    {
        class NavBarPrivate;

        class NavBar : public QWidget
        {
            Q_OBJECT
            Q_DECLARE_PRIVATE(NavBar)
        public:
            explicit NavBar(QWidget *parent = 0);

            void addGroup(NavBarGroup *group);
            NavBarGroup *addGroup(const QString &title);
            void addGroups(QList<NavBarGroup *> groups);

            void removeGroup(NavBarGroup *group);

            void clear();

            QList<NavBarGroup *> groups() const;
            bool containsGroup(NavBarGroup *group);

        protected:
            void paintEvent(QPaintEvent *event);

        private slots:
            void itemSelected(NavBarGroup *group, NavBarItem *item);
            void groupExpanded(NavBarGroup *);

        private:
            NavBarPrivate *d_ptr;
        };
    }
}

#endif // QUBE_NAVBAR_H
