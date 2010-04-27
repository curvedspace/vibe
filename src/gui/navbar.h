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

#ifndef NAVBAR_H
#define NAVBAR_H

#include <QtGui/QWidget>

#include "qube_global.h"
#include "navbargroup.h"

namespace QubeGui
{
    class NavBarPrivate;

    class QUBESHARED_EXPORT NavBar : public QWidget
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

#endif // NAVBAR_H
