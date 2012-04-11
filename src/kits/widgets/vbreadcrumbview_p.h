/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * Vibe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Vibe.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef VBREADCRUMBVIEW_P_H
#define VBREADCRUMBVIEW_P_H

#include <QStyledItemDelegate>

#include "vbreadcrumbview.h"

class QHBoxLayout;

class VBreadcrumbViewList;
class VBreadcrumbViewButton;

class VBreadcrumbViewPrivate : public QObject
{
    Q_OBJECT
public:
    VBreadcrumbViewPrivate(VBreadcrumbView *self);

    QList<QModelIndex> crumbs;
    QList<VBreadcrumbViewButton *> buttons;
    QHBoxLayout *buttonLayout;

    void addCrumb(const QModelIndex &index);

protected:
    Q_DECLARE_PUBLIC(VBreadcrumbView)

    VBreadcrumbView *const q_ptr;

public slots:
    void buttonPressed();
};

class VBreadcrumbViewDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    VBreadcrumbViewDelegate(QObject *parent);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // VBREADCRUMBVIEW_P_H
