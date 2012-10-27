/****************************************************************************
** Copyright (c) 2006 - 2011, the LibQxt project.
** See the Qxt AUTHORS file for a list of authors and copyright holders.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the LibQxt project nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
** <http://libqxt.org>  <foundation@libqxt.org>
*****************************************************************************/

#ifndef VBREADCRUMBVIEW_H
#define VBREADCRUMBVIEW_H

#include <QAbstractItemView>

#include <VibeWidgets/VibeWidgetsExport>

QT_FORWARD_DECLARE_CLASS(QListView)
class VBreadcrumbViewPrivate;

class VIBEWIDGETS_EXPORT VBreadcrumbView : public QAbstractItemView
{
    Q_OBJECT
public:
    VBreadcrumbView(QWidget *parent = 0);

    virtual void setModel(QAbstractItemModel *model);

    virtual QModelIndex indexAt(const QPoint &point) const;
    virtual void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
    virtual QRect visualRect(const QModelIndex &index) const;

    void setItemDelegate(QAbstractItemDelegate *delegate);

    QAbstractItemDelegate *crumbDelegate() const;
    virtual void setCrumbDelegate(QAbstractItemDelegate *delegate);

    QAbstractItemView *itemView() const;

public slots:
    virtual void reset();
    void back();
    void enterTree(const QModelIndex &index);

protected:
    virtual int horizontalOffset() const;
    virtual int verticalOffset() const;
    virtual bool isIndexHidden(const QModelIndex &index) const;
    virtual QModelIndex moveCursor(CursorAction action, Qt::KeyboardModifiers mods);
    virtual void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags flags);
    virtual QRegion visualRegionForSelection(const QItemSelection &selection) const;

    virtual void showEvent(QShowEvent *event);
    virtual void paintEvent(QPaintEvent *event);

private:
    Q_DECLARE_PRIVATE(VBreadcrumbView)
    VBreadcrumbViewPrivate *const d_ptr;
};

#endif // VBREADCRUMBVIEW_H
