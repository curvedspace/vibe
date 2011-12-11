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

#include "vabstractbreadcrumbmodel.h"

/*
 * VBreadCrumbModelNode
 */

VBreadCrumbModelNode::VBreadCrumbModelNode(const QString &path, Type type,  const VAbstractBreadCrumbModel *model) :
    m_path(path),
    m_type(type)
{
    if (type != Unknown && model)
        m_label = model->label(*this);
}

VBreadCrumbModelNode &VBreadCrumbModelNode::operator =(const VBreadCrumbModelNode &other)
{
    m_path = other.m_path;
    m_type = other.m_type;
    m_label = other.m_label;

    return *this;
}

/*
 * VAbstractBreadCrumbModel
 */

VAbstractBreadCrumbModel::VAbstractBreadCrumbModel() :
    m_model(0)
{
}

VAbstractBreadCrumbModel::~VAbstractBreadCrumbModel()
{
}

void VAbstractBreadCrumbModel::setFilter(Filters filter)
{
    m_filter = filter == 0 ? Containers : filter;
}

void VAbstractBreadCrumbModel::setItemModel(QAbstractItemModel *model)
{
    if (!model || m_model == model)
        return;
    m_model = model;
}
