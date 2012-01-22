/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (C) 2009 Dzimi Mve Alexandre
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Dzimi Mve Alexandre <dzimiwine@gmail.com>
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef VBREADCRUMBBAR_P_H
#define VBREADCRUMBBAR_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the on API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

class VBreadCrumbBar;
class VBreadCrumbComboBox;
class VAbstractBreadCrumbModel;

class VIBE_EXPORT VBreadCrumbBarPrivate
{
    Q_DECLARE_PUBLIC(VBreadCrumbBar);
public:
    VBreadCrumbBarPrivate(VBreadCrumbBar *q_ptr);
    ~VBreadCrumbBarPrivate();

    bool flat;
    bool editable;
    VBreadCrumbComboBox *comboBox;
    VAbstractBreadCrumbModel *model;

private:
    VBreadCrumbBar *q_ptr;
};

#endif // VBREADCRUMBBAR_P_H
