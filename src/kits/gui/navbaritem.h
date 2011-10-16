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

#ifndef NAVBARITEM_H
#define NAVBARITEM_H

#include <QtGui/QWidget>

#include <Qube/QubeCore/QubeGlobal>
#include <Qube/QubeGui/QSelectableWidget>

namespace QubeGui
{
    class NavBarItemPrivate;

    class QUBESHARED_EXPORT NavBarItem : public SelectableWidget
    {
        Q_OBJECT
        Q_PROPERTY(QString text READ text WRITE setText)
        Q_PROPERTY(QPixmap icon READ icon WRITE setIcon)
        Q_DECLARE_PRIVATE(NavBarItem)
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

#endif // NAVBARITEM_H
