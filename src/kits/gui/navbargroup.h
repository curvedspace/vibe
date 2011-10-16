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

#ifndef NAVBARGROUP_H
#define NAVBARGROUP_H

#include <QtGui/QWidget>

#include <Qube/QubeCore/QubeGlobal>
#include <Qube/QubeGui/QNavBarItem>

namespace QubeGui
{
    class NavBarGroupPrivate;

    class QUBESHARED_EXPORT NavBarGroup : public QWidget
    {
        Q_OBJECT
        Q_PROPERTY(QString title READ title WRITE setTitle)
        Q_PROPERTY(bool isStatic READ isStatic WRITE setStatic)
        Q_PROPERTY(bool isExpanded READ isExpanded WRITE expand)
        Q_DECLARE_PRIVATE(NavBarGroup)
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

#endif // NAVBARGROUP_H
