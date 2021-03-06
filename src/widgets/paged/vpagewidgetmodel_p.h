/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
 * Copyright (c) 2007 Matthias Kretz
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Matthias Kretz <kretz@kde.org>
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

#ifndef VPAGEWIDGETMODEL_P_H
#define VPAGEWIDGETMODEL_P_H

#include "vpagemodel_p.h"
#include "vpagewidgetmodel.h"

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Vibe API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

class PageItem
{
public:
    explicit PageItem(VPageWidgetItem *pageItem, PageItem *parent = 0);
    ~PageItem();

    void appendChild(PageItem *child);
    void insertChild(int row, PageItem *child);
    void removeChild(int row);

    PageItem *child(int row);
    int childCount() const;
    int columnCount() const;
    int row() const;
    PageItem *parent();

    VPageWidgetItem *pageWidgetItem() const;

    PageItem *findChild(const VPageWidgetItem *item);

    void dump(int indent = 0);

private:
    VPageWidgetItem *mPageWidgetItem;

    QList<PageItem *> mChildItems;
    PageItem *mParentItem;
};

class VPageWidgetModelPrivate : public VPageModelPrivate
{
    Q_DECLARE_PUBLIC(VPageWidgetModel)
protected:
    VPageWidgetModelPrivate()
        : rootItem(new PageItem(0, 0)) {
    }

    ~VPageWidgetModelPrivate() {
        delete rootItem;
        rootItem = 0;
    }

    PageItem *rootItem;

    void _q_itemChanged() {
        Q_Q(VPageWidgetModel);
        VPageWidgetItem *item = qobject_cast<VPageWidgetItem *>(q->sender());
        if (!item) {
            return;
        }

        const QModelIndex index = q->index(item);
        if (!index.isValid()) {
            return;
        }

        emit q->dataChanged(index, index);
    }

    void _q_itemToggled(bool checked) {
        Q_Q(VPageWidgetModel);
        VPageWidgetItem *item = qobject_cast<VPageWidgetItem *>(q->sender());
        if (!item) {
            return;
        }

        emit q->toggled(item, checked);
    }
};

#endif // VPAGEWIDGETMODEL_P_H
