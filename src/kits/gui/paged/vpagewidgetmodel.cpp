/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 * Copyright (C) 2006 Tobias Koenig
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#include <QPointer>
#include <QWidget>
#include <QIcon>

#include "vpagewidgetmodel.h"
#include "vpagewidgetmodel_p.h"

/*
 * VPageWidgetItem::Private
 */

class VPageWidgetItem::Private
{
public:
    Private() :
        checkable(false),
        checked(false),
        enabled(true) {
    }

    ~Private() {
        delete widget;
        widget = 0;
    }

    QString name;
    QString header;
    QIcon icon;
    QPointer<QWidget> widget;
    bool checkable : 1;
    bool checked : 1;
    bool enabled : 1;
};

/*
 * VPageWidgetItem
 */

VPageWidgetItem::VPageWidgetItem(QWidget *widget) :
    QObject(0),
    d(new Private)
{
    d->widget = widget;

    /**
     * Hide the widget, otherwise when the widget has this VPageView as
     * parent the widget is shown outside the QStackedWidget if the page
     * was not selected ( and reparented ) yet.
     */
    if (d->widget)
        d->widget->hide();
}

VPageWidgetItem::VPageWidgetItem(QWidget *widget, const QString &name) :
    QObject(0),
    d(new Private)
{
    d->widget = widget;
    d->name = name;

    /**
     * Hide the widget, otherwise when the widget has this VPageView as
     * parent the widget is shown outside the QStackedWidget if the page
     * was not selected ( and reparented ) yet.
     */
    if (d->widget)
        d->widget->hide();
}

VPageWidgetItem::~VPageWidgetItem()
{
    delete d;
}

void VPageWidgetItem::setEnabled(bool enabled)
{
    d->enabled = enabled;
    if (d->widget) {
        d->widget->setEnabled(enabled);
    }
    emit changed();
}

bool VPageWidgetItem::isEnabled() const
{
    return d->enabled;
}

QWidget *VPageWidgetItem::widget() const
{
    return d->widget;
}

void VPageWidgetItem::setName(const QString &name)
{
    d->name = name;

    emit changed();
}

QString VPageWidgetItem::name() const
{
    return d->name;
}

void VPageWidgetItem::setHeader(const QString &header)
{
    d->header = header;

    emit changed();
}

QString VPageWidgetItem::header() const
{
    return d->header;
}

void VPageWidgetItem::setIcon(const QIcon &icon)
{
    d->icon = icon;

    emit changed();
}

QIcon VPageWidgetItem::icon() const
{
    return d->icon;
}

void VPageWidgetItem::setCheckable(bool checkable)
{
    d->checkable = checkable;

    emit changed();
}

bool VPageWidgetItem::isCheckable() const
{
    return d->checkable;
}

void VPageWidgetItem::setChecked(bool checked)
{
    d->checked = checked;

    emit toggled(checked);
    emit changed();
}

bool VPageWidgetItem::isChecked() const
{
    return d->checked;
}

/*
 * PageItem
 */

PageItem::PageItem(VPageWidgetItem *pageWidgetItem, PageItem *parent) :
    mPageWidgetItem(pageWidgetItem),
    mParentItem(parent)
{
}

PageItem::~PageItem()
{
    delete mPageWidgetItem;
    mPageWidgetItem = 0;

    qDeleteAll(mChildItems);
}

void PageItem::appendChild(PageItem *item)
{
    mChildItems.append(item);
}

void PageItem::insertChild(int row, PageItem *item)
{
    mChildItems.insert(row, item);
}

void PageItem::removeChild(int row)
{
    mChildItems.removeAt(row);
}

PageItem *PageItem::child(int row)
{
    return mChildItems.value(row);
}

int PageItem::childCount() const
{
    return mChildItems.count();
}

int PageItem::columnCount() const
{
    return 1;
}

PageItem *PageItem::parent()
{
    return mParentItem;
}

int PageItem::row() const
{
    if (mParentItem)
        return mParentItem->mChildItems.indexOf(const_cast<PageItem *>(this));

    return 0;
}

VPageWidgetItem *PageItem::pageWidgetItem() const
{
    return mPageWidgetItem;
}

PageItem *PageItem::findChild(const VPageWidgetItem *item)
{
    if (mPageWidgetItem == item)
        return this;

    for (int i = 0; i < mChildItems.count(); ++i) {
        PageItem *pageItem = mChildItems[ i ]->findChild(item);
        if (pageItem)
            return pageItem;
    }

    return 0;
}

void PageItem::dump(int indent)
{
    QString prefix;
    for (int i = 0; i < indent; ++i)
        prefix.append(" ");

    const QString name = (mPageWidgetItem ? mPageWidgetItem->name() : "root");
    qDebug("%s (%p)", qPrintable(QString("%1%2").arg(prefix, name)), (void *)this);
    for (int i = 0; i < mChildItems.count(); ++i)
        mChildItems[ i ]->dump(indent + 2);
}

/*
 * VPageWidgetModel
 */

VPageWidgetModel::VPageWidgetModel(QObject *parent) :
    VPageModel(*new VPageWidgetModelPrivate, parent)
{
}

VPageWidgetModel::~VPageWidgetModel()
{
}

int VPageWidgetModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant VPageWidgetModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    PageItem *item = static_cast<PageItem *>(index.internalPointer());

    if (role == Qt::DisplayRole)
        return QVariant(item->pageWidgetItem()->name());
    else if (role == Qt::DecorationRole)
        return QVariant(item->pageWidgetItem()->icon());
    else if (role == HeaderRole)
        return QVariant(item->pageWidgetItem()->header());
    else if (role == WidgetRole)
        return QVariant::fromValue(item->pageWidgetItem()->widget());
    else if (role == Qt::CheckStateRole) {
        if (item->pageWidgetItem()->isCheckable()) {
            return (item->pageWidgetItem()->isChecked() ? Qt::Checked : Qt::Unchecked);
        } else
            return QVariant();
    } else
        return QVariant();
}

bool VPageWidgetModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    if (role != Qt::CheckStateRole)
        return false;

    PageItem *item = static_cast<PageItem *>(index.internalPointer());
    if (!item)
        return false;

    if (!item->pageWidgetItem()->isCheckable())
        return false;

    if (value.toInt() == Qt::Checked)
        item->pageWidgetItem()->setChecked(true);
    else
        item->pageWidgetItem()->setChecked(false);

    return true;
}

Qt::ItemFlags VPageWidgetModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    Qt::ItemFlags flags = Qt::ItemIsSelectable;

    PageItem *item = static_cast<PageItem *>(index.internalPointer());
    if (item->pageWidgetItem()->isCheckable())
        flags |= Qt::ItemIsUserCheckable;
    if (item->pageWidgetItem()->isEnabled()) {
        flags |= Qt::ItemIsEnabled;
    }

    return flags;
}

QModelIndex VPageWidgetModel::index(int row, int column, const QModelIndex &parent) const
{
    PageItem *parentItem;

    if (parent.isValid())
        parentItem = static_cast<PageItem *>(parent.internalPointer());
    else
        parentItem = d_func()->rootItem;

    PageItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex VPageWidgetModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    PageItem *item = static_cast<PageItem *>(index.internalPointer());
    PageItem *parentItem = item->parent();

    if (parentItem == d_func()->rootItem)
        return QModelIndex();
    else
        return createIndex(parentItem->row(), 0, parentItem);
}

int VPageWidgetModel::rowCount(const QModelIndex &parent) const
{
    PageItem *parentItem;

    if (!parent.isValid())
        parentItem = d_func()->rootItem;
    else
        parentItem = static_cast<PageItem *>(parent.internalPointer());

    return parentItem->childCount();
}

VPageWidgetItem *VPageWidgetModel::addPage(QWidget *widget, const QString &name)
{
    VPageWidgetItem *item = new VPageWidgetItem(widget, name);

    addPage(item);

    return item;
}

void VPageWidgetModel::addPage(VPageWidgetItem *item)
{
    emit layoutAboutToBeChanged();

    Q_D(VPageWidgetModel);
    connect(item, SIGNAL(changed()), this, SLOT(_q_itemChanged()));
    connect(item, SIGNAL(toggled(bool)), this, SLOT(_q_itemToggled(bool)));

    // The row to be inserted
    int row = d->rootItem->childCount();

    beginInsertRows(QModelIndex(), row, row);

    PageItem *pageItem = new PageItem(item, d->rootItem);
    d->rootItem->appendChild(pageItem);

    endInsertRows();

    emit layoutChanged();
}

VPageWidgetItem *VPageWidgetModel::insertPage(VPageWidgetItem *before, QWidget *widget, const QString &name)
{
    VPageWidgetItem *item = new VPageWidgetItem(widget, name);

    insertPage(before, item);

    return item;
}

void VPageWidgetModel::insertPage(VPageWidgetItem *before, VPageWidgetItem *item)
{
    PageItem *beforePageItem = d_func()->rootItem->findChild(before);
    if (!beforePageItem) {
        qDebug("Invalid VPageWidgetItem passed!");
        return;
    }

    emit layoutAboutToBeChanged();

    connect(item, SIGNAL(changed()), this, SLOT(_q_itemChanged()));
    connect(item, SIGNAL(toggled(bool)), this, SLOT(_q_itemToggled(bool)));

    PageItem *parent = beforePageItem->parent();

    // The row to be inserted
    int row = beforePageItem->row();

    QModelIndex index;
    if (parent != d_func()->rootItem)
        index = createIndex(parent->row(), 0, parent);

    beginInsertRows(index, row, row);

    PageItem *newPageItem = new PageItem(item, parent);
    parent->insertChild(row, newPageItem);

    endInsertRows();

    emit layoutChanged();
}

VPageWidgetItem *VPageWidgetModel::addSubPage(VPageWidgetItem *parent, QWidget *widget, const QString &name)
{
    VPageWidgetItem *item = new VPageWidgetItem(widget, name);

    addSubPage(parent, item);

    return item;
}

void VPageWidgetModel::addSubPage(VPageWidgetItem *parent, VPageWidgetItem *item)
{
    PageItem *parentPageItem = d_func()->rootItem->findChild(parent);
    if (!parentPageItem) {
        qDebug("Invalid VPageWidgetItem passed!");
        return;
    }

    emit layoutAboutToBeChanged();

    connect(item, SIGNAL(changed()), this, SLOT(_q_itemChanged()));
    connect(item, SIGNAL(toggled(bool)), this, SLOT(_q_itemToggled(bool)));

    // The row to be inserted
    int row = parentPageItem->childCount();

    QModelIndex index;
    if (parentPageItem != d_func()->rootItem) {
        index = createIndex(parentPageItem->row(), 0, parentPageItem);
    }

    beginInsertRows(index, row, row);

    PageItem *newPageItem = new PageItem(item, parentPageItem);
    parentPageItem->appendChild(newPageItem);

    endInsertRows();

    emit layoutChanged();
}

void VPageWidgetModel::removePage(VPageWidgetItem *item)
{
    if (!item)
        return;

    Q_D(VPageWidgetModel);

    PageItem *pageItem = d->rootItem->findChild(item);
    if (!pageItem) {
        qDebug("Invalid VPageWidgetItem passed!");
        return;
    }

    emit layoutAboutToBeChanged();

    disconnect(item, SIGNAL(changed()), this, SLOT(_q_itemChanged()));
    disconnect(item, SIGNAL(toggled(bool)), this, SLOT(_q_itemToggled(bool)));

    PageItem *parentPageItem = pageItem->parent();
    int row = parentPageItem->row();

    QModelIndex index;
    if (parentPageItem != d->rootItem)
        index = createIndex(row, 0, parentPageItem);

    beginRemoveRows(index, pageItem->row(), pageItem->row());

    parentPageItem->removeChild(pageItem->row());
    delete pageItem;

    endRemoveRows();

    emit layoutChanged();
}

VPageWidgetItem *VPageWidgetModel::item(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    PageItem *item = static_cast<PageItem *>(index.internalPointer());
    if (!item)
        return 0;

    return item->pageWidgetItem();
}

QModelIndex VPageWidgetModel::index(const VPageWidgetItem *item) const
{
    if (!item)
        return QModelIndex();

    const PageItem *pageItem = d_func()->rootItem->findChild(item);
    if (!pageItem)
        return QModelIndex();

    return createIndex(pageItem->row(), 0, (void *)pageItem);
}

#include "vpagewidgetmodel.moc"
