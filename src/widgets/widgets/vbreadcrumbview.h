/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
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

#ifndef VBREADCRUMBVIEW_H
#define VBREADCRUMBVIEW_H

#include <QAbstractItemView>

#include <VibeWidgets/VibeWidgetsExport>

class VBreadcrumbViewPrivate;

class VIBEWIDGETS_EXPORT VBreadcrumbView : public QAbstractItemView
{
    Q_OBJECT
public:
    explicit VBreadcrumbView(QWidget *parent = 0);

    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;
    virtual QSize maximumSizeHint() const;

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
