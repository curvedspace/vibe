/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (C) 2009 Dzimi Mve Alexandre
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *    Dzimi Mve Alexandre <dzimiwine@gmail.com>
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef VABSTRACTBREADCRUMBMODEL_H
#define VABSTRACTBREADCRUMBMODEL_H

#include <QtCore/QString>

#include <VibeCore/VGlobal>

class QAbstractItemModel;
class QIcon;
class QMenu;
class QMimeData;
template< typename T >
class QList;

class VAbstractBreadCrumbModel;

class VIBE_EXPORT VBreadCrumbModelNode
{
public:
    enum Type {
        Global,
        Root,
        Container,
        Leaf,
        Unknown
    };

    VBreadCrumbModelNode() :
        m_path(QString()),
        m_label(QString()),
        m_type(Unknown)
    { }
    explicit VBreadCrumbModelNode(const QString &path, Type type, const VAbstractBreadCrumbModel *model = 0);
    inline VBreadCrumbModelNode(const VBreadCrumbModelNode &other) :
        m_path(other.m_path),
        m_label(other.m_label),
        m_type(other.m_type)
    { }

    inline Type type() const {
        return m_type;
    }
    inline QString path() const {
        return m_path;
    }
    inline QString label() const {
        return m_label;
    }

    VBreadCrumbModelNode &operator = (const VBreadCrumbModelNode &other);

private:
    QString m_path;
    QString m_label;
    Type m_type;
};

typedef QList<VBreadCrumbModelNode> VBreadCrumbModelNodeList;

class VIBE_EXPORT VAbstractBreadCrumbModel
{
public:
    enum Filter {
        Containers = 1,
        AllNodes = 2,
        Hidden = 4
    };
    Q_DECLARE_FLAGS(Filters, Filter)

    VAbstractBreadCrumbModel();
    virtual ~VAbstractBreadCrumbModel();

    inline QAbstractItemModel *itemModel() const {
        return m_model;
    }

    inline Filters filter() const {
        return m_filter;
    }
    void setFilter(Filters filter);

    virtual QString defaultPath() const = 0;
    virtual QString cleanPath(const QString &) const = 0;

    virtual bool isValid(const QString &path) const = 0;
    virtual VBreadCrumbModelNodeList splitPath(const QString &path) const = 0;

    virtual QIcon icon(const VBreadCrumbModelNode &) const = 0;
    virtual QString label(const VBreadCrumbModelNode &) const = 0;

    virtual QMimeData *mimeData(const VBreadCrumbModelNode &) const = 0;

    virtual QMenu *buildMenu(const VBreadCrumbModelNode &) = 0;

    virtual bool supportsMenuNavigation() const = 0;

protected:
    void setItemModel(QAbstractItemModel *);

private:
    QAbstractItemModel *m_model;
    Filters m_filter;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(VAbstractBreadCrumbModel::Filters)

#endif // VABSTRACTBREADCRUMBMODEL_H
