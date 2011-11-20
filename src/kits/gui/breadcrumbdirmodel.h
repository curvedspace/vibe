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

#ifndef BREADCRUMBDIRMODEL_H
#define BREADCRUMBDIRMODEL_H

#include <QObject>
#include <QMap>

#include "abstractbreadcrumbmodel.h"

class QDateTime;
class QDirModel;
class QDir;
class QFileSystemWatcher;
class QMenu;
class QStringList;

namespace Qube
{
    namespace Gui
    {
        class BreadCrumbDirModelMenu;

        class BreadCrumbDirModelMenuManager : public QObject
        {
            Q_OBJECT

        public:
            BreadCrumbDirModelMenuManager( QObject * parent = 0 );
            ~BreadCrumbDirModelMenuManager();

            BreadCrumbDirModelMenu * menu( const QString & path ) const;
            void registerPath( const QString & path );
            bool isRegisteredPath( const QString & path ) const;
            QMenu * mainMenu() const;
            QMenu * driveMenu() const;

            void clearAll();
            void clean();

        protected slots:
            void handleDirectoryChanged( const QString & path );

        private:
            QMap< QString, BreadCrumbDirModelMenu * > m_menuMapping;
            QMenu * m_mainMenu;
            QMenu * m_driveMenu;
            QFileSystemWatcher * m_watcher;
        };

        class BreadCrumbDirModel : public AbstractBreadCrumbModel
        {
        public:
            BreadCrumbDirModel();
            virtual ~BreadCrumbDirModel();

            QString defaultPath() const;
            QString cleanPath( const QString & ) const;
            bool isValid( const QString & path ) const;
            BreadCrumbModelNodeList splitPath( const QString & path ) const;

            QIcon icon( const BreadCrumbModelNode & ) const;
            QString label( const BreadCrumbModelNode & ) const;

            QMimeData * mimeData( const BreadCrumbModelNode & ) const;

            QMenu * buildMenu( const BreadCrumbModelNode & );

            inline bool supportsMenuNavigation() const {
                return true;
            }

        protected:
            void makeCurrent() const;

        private:
            static BreadCrumbDirModelMenuManager * m_menuManager;
            static QDirModel * m_dirModel;
        };
    }
}

#endif // QIRBREADCRUMBDIRMODEL_H
