/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 * Copyright (C) 2007 Rafael Fernández López
 * Copyright (C) 2006 Tobias Koenig
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Rafael Fernández López <ereslibre@kde.org>
 *    Tobias Koenig <tokoe@kde.org>
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

#include <QAbstractItemView>
#include <QGridLayout>
#include <QLabel>
#include <QSize>
#include <QStackedWidget>
#include <QTimer>

#include "vpageview.h"
#include "vpageview_p.h"
#include "vpagemodel.h"
#include "vtitlewidget.h"

/*
 * VPageViewPrivate
 */

void VPageViewPrivate::_q_rebuildGui()
{
    // Clean up old view
    Q_Q(VPageView);

    QModelIndex currentLastIndex;
    if (view && view->selectionModel()) {
        QObject::disconnect(view->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
                            q, SLOT(_q_pageSelected(const QItemSelection &, const QItemSelection &)));
        currentLastIndex = view->selectionModel()->currentIndex();
    }

    delete view;
    view = q->createView();

    Q_ASSERT(view);

    view->setSelectionBehavior(QAbstractItemView::SelectItems);
    view->setSelectionMode(QAbstractItemView::SingleSelection);

    if (model) {
        view->setModel(model);
    }

    // Setup new view
    if (view->selectionModel()) {
        QObject::connect(view->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), q, SLOT(_q_pageSelected(const QItemSelection &, const QItemSelection &)));

        if (currentLastIndex.isValid())
            view->selectionModel()->setCurrentIndex(currentLastIndex, QItemSelectionModel::Select);
        else if (model)
            view->selectionModel()->setCurrentIndex(model->index(0, 0), QItemSelectionModel::Select);
    }

    if (faceType == VPageView::Tabbed) {
        stack->setVisible(false);
        layout->removeWidget(stack);
    } else {
        layout->addWidget(stack, 2, 1);
        stack->setVisible(true);
    }

    titleWidget->setVisible(q->showPageHeader());

    Qt::Alignment alignment = q->viewPosition();
    if (alignment & Qt::AlignTop)
        layout->addWidget(view, 2, 1);
    else if (alignment & Qt::AlignRight)
        layout->addWidget(view, 1, 2, 2, 1);
    else if (alignment & Qt::AlignBottom)
        layout->addWidget(view, 4, 1);
    else if (alignment & Qt::AlignLeft)
        layout->addWidget(view, 1, 0, 2, 1);
}

void VPageViewPrivate::updateSelection()
{
    /**
     * Select the first item in the view if not done yet.
     */

    if (!model)
        return;

    if (!view || !view->selectionModel())
        return;

    const QModelIndex index = view->selectionModel()->currentIndex();
    if (!index.isValid())
        view->selectionModel()->setCurrentIndex(model->index(0, 0), QItemSelectionModel::Select);
}

void VPageViewPrivate::cleanupPages()
{
    /**
     * Remove all orphan pages from the stacked widget.
     */

    const QList<QWidget *> widgets = collectPages();

    for (int i = 0; i < stack->count(); ++i) {
        QWidget *page = stack->widget(i);

        bool found = false;
        for (int j = 0; j < widgets.count(); ++j) {
            if (widgets[ j ] == page)
                found = true;
        }

        if (!found)
            stack->removeWidget(page);
    }
}

QList<QWidget *> VPageViewPrivate::collectPages(const QModelIndex &parentIndex)
{
    /**
     * Traverse through the model recursive and collect all widgets in
     * a list.
     */
    QList<QWidget *> retval;

    int rows = model->rowCount(parentIndex);
    for (int j = 0; j < rows; ++j) {
        const QModelIndex index = model->index(j, 0, parentIndex);
        retval.append(qvariant_cast<QWidget *>(model->data(index, VPageModel::WidgetRole)));

        if (model->rowCount(index) > 0) {
            retval += collectPages(index);
        }
    }

    return retval;
}

VPageView::FaceType VPageViewPrivate::detectAutoFace() const
{
    if (!model)
        return VPageView::Plain;

    /**
     * Check whether the model has sub pages.
     */
    bool hasSubPages = false;
    const int count = model->rowCount();
    for (int i = 0; i < count; ++i) {
        if (model->rowCount(model->index(i, 0)) > 0) {
            hasSubPages = true;
            break;
        }
    }

    if (hasSubPages)
        return VPageView::Tree;

    if (model->rowCount() > 1)
        return VPageView::List;

    return VPageView::Plain;
}

void VPageViewPrivate::_q_modelChanged()
{
    if (!model)
        return;

    /**
     * If the face type is Auto, we rebuild the GUI whenever the layout
     * of the model changes.
     */
    if (faceType == VPageView::Auto) {
        _q_rebuildGui();
        // If you discover some crashes use the line below instead...
        //QTimer::singleShot(0, q, SLOT(_q_rebuildGui()));
    }

    /**
     * Set the stack to the minimum size of the largest widget.
     */
    QSize size = stack->size();
    const QList<QWidget *> widgets = collectPages();
    for (int i = 0; i < widgets.count(); ++i) {
        const QWidget *widget = widgets[ i ];
        if (widget)
            size = size.expandedTo(widget->minimumSizeHint());
    }
    stack->setMinimumSize(size);

    updateSelection();
}

void VPageViewPrivate::_q_pageSelected(const QItemSelection &index, const QItemSelection &previous)
{
    if (!model)
        return;

    // Return if the current Index is not valid
    if (index.indexes().size() != 1) {
        return;
    }
    QModelIndex currentIndex = index.indexes().first();

    QModelIndex previousIndex;
    // The previous index can be invalid
    if (previous.indexes().size() == 1) {
        previousIndex = previous.indexes().first();
    }

    if (faceType != VPageView::Tabbed) {
        QWidget *widget = qvariant_cast<QWidget *>(model->data(currentIndex, VPageModel::WidgetRole));

        if (widget) {
            if (stack->indexOf(widget) == -1) {     // not included yet
                stack->addWidget(widget);
            }

            stack->setCurrentWidget(widget);
        } else {
            stack->setCurrentWidget(defaultWidget);
        }

        updateTitleWidget(currentIndex);
    }

    Q_Q(VPageView);
    emit q->currentPageChanged(currentIndex, previousIndex);
}

void VPageViewPrivate::updateTitleWidget(const QModelIndex &index)
{
    Q_Q(VPageView);

    QString header = model->data(index, VPageModel::HeaderRole).toString();
    if (header.isNull()) {   //TODO KDE5 remove that ugly logic, see also doxy-comments in VPageWidgetItem::setHeader()
        header = model->data(index, Qt::DisplayRole).toString();
    }

    const QIcon icon = model->data(index, Qt::DecorationRole).value<QIcon>();
    titleWidget->setPixmap(icon.pixmap(22, 22));
    titleWidget->setText(header);

    titleWidget->setVisible(q->showPageHeader());
}

void VPageViewPrivate::_q_dataChanged(const QModelIndex &, const QModelIndex &)
{
    /**
     * When data has changed we update the header and icon for the currently selected
     * page.
     */
    if (!view)
        return;

    QModelIndex index = view->selectionModel()->currentIndex();
    if (!index.isValid())
        return;

    updateTitleWidget(index);
}

VPageViewPrivate::VPageViewPrivate(VPageView *_parent)
    : q_ptr(_parent), model(0), faceType(VPageView::Auto),
      layout(0), stack(0), titleWidget(0), view(0)
{
}

void VPageViewPrivate::init()
{
    Q_Q(VPageView);
    layout = new QGridLayout(q);
    stack = new VPageStackedWidget(q);
    titleWidget = new VTitleWidget(q);
    QPixmap emptyPixmap(22, 22);
    emptyPixmap.fill(Qt::transparent);
    titleWidget->setPixmap(emptyPixmap);
    layout->addWidget(titleWidget, 1, 1);
    layout->addWidget(stack, 2, 1);

    defaultWidget = new QWidget(q);
    stack->addWidget(defaultWidget);

    // stack should use most space
    layout->setColumnStretch(1, 1);
    layout->setRowStretch(2, 1);
}

/*
 * VPageView
 */

VPageView::VPageView(QWidget *parent) :
    QWidget(parent),
    d_ptr(new VPageViewPrivate(this))
{
    d_ptr->init();
}

VPageView::VPageView(VPageViewPrivate &dd, QWidget *parent) :
    QWidget(parent),
    d_ptr(&dd)
{
    d_ptr->init();
}

VPageView::~VPageView()
{
    delete d_ptr;
}

void VPageView::setModel(QAbstractItemModel *model)
{
    Q_D(VPageView);

    // clean up old model
    if (d->model) {
        disconnect(d->model, SIGNAL(layoutChanged()), this, SLOT(_q_modelChanged()));
        disconnect(d->model, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
                   this, SLOT(_q_dataChanged(const QModelIndex &, const QModelIndex &)));
    }

    d->model = model;

    if (d->model) {
        connect(d->model, SIGNAL(layoutChanged()), this, SLOT(_q_modelChanged()));
        connect(d->model, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
                this, SLOT(_q_dataChanged(const QModelIndex &, const QModelIndex &)));

        // set new model in navigation view
        if (d->view)
            d->view->setModel(model);
    }

    d->_q_rebuildGui();
}

QAbstractItemModel *VPageView::model() const
{
    return d_func()->model;
}

void VPageView::setFaceType(FaceType faceType)
{
    Q_D(VPageView);
    d->faceType = faceType;

    d->_q_rebuildGui();
}

VPageView::FaceType VPageView::faceType() const
{
    return d_func()->faceType;
}

void VPageView::setCurrentPage(const QModelIndex &index)
{
    Q_D(VPageView);
    if (!d->view || !d->view->selectionModel())
        return;

    d->view->selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);
}

QModelIndex VPageView::currentPage() const
{
    Q_D(const VPageView);
    if (!d->view || !d->view->selectionModel())
        return QModelIndex();

    return d->view->selectionModel()->currentIndex();
}

void VPageView::setItemDelegate(QAbstractItemDelegate *delegate)
{
    Q_D(VPageView);
    if (d->view)
        d->view->setItemDelegate(delegate);
}

QAbstractItemDelegate *VPageView::itemDelegate() const
{
    Q_D(const VPageView);
    if (d->view)
        return d->view->itemDelegate();
    else
        return 0;
}

void VPageView::setDefaultWidget(QWidget *widget)
{
    Q_D(VPageView);

    Q_ASSERT(widget);

    bool isCurrent = (d->stack->currentIndex() == d->stack->indexOf(d->defaultWidget));

    // Remove old default widget
    d->stack->removeWidget(d->defaultWidget);
    delete d->defaultWidget;

    // Add new default widget
    d->defaultWidget = widget;
    d->stack->addWidget(d->defaultWidget);

    if (isCurrent)
        d->stack->setCurrentWidget(d->defaultWidget);
}

QAbstractItemView *VPageView::createView()
{
    Q_D(VPageView);

    if (d->faceType == Auto) {
        const FaceType faceType = d->detectAutoFace();

        if (faceType == Plain)
            return new VPrivate::VPagePlainView(this);
        else if (faceType == List)
            return new VPrivate::VPageListView(this);
        else if (faceType == Tree)
            return new VPrivate::VPageTreeView(this);
        else // should never happen
            return 0;
    } else if (d->faceType == Plain)
        return new VPrivate::VPagePlainView(this);
    else if (d->faceType == List)
        return new VPrivate::VPageListView(this);
    else if (d->faceType == Tree)
        return new VPrivate::VPageTreeView(this);
    else if (d->faceType == Tabbed)
        return new VPrivate::VPageTabbedView(this);
    else
        return 0;
}

bool VPageView::showPageHeader() const
{
    Q_D(const VPageView);
    FaceType faceType = d->faceType;

    if (faceType == Auto)
        faceType = d->detectAutoFace();

    if (faceType == Tabbed) {
        return false;
    } else {
        return !d->titleWidget->text().isEmpty();
    }
}

Qt::Alignment VPageView::viewPosition() const
{
    Q_D(const VPageView);
    FaceType faceType = d->faceType;

    if (faceType == Auto)
        faceType = d->detectAutoFace();

    if (faceType == Plain || faceType == Tabbed)
        return Qt::AlignTop;
    else
        return Qt::AlignLeft;
}

#include "moc_vpageview.cpp"
