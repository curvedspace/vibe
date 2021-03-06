/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
 * Copyright (C) 2006 Tobias Koenig
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Tobias Koenig <tokoe@kde.org>
 *
 * $BEGIN_LICENSE:LGPL2$
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
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
