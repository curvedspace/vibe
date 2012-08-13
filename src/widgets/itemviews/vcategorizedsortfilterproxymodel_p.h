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

#ifndef VCATEGORIZEDSORTFILTERPROXYMODEL_P_H
#define VCATEGORIZEDSORTFILTERPROXYMODEL_P_H

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

class VCategorizedSortFilterProxyModel;

class VCategorizedSortFilterProxyModel::Private
{
public:
    Private()
        : sortColumn(0)
        , sortOrder(Qt::AscendingOrder)
        , categorizedModel(false)
        , sortCategoriesByNaturalComparison(true) {
    }

    ~Private() {
    }

    int sortColumn;
    Qt::SortOrder sortOrder;
    bool categorizedModel;
    bool sortCategoriesByNaturalComparison;
};

#endif // VCATEGORIZEDSORTFILTERPROXYMODEL_P_H
