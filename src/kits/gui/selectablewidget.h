/****************************************************************************
 *
 * Copyright (c) 2010 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * All rights reserved.
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

#ifndef SELECTABLEWIDGET_H
#define SELECTABLEWIDGET_H

#include <QWidget>

#include <QubeCore/Global>

namespace QubeGui
{
    class SelectableWidgetPrivate;

    class QUBESHARED_EXPORT SelectableWidget : public QWidget
    {
        Q_OBJECT
        Q_PROPERTY(bool isSelected READ isSelected WRITE select)
        Q_DECLARE_PRIVATE(SelectableWidget)
    public:
        explicit SelectableWidget(QWidget *parent = 0);

        bool isSelected() const;

    signals:
        void selected();
        void selected(SelectableWidget *widget);

    public slots:
        void select();
        void select(bool isSelected);
        void unselect();

    protected:
        void mouseReleaseEvent(QMouseEvent *event);

    private:
        SelectableWidgetPrivate *d_ptr;
    };
}

#endif // SELECTABLEWIDGET_H
