/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 * Copyright (C) 2006 Tobias Koenig
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Tobias Koenig <tokoe@kde.org>
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

#include "vpagewidget.h"
#include "vpagewidget_p.h"
#include "vpagewidgetmodel.h"

/*
 * VPageWidgetPrivate
 */

VPageWidgetPrivate::VPageWidgetPrivate(VPageWidget *q) :
    VPageViewPrivate(q)
{
}

void VPageWidgetPrivate::_q_slotCurrentPageChanged(const QModelIndex &current, const QModelIndex &before)
{
    VPageWidgetItem *currentItem = 0;
    if (current.isValid())
        currentItem = model()->item(current);

    VPageWidgetItem *beforeItem = 0;
    if (before.isValid())
        beforeItem = model()->item(before);

    Q_Q(VPageWidget);
    emit q->currentPageChanged(currentItem, beforeItem);
}

/*
 * VPageWidget
 */

VPageWidget::VPageWidget(VPageWidgetPrivate &dd, QWidget *parent) :
    VPageView(dd, parent)
{
    Q_D(VPageWidget);

    connect(this, SIGNAL(currentPageChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(_q_slotCurrentPageChanged(const QModelIndex &, const QModelIndex &)));

    if (!d->VPageViewPrivate::model)
        setModel(new VPageWidgetModel(this));
    else
        Q_ASSERT(qobject_cast<VPageWidgetModel *>(d->VPageViewPrivate::model));

    connect(d->model(), SIGNAL(toggled(VPageWidgetItem *, bool)),
            this, SIGNAL(pageToggled(VPageWidgetItem *, bool)));
}

VPageWidget::VPageWidget(QWidget *parent) :
    VPageView(*new VPageWidgetPrivate(this), parent)
{
    Q_D(VPageWidget);

    connect(this, SIGNAL(currentPageChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(_q_slotCurrentPageChanged(const QModelIndex &, const QModelIndex &)));

    setModel(new VPageWidgetModel(this));

    connect(d->model(), SIGNAL(toggled(VPageWidgetItem *, bool)),
            this, SIGNAL(pageToggled(VPageWidgetItem *, bool)));
}

VPageWidget::~VPageWidget()
{
}

VPageWidgetItem *VPageWidget::addPage(QWidget *widget, const QString &name)
{
    return d_func()->model()->addPage(widget, name);
}

void VPageWidget::addPage(VPageWidgetItem *item)
{
    d_func()->model()->addPage(item);
}

VPageWidgetItem *VPageWidget::insertPage(VPageWidgetItem *before, QWidget *widget, const QString &name)
{
    return d_func()->model()->insertPage(before, widget, name);
}

void VPageWidget::insertPage(VPageWidgetItem *before, VPageWidgetItem *item)
{
    d_func()->model()->insertPage(before, item);
}

VPageWidgetItem *VPageWidget::addSubPage(VPageWidgetItem *parent, QWidget *widget, const QString &name)
{
    return d_func()->model()->addSubPage(parent, widget, name);
}

void VPageWidget::addSubPage(VPageWidgetItem *parent, VPageWidgetItem *item)
{
    d_func()->model()->addSubPage(parent, item);
}

void VPageWidget::removePage(VPageWidgetItem *item)
{
    // Emit signal before we remove it, because the item will be deleted in the model
    emit pageRemoved(item);
    d_func()->model()->removePage(item);
}

void VPageWidget::setCurrentPage(VPageWidgetItem *item)
{
    const QModelIndex index = d_func()->model()->index(item);
    if (!index.isValid())
        return;

    VPageView::setCurrentPage(index);
}

VPageWidgetItem *VPageWidget::currentPage() const
{
    const QModelIndex index = VPageView::currentPage();

    if (!index.isValid())
        return 0;

    return d_func()->model()->item(index);
}

#include "moc_vpagewidget.cpp"
