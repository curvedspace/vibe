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

#include <QApplication>
#include <QDebug>
#include <QPaintEvent>
#include <QListView>
#include <QToolButton>
#include <QBoxLayout>
#include <QModelIndex>
#include <QAbstractItemDelegate>
#include <QStyle>
#include <QStylePainter>
#include <QSize>
#include <QFont>
#include <QtAlgorithms>

#include "vbreadcrumbview.h"
#include "vbreadcrumbview_p.h"

// This class exists only to grant access to QListView's protected members
class VBreadcrumbViewList : public QListView
{
    friend class VBreadcrumbView;
public:
    VBreadcrumbViewList(QWidget *parent)
        : QListView(parent) {
    }
};

VBreadcrumbViewDelegate::VBreadcrumbViewDelegate(QAbstractItemDelegate *other, QObject *parent)
    : QAbstractItemDelegate(parent), delegate(other)
{
    QObject::connect(other, SIGNAL(closeEditor(QWidget *, QAbstractItemDelegate::EndEditHint)),
                     this, SIGNAL(closeEditor(QWidget *, QAbstractItemDelegate::EndEditHint)));
    QObject::connect(other, SIGNAL(commitData(QWidget *)), this, SIGNAL(commitData(QWidget *)));
    QObject::connect(other, SIGNAL(sizeHintChanged(QModelIndex)), this, SIGNAL(sizeHintChanged(QModelIndex)));

}

void VBreadcrumbViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    delegate->paint(painter, option, index);

    if (!index.model()->hasChildren(index))
        return;

    int arrow = 8;
    int pad = (option.rect.height() - arrow) / 2;
    QStyleOption arrowOption;
    arrowOption = option;
    arrowOption.rect = QRect(option.rect.right() - arrow, option.rect.top() + pad, arrow, arrow);
    QApplication::style()->drawPrimitive(QStyle::PE_IndicatorArrowRight, &arrowOption, painter);
}

QSize VBreadcrumbViewDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return delegate->sizeHint(option, index) + QSize(8, 0);
}

QWidget *VBreadcrumbViewDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return delegate->createEditor(parent, option, index);
}

void VBreadcrumbViewDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    delegate->setEditorData(editor, index);
}

void VBreadcrumbViewDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    delegate->setModelData(editor, model, index);
}

void VBreadcrumbViewDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    delegate->updateEditorGeometry(editor, option, index);
}

bool VBreadcrumbViewDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return delegate->editorEvent(event, model, option, index);
}

class VBreadcrumbViewButton : public QAbstractButton
{
public:
    VBreadcrumbViewButton(const QModelIndex &idx, VBreadcrumbView *parent)
        : QAbstractButton(parent), index(idx) {
        // initializers only
    }

    QModelIndex index;

    QSize sizeHint() const {
        QStyleOptionViewItem itemOption;
        itemOption.initFrom(this);
        int border = style()->pixelMetric(QStyle::PM_DefaultFrameWidth) * 2;
        int arrow = 8;
        if (index.isValid())
            return static_cast<VBreadcrumbView *>(parent())->itemDelegate()->sizeHint(itemOption, index) + QSize(border + arrow, border);
        else
            return QSize(border + arrow, border + arrow);
    }

protected:
    void enterEvent(QEvent *event) {
        Q_UNUSED(event);
        update();
    }

    void leaveEvent(QEvent *event) {
        Q_UNUSED(event);
        update();
    }

    void paintEvent(QPaintEvent *event) {
        Q_UNUSED(event);
        QStylePainter painter(this);
        QStyleOptionButton option;
        option.initFrom(this);
        if (rect().contains(mapFromGlobal(QCursor::pos())))
            painter.drawPrimitive(QStyle::PE_PanelButtonTool, option);
        int border = painter.style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
        option.rect = option.rect.adjusted(border, border, -border, -border);
        if (index.isValid()) {
            QAbstractItemDelegate *delegate = static_cast<VBreadcrumbView *>(parent())->crumbDelegate();
            QStyleOptionViewItem itemOption;
            itemOption.initFrom(this);
            itemOption.rect = option.rect;
            delegate->paint(&painter, itemOption, index);
        }
        int arrow = 8;
        int pad = (height() - (2 * border) - arrow) / 2;
        QStyleOption arrowOption;
        arrowOption.initFrom(this);
        arrowOption.rect = QRect(width() - border - arrow, pad, arrow, arrow);
        painter.drawPrimitive(QStyle::PE_IndicatorArrowRight, arrowOption);
    }
};

VBreadcrumbViewPrivate::VBreadcrumbViewPrivate(VBreadcrumbView *parent)
    : crumbDelegate(0)
    , q_ptr(parent)
{
    // initializers only
}

void VBreadcrumbViewPrivate::addCrumb(const QModelIndex &index)
{
    Q_Q(VBreadcrumbView);

    crumbs.append(index);
    VBreadcrumbViewButton *button = new VBreadcrumbViewButton(index, q);
    button->setFocusPolicy(Qt::NoFocus);
    button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    buttons.append(button);
    buttonLayout->insertWidget(buttons.count() - 1, button, 0);
    QObject::connect(button, SIGNAL(clicked()), this, SLOT(buttonPressed()));
}

void VBreadcrumbViewPrivate::buttonPressed()
{
    Q_Q(VBreadcrumbView);
    q->enterTree(static_cast<VBreadcrumbViewButton *>(sender())->index);
}

VBreadcrumbView::VBreadcrumbView(QWidget *parent)
    : QAbstractItemView(parent)
    , d_ptr(new VBreadcrumbViewPrivate(this))
{
    Q_D(VBreadcrumbView);

    setAutoFillBackground(false);
    setBackgroundRole(QPalette::Window);
    viewport()->setAutoFillBackground(false);
    viewport()->setBackgroundRole(QPalette::Window);
    setFrameStyle(QFrame::NoFrame);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    d->buttonLayout = new QHBoxLayout;
    d->buttonLayout->setContentsMargins(0, 0, 0, 0);
    d->buttonLayout->setSpacing(0);
    d->buttonLayout->addStretch(1);
    QToolButton *backButton = new QToolButton(this);
    backButton->setIcon(style()->standardPixmap(QStyle::SP_FileDialogBack));
    backButton->setAutoRaise(true);
    backButton->setFocusPolicy(Qt::NoFocus);
    d->buttonLayout->addWidget(backButton);
    layout->addLayout(d->buttonLayout, 0);
    d->view = new VBreadcrumbViewList(this);
    layout->addWidget(d->view, 1);
    d->addCrumb(QModelIndex());

    QObject::connect(d->view, SIGNAL(activated(QModelIndex)), this, SLOT(enterTree(QModelIndex)));
    QObject::connect(backButton, SIGNAL(clicked()), this, SLOT(back()));

    // FIXME: Temporarily hide the list view and back button, we need
    //        to do something better, some programs like Swordfish don't even
    //        want to create it
    d->view->hide();
    backButton->hide();
}

void VBreadcrumbView::setModel(QAbstractItemModel *model)
{
    Q_D(VBreadcrumbView);

    d->view->setModel(model);
    QAbstractItemView::setModel(model);
    delete d->view->itemDelegate();
    d->view->setItemDelegate(new VBreadcrumbViewDelegate(itemDelegate(), this));
}

void VBreadcrumbView::reset()
{
    Q_D(VBreadcrumbView);

    QAbstractItemView::reset();
    qDeleteAll(d->buttons);
    d->crumbs.clear();
    d->buttons.clear();
    d->addCrumb(QModelIndex());
    d->view->reset();
}

int VBreadcrumbView::horizontalOffset() const
{
    Q_D(const VBreadcrumbView);
    return d->view->horizontalOffset();
}

int VBreadcrumbView::verticalOffset() const
{
    Q_D(const VBreadcrumbView);
    return d->view->verticalOffset();
}

QModelIndex VBreadcrumbView::indexAt(const QPoint &point) const
{
    Q_D(const VBreadcrumbView);
    return d->view->indexAt(d->view->mapFromParent(point));
}

bool VBreadcrumbView::isIndexHidden(const QModelIndex &index) const
{
    Q_D(const VBreadcrumbView);
    return d->view->isIndexHidden(index);
}

QModelIndex VBreadcrumbView::moveCursor(CursorAction action, Qt::KeyboardModifiers mods)
{
    Q_D(VBreadcrumbView);
    return d->view->moveCursor(action, mods);
}

void VBreadcrumbView::scrollTo(const QModelIndex &index, ScrollHint hint)
{
    Q_D(VBreadcrumbView);

    if (index.parent() != d->view->rootIndex())
        // TODO: set the breadcrumbs and the view's root index correctly
        d->view->scrollTo(index, hint);
}

void VBreadcrumbView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags flags)
{
    Q_D(VBreadcrumbView);

    QPoint tl = d->view->mapFromParent(rect.topLeft());
    d->view->setSelection(QRect(tl, rect.size()), flags);
}

QRect VBreadcrumbView::visualRect(const QModelIndex &index) const
{
    Q_D(const VBreadcrumbView);

    QRect rect = d->view->visualRect(index);
    return QRect(d->view->mapToParent(rect.topLeft()), rect.size());
}

QRegion VBreadcrumbView::visualRegionForSelection(const QItemSelection &selection) const
{
    Q_D(const VBreadcrumbView);

    QRegion region = d->view->visualRegionForSelection(selection);
    return region.translated(d->view->pos());
}

void VBreadcrumbView::enterTree(const QModelIndex &index)
{
    Q_D(VBreadcrumbView);

    if (!model())
        return;
    if (!model()->hasChildren(index)) {
        emit activated(index);
        return;
    }
    if (index == d->view->rootIndex()) {
        // do nothing but reload the view
    } else if (index.parent() != d->view->rootIndex()) {
        foreach(VBreadcrumbViewButton * b, d->buttons) b->deleteLater();
        d->crumbs.clear();
        d->buttons.clear();
        d->addCrumb(QModelIndex());
        QList<QModelIndex> chain;
        QModelIndex pos = index;
        while (pos.isValid()) {
            chain.append(pos);
            pos = pos.parent();
        }
        while (!chain.isEmpty()) {
            d->addCrumb(chain.last());
            chain.removeLast();
        }
    } else {
        d->addCrumb(index);
    }
    d->view->setRootIndex(index);
}

void VBreadcrumbView::back()
{
    Q_D(VBreadcrumbView);

    if (d->crumbs.count() <= 1)
        return;
    d->buttons.last()->deleteLater();
    d->buttons.removeLast();
    d->crumbs.removeLast();
    d->view->setRootIndex(d->crumbs.last());
}

void VBreadcrumbView::setItemDelegate(QAbstractItemDelegate *delegate)
{
    QAbstractItemView::setItemDelegate(delegate);
    //delete d->view->itemDelegate();
    //d->view->setItemDelegate(new VBreadcrumbViewDelegate(itemDelegate(), this));
}

QAbstractItemView *VBreadcrumbView::itemView() const
{
    Q_D(const VBreadcrumbView);
    return d->view;
}

void VBreadcrumbView::showEvent(QShowEvent *event)
{
    Q_D(VBreadcrumbView);

    VBreadcrumbViewDelegate *viewDelegate = qobject_cast<VBreadcrumbViewDelegate *>(d->view->itemDelegate());
    if (viewDelegate->delegate != itemDelegate()) {
        d->view->setItemDelegate(new VBreadcrumbViewDelegate(itemDelegate(), this));
        delete viewDelegate;
        d->view->reset();
    }
    QAbstractItemView::showEvent(event);
}

void VBreadcrumbView::paintEvent(QPaintEvent *event)
{
    Q_D(VBreadcrumbView);

    VBreadcrumbViewDelegate *viewDelegate = qobject_cast<VBreadcrumbViewDelegate *>(d->view->itemDelegate());
    if (viewDelegate->delegate != itemDelegate()) {
        d->view->setItemDelegate(new VBreadcrumbViewDelegate(itemDelegate(), this));
        delete viewDelegate;
        d->view->reset();
    }
    QAbstractItemView::paintEvent(event);
}

QAbstractItemDelegate *VBreadcrumbView::crumbDelegate() const
{
    Q_D(const VBreadcrumbView);

    QAbstractItemDelegate *rv = d->crumbDelegate;
    if (!rv)
        return itemDelegate();
    return rv;
}

void VBreadcrumbView::setCrumbDelegate(QAbstractItemDelegate *delegate)
{
    Q_D(VBreadcrumbView);

    d->crumbDelegate = delegate;
    update();
    foreach(VBreadcrumbViewButton * button, d->buttons) {
        button->update();
    }
}
