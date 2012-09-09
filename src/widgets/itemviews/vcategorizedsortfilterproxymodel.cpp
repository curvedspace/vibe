/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 * Copyright (c) 2007 Rafael Fernández López
 * Copyright (c) John Tapsell
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Rafael Fernández López <ereslibre@kde.org>
 *    John Tapsell <tapsell@kde.org>
 *
 * $BEGIN_LICENSE:LGPL$
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
 *
 * $END_LICENSE$
 ***************************************************************************/

#include <limits.h>

#include <QItemSelection>
#include <QStringList>
#include <QSize>

#include <VibeCore/VStringHandler>

#include "vcategorizedsortfilterproxymodel.h"
#include "vcategorizedsortfilterproxymodel_p.h"

VCategorizedSortFilterProxyModel::VCategorizedSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , d(new Private())
{
}

VCategorizedSortFilterProxyModel::~VCategorizedSortFilterProxyModel()
{
    delete d;
}

void VCategorizedSortFilterProxyModel::sort(int column, Qt::SortOrder order)
{
    d->sortColumn = column;
    d->sortOrder = order;

    QSortFilterProxyModel::sort(column, order);
}

bool VCategorizedSortFilterProxyModel::isCategorizedModel() const
{
    return d->categorizedModel;
}

void VCategorizedSortFilterProxyModel::setCategorizedModel(bool categorizedModel)
{
    if (categorizedModel == d->categorizedModel) {
        return;
    }

    d->categorizedModel = categorizedModel;

    invalidate();
}

int VCategorizedSortFilterProxyModel::sortColumn() const
{
    return d->sortColumn;
}

Qt::SortOrder VCategorizedSortFilterProxyModel::sortOrder() const
{
    return d->sortOrder;
}

void VCategorizedSortFilterProxyModel::setSortCategoriesByNaturalComparison(bool sortCategoriesByNaturalComparison)
{
    if (sortCategoriesByNaturalComparison == d->sortCategoriesByNaturalComparison) {
        return;
    }

    d->sortCategoriesByNaturalComparison = sortCategoriesByNaturalComparison;

    invalidate();
}

bool VCategorizedSortFilterProxyModel::sortCategoriesByNaturalComparison() const
{
    return d->sortCategoriesByNaturalComparison;
}

bool VCategorizedSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (d->categorizedModel) {
        int compare = compareCategories(left, right);

        if (compare > 0)
            // left is greater than right
            return false;
        else if (compare < 0)
            // left is less than right
            return true;
    }

    return subSortLessThan(left, right);
}

bool VCategorizedSortFilterProxyModel::subSortLessThan(const QModelIndex &left, const QModelIndex &right) const
{
    return QSortFilterProxyModel::lessThan(left, right);
}

int VCategorizedSortFilterProxyModel::compareCategories(const QModelIndex &left, const QModelIndex &right) const
{
    QVariant l = (left.model() ? left.model()->data(left, CategorySortRole) : QVariant());
    QVariant r = (right.model() ? right.model()->data(right, CategorySortRole) : QVariant());

    Q_ASSERT(l.isValid());
    Q_ASSERT(r.isValid());
    Q_ASSERT(l.type() == r.type());

    if (l.type() == QVariant::String) {
        QString lstr = l.toString();
        QString rstr = r.toString();

        if (d->sortCategoriesByNaturalComparison) {
            return VStringHandler::naturalCompare(lstr, rstr);
        } else {
            if (lstr < rstr) {
                return -1;
            }

            if (lstr > rstr) {
                return 1;
            }

            return 0;
        }
    }

    qlonglong lint = l.toLongLong();
    qlonglong rint = r.toLongLong();

    if (lint < rint)
        return -1;

    if (lint > rint)
        return 1;

    return 0;
}
