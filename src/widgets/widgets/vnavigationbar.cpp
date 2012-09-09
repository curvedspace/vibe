/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPL-ONLY$
 *
 * This file may be used under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation and
 * appearing in the file LICENSE.LGPL included in the packaging of
 * this file, either version 2.1 of the License, or (at your option) any
 * later version.  Please review the following information to ensure the
 * GNU Lesser General Public License version 2.1 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 * If you have questions regarding the use of this file, please contact
 * us via http://www.maui-project.org/.
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
