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

#ifndef VBREADCRUMBVIEW_H
#define VBREADCRUMBVIEW_H

#include <QAbstractItemView>

#include <VibeCore/VGlobal>

class VBreadcrumbViewPrivate;

class VIBE_EXPORT VBreadcrumbView : public QAbstractItemView
{
    Q_OBJECT
public:
    explicit VBreadcrumbView(QWidget *parent = 0);

    virtual QSize sizeHint() const;

    virtual void setModel(QAbstractItemModel *model);

    virtual QModelIndex indexAt(const QPoint &point) const;
    virtual void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
    virtual QRect visualRect(const QModelIndex &index) const;

public slots:
    virtual void reset();
    void back();
    void next();
    void enterTree(const QModelIndex &index);

protected:
    virtual int horizontalOffset() const;
    virtual int verticalOffset() const;
    virtual bool isIndexHidden(const QModelIndex &index) const;
    virtual QModelIndex moveCursor(CursorAction action, Qt::KeyboardModifiers mods);
    virtual void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags flags);
    virtual QRegion visualRegionForSelection(const QItemSelection &selection) const;

private:
    Q_DECLARE_PRIVATE(VBreadcrumbView)

    VBreadcrumbViewPrivate *const d_ptr;
};

#endif // VBREADCRUMBVIEW_H
