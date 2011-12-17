/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 * Copyright (C) 2007 Rafael Fernández López
 * Copyright (C) 2006 Tobias Koenig
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *	Rafael Fernández López <ereslibre@kde.org>
 *	Tobias Koenig <tokoe@kde.org>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

#ifndef VPAGEVIEW_P_H
#define VPAGEVIEW_P_H

#include <QAbstractItemDelegate>
#include <QGridLayout>
#include <QStackedWidget>
#include <QAbstractProxyModel>
#include <QListView>
#include <QTabBar>
#include <QTreeView>

#include "vpageview.h"
#include "vtitlewidget.h"

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

class VPageStackedWidget : public QStackedWidget
{
public:
    VPageStackedWidget(QWidget *parent = 0) :
        QStackedWidget(parent) {
    }

    void setMinimumSize(const QSize &size) {
        mMinimumSize = size;
    }

    virtual QSize minimumSizeHint() const {
        return mMinimumSize.expandedTo(QStackedWidget::minimumSizeHint());
    }

private:
    QSize mMinimumSize;
};

class VPageViewPrivate
{
    Q_DECLARE_PUBLIC(VPageView)
protected:
    VPageViewPrivate(VPageView *);

    VPageView *q_ptr;

    // data
    QAbstractItemModel *model;
    VPageView::FaceType faceType;

    // gui
    QGridLayout *layout;
    VPageStackedWidget *stack;
    VTitleWidget *titleWidget;
    QWidget *defaultWidget;

    QAbstractItemView *view;

    void updateTitleWidget(const QModelIndex &index);

    void updateSelection();
    void cleanupPages();
    QList<QWidget *> collectPages(const QModelIndex &parent = QModelIndex());
    VPageView::FaceType detectAutoFace() const;

    // private slots
    void _q_rebuildGui();
    void _q_modelChanged();
    void _q_dataChanged(const QModelIndex &, const QModelIndex &);
    void _q_pageSelected(const QItemSelection &, const QItemSelection &);

private:
    void init();
};

namespace VPrivate
{

    class VPageListViewDelegate;
    class VPageListViewProxy;

    class VPagePlainView : public QAbstractItemView
    {
    public:
        VPagePlainView(QWidget *parent = 0);

        virtual QModelIndex indexAt(const QPoint &point) const;
        virtual void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
        virtual QRect visualRect(const QModelIndex &index) const;

    protected:
        virtual QModelIndex moveCursor(QAbstractItemView::CursorAction, Qt::KeyboardModifiers);
        virtual int horizontalOffset() const;
        virtual int verticalOffset() const;
        virtual bool isIndexHidden(const QModelIndex &) const;
        virtual void setSelection(const QRect &, QFlags<QItemSelectionModel::SelectionFlag>);
        virtual QRegion visualRegionForSelection(const QItemSelection &) const;
    };

    class VPageListView : public QListView
    {
        Q_OBJECT
    public:
        VPageListView(QWidget *parent = 0);
        virtual ~VPageListView();

        virtual void setModel(QAbstractItemModel *model);

    private slots:
        void updateWidth();
    };

    class VPageTreeView : public QTreeView
    {
        Q_OBJECT
    public:
        VPageTreeView(QWidget *parent = 0);

        virtual void setModel(QAbstractItemModel *model);

    private slots:
        void updateWidth();

    private:
        void expandItems(const QModelIndex &index = QModelIndex());
    };

    class VPageTabbedView : public QAbstractItemView
    {
        Q_OBJECT
    public:
        VPageTabbedView(QWidget *parent = 0);
        virtual ~VPageTabbedView();

        virtual void setModel(QAbstractItemModel *model);

        virtual QModelIndex indexAt(const QPoint &point) const;
        virtual void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
        virtual QRect visualRect(const QModelIndex &index) const;

        virtual QSize minimumSizeHint() const;

    protected:
        virtual QModelIndex moveCursor(QAbstractItemView::CursorAction, Qt::KeyboardModifiers);
        virtual int horizontalOffset() const;
        virtual int verticalOffset() const;
        virtual bool isIndexHidden(const QModelIndex &) const;
        virtual void setSelection(const QRect &, QFlags<QItemSelectionModel::SelectionFlag>);
        virtual QRegion visualRegionForSelection(const QItemSelection &) const;

    private Q_SLOTS:
        void currentPageChanged(int);
        void layoutChanged();
        virtual void dataChanged(const QModelIndex &, const QModelIndex &);

    private:
        QTabWidget *mTabWidget;
    };

    class VPageListViewDelegate : public QAbstractItemDelegate
    {
        Q_OBJECT
    public:
        VPageListViewDelegate(QObject *parent = 0);

        virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    private slots:
        void iconSettingsChanged(int group);

    private:
        void drawFocus(QPainter *, const QStyleOptionViewItem &, const QRect &) const;
        int mIconSize;
    };

    /**
     * We need this proxy model to map the leaves of a tree-like model
     * to a one-level list model.
     */
    class VPageListViewProxy : public QAbstractProxyModel
    {
        Q_OBJECT
    public:
        VPageListViewProxy(QObject *parent = 0);
        virtual ~VPageListViewProxy();

        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex &) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual QModelIndex mapFromSource(const QModelIndex &index) const;
        virtual QModelIndex mapToSource(const QModelIndex &index) const;

    public slots:
        void rebuildMap();

    private:
        void addMapEntry(const QModelIndex &);

        QList<QModelIndex> mList;
    };

    class SelectionModel : public QItemSelectionModel
    {
        Q_OBJECT
    public:
        SelectionModel(QAbstractItemModel *model, QObject *parent);

    public slots:
        virtual void clear();
        virtual void select(const QModelIndex &index, QItemSelectionModel::SelectionFlags command);
        virtual void select(const QItemSelection &selection, QItemSelectionModel::SelectionFlags command);
    };
}

#endif // VPAGEVIEW_P_H
