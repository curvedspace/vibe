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

/*
 * Partially based on QxtCrumbView from the LibQxt project.
 * Copyright (c) 2006 - 2011, the LibQxt project.
 * See the Qxt AUTHORS file for a list of authors and copyright holders.
 */

#include <QApplication>
#include <QBoxLayout>
#include <QFontMetrics>
#include <QListView>
#include <QStylePainter>
#include <QToolButton>
#include <QDebug>

#include "vbreadcrumbview.h"
#include "vbreadcrumbview_p.h"

VBreadcrumbViewDelegate::VBreadcrumbViewDelegate(QObject *parent) :
    QAbstractItemDelegate(parent)
{
}

void VBreadcrumbViewDelegate::paint(QPainter *painter,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
    const QAbstractItemModel *model = index.model();
    const bool isChild = model->hasChildren(index);
    const bool isLTR = option.direction == Qt::LeftToRight;

    if (!model->hasChildren(index))
        return;

    painter->save();

    int arrow = 8;
    int iconSize = QApplication::style()->pixelMetric(QStyle::PM_ToolBarIconSize, &option);
    int pad = (option.rect.height() - iconSize) / 2;
    int kLateralMargin = 4;
    int margin = kLateralMargin;

    // Draw icon
    QIcon icon = QIcon(model->data(index, Qt::DecorationRole).value<QIcon>());
    QPixmap pm = icon.pixmap(iconSize, iconSize);
    QPoint point(isLTR ? option.rect.left() + margin
                 : option.rect.right() - margin - iconSize,
                 option.rect.top() + pad);
    painter->drawPixmap(point, pm);

    QRect rectText(0, 0, 0, 0);
    if (index.parent().isValid()) {
        // Increase margin
        margin += kLateralMargin;

        // Draw text for every child item
        rectText = QRect(isLTR
                         ? option.rect.left() + iconSize + margin
                         : 0,
                         option.rect.top(), option.rect.width() - iconSize - margin, option.rect.height());
        painter->drawText(rectText, Qt::AlignLeft | Qt::AlignVCenter,
                          option.fontMetrics.elidedText(model->data(index).toString(), Qt::ElideRight, rectText.width()));
    }

    painter->restore();
}

QSize VBreadcrumbViewDelegate::sizeHint(const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const
{
    QStyleOptionViewItemV4 opt = option;

    QWidget *widget = 0;
    if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
        widget = (QWidget *)v3->widget;

    int w = 0, h = 0;
    int kLateralMargin = 4;
    int iconSize = QApplication::style()->pixelMetric(QStyle::PM_ToolBarIconSize, &option);
    const QAbstractItemModel *model = index.model();

    w = h = iconSize;

    if (index.parent().isValid()) {
        QFont font;
        QFontMetrics fm(font);

        QSize textSize = fm.size(Qt::TextShowMnemonic, model->data(index).toString());
        textSize.setWidth(textSize.width() + fm.width(QLatin1Char(' ')) * 2);
        w += 4 + textSize.width() + kLateralMargin;
        if (textSize.height() > h)
            h = textSize.height();
    }

    return QApplication::style()->sizeFromContents(QStyle::CT_ToolButton,
                                                   &opt, QSize(w, h), widget);
}

class VBreadcrumbViewButton : public QAbstractButton
{
public:
    VBreadcrumbViewButton(const QModelIndex &idx, VBreadcrumbView *parent) : QAbstractButton(parent), index(idx) {
        // initializers only
    }

    QModelIndex index;

    QSize sizeHint() const {
        int border = style()->pixelMetric(QStyle::PM_DefaultFrameWidth) * 2;

        if (index.isValid()) {
            VBreadcrumbView *view = qobject_cast<VBreadcrumbView *>(parent());
            if (view) {
                QStyleOptionViewItemV4 option;
                option.initFrom(this);

                return view->itemDelegate()->sizeHint(option, index) + QSize(border, border);
            }
        }

        return QSize(border, border);
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
            VBreadcrumbView *view = qobject_cast<VBreadcrumbView *>(parent());
            if (view) {
                QAbstractItemDelegate *delegate = qobject_cast<QAbstractItemDelegate *>(view->itemDelegate());
                QStyleOptionViewItem itemOption;
                itemOption.initFrom(this);
                itemOption.rect = option.rect;
                delegate->paint(&painter, itemOption, index);
            }
        }
        return;

        int arrow = 8;
        int pad = (height() - (2 * border) - arrow) / 2;
        QStyleOption arrowOption;
        arrowOption.initFrom(this);
        arrowOption.rect = QRect(width() - border - arrow, pad, arrow, arrow);
        painter.drawPrimitive(QStyle::PE_IndicatorArrowRight, arrowOption);
    }
};

VBreadcrumbViewPrivate::VBreadcrumbViewPrivate(VBreadcrumbView *self) :
    q_ptr(self)
{
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

VBreadcrumbView::VBreadcrumbView(QWidget *parent) :
    QAbstractItemView(parent),
    d_ptr(new VBreadcrumbViewPrivate(this))
{
    Q_D(VBreadcrumbView);

    setAutoFillBackground(false);
    setBackgroundRole(QPalette::Window);
    viewport()->setAutoFillBackground(false);
    viewport()->setBackgroundRole(QPalette::Window);
    setFrameStyle(QFrame::NoFrame);
    d->buttonLayout = new QHBoxLayout(this);
    d->buttonLayout->setContentsMargins(0, 0, 0, 0);
    d->buttonLayout->setSpacing(0);
    d->buttonLayout->addStretch(1);
    d->addCrumb(QModelIndex());
}

QSize VBreadcrumbView::sizeHint() const
{
    int border = style()->pixelMetric(QStyle::PM_DefaultFrameWidth) * 2;
    return QSize(border, border);
}

QSize VBreadcrumbView::minimumSizeHint() const
{
    return sizeHint();
}

QSize VBreadcrumbView::maximumSizeHint() const
{
    return minimumSizeHint();
}

void VBreadcrumbView::setModel(QAbstractItemModel *model)
{
    Q_D(VBreadcrumbView);

    QAbstractItemView::setModel(model);
    setItemDelegate(new VBreadcrumbViewDelegate(this));
}

void VBreadcrumbView::reset()
{
    Q_D(VBreadcrumbView);

    QAbstractItemView::reset();
    qDeleteAll(d->buttons);
    d->crumbs.clear();
    d->buttons.clear();
    d->addCrumb(QModelIndex());
}

int VBreadcrumbView::horizontalOffset() const
{
    Q_D(const VBreadcrumbView);
    return 0;
}

int VBreadcrumbView::verticalOffset() const
{
    Q_D(const VBreadcrumbView);
    return 0;
}

QModelIndex VBreadcrumbView::indexAt(const QPoint &point) const
{
    return QModelIndex();
}

bool VBreadcrumbView::isIndexHidden(const QModelIndex &index) const
{
    Q_D(const VBreadcrumbView);
    return false;
}

QModelIndex VBreadcrumbView::moveCursor(CursorAction action, Qt::KeyboardModifiers mods)
{
    return QModelIndex();
}

void VBreadcrumbView::scrollTo(const QModelIndex &index, ScrollHint hint)
{
}

void VBreadcrumbView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags flags)
{
}

QRect VBreadcrumbView::visualRect(const QModelIndex &index) const
{
    Q_D(const VBreadcrumbView);

    foreach(VBreadcrumbViewButton * b, d->buttons) {
        if (b->index == index)
            return b->geometry();
    }
}

QRegion VBreadcrumbView::visualRegionForSelection(const QItemSelection &selection) const
{
    return QRegion();
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
    if (index == rootIndex()) {
        // do nothing but reload the view
    } else if (index.parent() != rootIndex()) {
        foreach(VBreadcrumbViewButton * b, d->buttons)
        b->deleteLater();
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
    setRootIndex(index);
}

void VBreadcrumbView::back()
{
    Q_D(VBreadcrumbView);

    if (d->crumbs.count() <= 1)
        return;
    d->buttons.last()->deleteLater();
    d->buttons.removeLast();
    d->crumbs.removeLast();
    setRootIndex(d->crumbs.last());
}

void VBreadcrumbView::next()
{
}

#include "moc_vbreadcrumbview.cpp"
