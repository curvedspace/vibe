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
