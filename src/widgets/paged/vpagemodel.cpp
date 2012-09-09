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

#include "vpagemodel.h"
#include "vpagemodel_p.h"

/*
 * VPageModelPrivate
 */

VPageModelPrivate::~VPageModelPrivate()
{
}

/*
 * VPageModel
 */

VPageModel::VPageModel(QObject *parent) :
    QAbstractItemModel(parent),
    d_ptr(0)
{
}

VPageModel::VPageModel(VPageModelPrivate &dd, QObject *parent) :
    QAbstractItemModel(parent),
    d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

VPageModel::~VPageModel()
{
    delete d_ptr;
}

#include "moc_vpagemodel.cpp"
