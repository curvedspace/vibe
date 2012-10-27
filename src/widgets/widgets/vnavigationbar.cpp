/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPL2$
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include <QScrollBar>
#include <QStandardItemModel>
#include <QItemSelectionModel>

#include "vnavigationbar.h"
#include "navigationitemdelegate.h"

VNavigationBar::VNavigationBar(QWidget *parent) :
    QTreeView(parent)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    setMinimumWidth(145);
    setMaximumWidth(245);
    setFrameShape(QFrame::StyledPanel);
    setFrameShadow(QFrame::Plain);
    setContentsMargins(0, 0, 0, 0);
    setHeaderHidden(true);
    setWordWrap(true);
    setAnimated(true);
    setItemsExpandable(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setRootIsDecorated(false);
    setUniformRowHeights(false);
    setAlternatingRowColors(false);
    setAutoFillBackground(false);
    verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    setItemDelegate(new VPrivate::NavigationItemDelegate(this));
}

void VNavigationBar::drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const
{
    QRect primitive(rect.right() + 4, rect.top(), 5, rect.height());

    QStyleOption option;
    option.initFrom(this);
    option.rect = primitive;

    QStyle::State extraFlags = QStyle::State_None;

    const bool children = static_cast<QStandardItemModel *>(model())->hasChildren(index);

    if (children) {
        if (!isExpanded(index))
            style()->drawPrimitive(QStyle::PE_IndicatorArrowRight, &option, painter, this);
        else
            style()->drawPrimitive(QStyle::PE_IndicatorArrowDown, &option, painter, this);
    }
}

#include "moc_vnavigationbar.cpp"
