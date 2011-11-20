/**********************************************************************
**
** This file is part of on Toolkit.
**
** Copyright (C) 2009-2020 Dzimi Mve Alexandre <dzimiwine@gmail.com>
**
** Contact: dzimiwine@gmail.com
**
** on is a free toolkit developed in Qt by Dzimi Mve A.; you can redistribute
** sources and libraries of this library and/or modify them under the terms of
** the GNU Library General Public License version 3.0 as published by the
** Free Software Foundation and appearing in the file LICENSE.txt included in
** the packaging of this file.
** Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** This SDK is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
**
**********************************************************************/

#ifndef ABSTRACTBREADCRUMBMODEL_H
#define ABSTRACTBREADCRUMBMODEL_H

#include <QtCore/QString>

class QAbstractItemModel;
class QIcon;
class QMenu;
class QMimeData;
template< typename T >
class QList;

namespace Qube
{
    namespace Gui
    {
        class AbstractBreadCrumbModel;

        class BreadCrumbModelNode
        {
        public:
            enum Type {Global, Root, Container, Leaf, Unknown};

            BreadCrumbModelNode() : m_path(QString()),m_label(QString()),m_type(Unknown)
            { }
            explicit BreadCrumbModelNode(const QString & path, Type type, const AbstractBreadCrumbModel * model = 0);
            inline BreadCrumbModelNode( const BreadCrumbModelNode & other ) : m_path(other.m_path), m_label(other.m_label),
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

            BreadCrumbModelNode & operator = ( const BreadCrumbModelNode & other );

        private:
            QString m_path;
            QString m_label;
            Type m_type;
        };
        typedef QList< BreadCrumbModelNode > BreadCrumbModelNodeList;

        class AbstractBreadCrumbModel
        {
        public:
            enum Filter {
                Containers = 1,
                AllNodes = 2,
                Hidden = 4
            };
            Q_DECLARE_FLAGS(Filters,Filter)

            AbstractBreadCrumbModel();
            virtual ~AbstractBreadCrumbModel();

            inline QAbstractItemModel * itemModel() const {
                return m_model;
            }

            inline Filters filter() const {
                return m_filter;
            }
            void setFilter( Filters filter );

            virtual QString defaultPath() const = 0;
            virtual QString cleanPath( const QString & ) const = 0;

            virtual bool isValid( const QString & path ) const = 0;
            virtual BreadCrumbModelNodeList splitPath( const QString & path ) const = 0;

            virtual QIcon icon( const BreadCrumbModelNode & ) const = 0;
            virtual QString label( const BreadCrumbModelNode & ) const = 0;

            virtual QMimeData * mimeData( const BreadCrumbModelNode & ) const = 0;

            virtual QMenu * buildMenu(const BreadCrumbModelNode &) = 0;

            virtual bool supportsMenuNavigation() const = 0;

        protected:
            void setItemModel( QAbstractItemModel * );

        private:
            QAbstractItemModel * m_model;
            Filters m_filter;
        };
        Q_DECLARE_OPERATORS_FOR_FLAGS(AbstractBreadCrumbModel::Filters)
    }
}

#endif // ABSTRACTBREADCRUMBMODEL_H
