/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 * Copyright (c) 2007-2009 Rafael Fernández López
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Rafael Fernández López <ereslibre@kde.org>
 *
 * $BEGIN_LICENSE:LGPL$
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
 *
 * $END_LICENSE$
 ***************************************************************************/

#include <QPainter>
#include <QStyleOption>
#include <QApplication>

#include "vcategorydrawer.h"
#include "vcategorizedview.h"
#include "vcategorizedsortfilterproxymodel.h"

#define HORIZONTAL_HINT 3

class VCategoryDrawer::Private
{
public:
    Private(VCategorizedView *view)
        : view(view)
        , leftMargin(0)
        , rightMargin(0) {
    }

    ~Private() {
    }

    VCategorizedView *view;
    int leftMargin;
    int rightMargin;
};

VCategoryDrawer::VCategoryDrawer(VCategorizedView *view)
    : QObject(view)
    , d(new Private(view))
{
    setLeftMargin(2);
    setRightMargin(2);
}

VCategoryDrawer::~VCategoryDrawer()
{
    delete d;
}

void VCategoryDrawer::drawCategory(const QModelIndex &index,
                                   int /*sortRole*/,
                                   const QStyleOption &option,
                                   QPainter *painter) const
{
    painter->setRenderHint(QPainter::Antialiasing);

    const QString category = index.model()->data(index, VCategorizedSortFilterProxyModel::CategoryDisplayRole).toString();
    const QRect optRect = option.rect;
    QFont font(QApplication::font());
    font.setBold(true);
    const QFontMetrics fontMetrics = QFontMetrics(font);

    QColor outlineColor = option.palette.text().color();
    outlineColor.setAlphaF(0.35);

    //BEGIN: top left corner
    {
        painter->save();
        painter->setPen(outlineColor);
        const QPointF topLeft(optRect.topLeft());
        QRectF arc(topLeft, QSizeF(4, 4));
        arc.translate(0.5, 0.5);
        painter->drawArc(arc, 1440, 1440);
        painter->restore();
    }
    //END: top left corner

    //BEGIN: left vertical line
    {
        QPoint start(optRect.topLeft());
        start.ry() += 3;
        QPoint verticalGradBottom(optRect.topLeft());
        verticalGradBottom.ry() += fontMetrics.height() + 5;
        QLinearGradient gradient(start, verticalGradBottom);
        gradient.setColorAt(0, outlineColor);
        gradient.setColorAt(1, Qt::transparent);
        painter->fillRect(QRect(start, QSize(1, fontMetrics.height() + 5)), gradient);
    }
    //END: left vertical line

    //BEGIN: horizontal line
    {
        QPoint start(optRect.topLeft());
        start.rx() += 3;
        QPoint horizontalGradTop(optRect.topLeft());
        horizontalGradTop.rx() += optRect.width() - 6;
        painter->fillRect(QRect(start, QSize(optRect.width() - 6, 1)), outlineColor);
    }
    //END: horizontal line

    //BEGIN: top right corner
    {
        painter->save();
        painter->setPen(outlineColor);
        QPointF topRight(optRect.topRight());
        topRight.rx() -= 4;
        QRectF arc(topRight, QSizeF(4, 4));
        arc.translate(0.5, 0.5);
        painter->drawArc(arc, 0, 1440);
        painter->restore();
    }
    //END: top right corner

    //BEGIN: right vertical line
    {
        QPoint start(optRect.topRight());
        start.ry() += 3;
        QPoint verticalGradBottom(optRect.topRight());
        verticalGradBottom.ry() += fontMetrics.height() + 5;
        QLinearGradient gradient(start, verticalGradBottom);
        gradient.setColorAt(0, outlineColor);
        gradient.setColorAt(1, Qt::transparent);
        painter->fillRect(QRect(start, QSize(1, fontMetrics.height() + 5)), gradient);
    }
    //END: right vertical line

    //BEGIN: text
    {
        QRect textRect(option.rect);
        textRect.setTop(textRect.top() + 7);
        textRect.setLeft(textRect.left() + 7);
        textRect.setHeight(fontMetrics.height());
        textRect.setRight(textRect.right() - 7);

        painter->save();
        painter->setFont(font);
        QColor penColor(option.palette.text().color());
        penColor.setAlphaF(0.6);
        painter->setPen(penColor);
        painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, category);
        painter->restore();
    }
    //END: text
}

int VCategoryDrawer::categoryHeight(const QModelIndex &index, const QStyleOption &option) const
{
    Q_UNUSED(index);
    Q_UNUSED(option)

    QFont font(QApplication::font());
    font.setBold(true);
    QFontMetrics fontMetrics(font);

    const int height = fontMetrics.height() + 1 /* 1 pixel-width gradient */
                       + 11 /* top and bottom separation */;
    return height;
}

int VCategoryDrawer::leftMargin() const
{
    return d->leftMargin;
}

void VCategoryDrawer::setLeftMargin(int leftMargin)
{
    d->leftMargin = leftMargin;
}

int VCategoryDrawer::rightMargin() const
{
    return d->rightMargin;
}

void VCategoryDrawer::setRightMargin(int rightMargin)
{
    d->rightMargin = rightMargin;
}

VCategorizedView *VCategoryDrawer::view() const
{
    return d->view;
}

VCategoryDrawer &VCategoryDrawer::operator=(const VCategoryDrawer &cd)
{
    d->leftMargin = cd.d->leftMargin;
    d->rightMargin = cd.d->rightMargin;
    return *this;
}

void VCategoryDrawer::mouseButtonPressed(const QModelIndex &, const QRect &, QMouseEvent *event)
{
    event->ignore();
}

void VCategoryDrawer::mouseButtonReleased(const QModelIndex &, const QRect &, QMouseEvent *event)
{
    event->ignore();
}

void VCategoryDrawer::mouseMoved(const QModelIndex &, const QRect &, QMouseEvent *event)
{
    event->ignore();
}

void VCategoryDrawer::mouseButtonDoubleClicked(const QModelIndex &, const QRect &, QMouseEvent *event)
{
    event->ignore();
}

void VCategoryDrawer::mouseLeft(const QModelIndex &, const QRect &)
{
}

#include "moc_vcategorydrawer.cpp"
