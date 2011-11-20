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

#include <QAction>
#include <QMenu>
#include <QDir>
#include <QDirModel>
#include <QModelIndex>
#include <QLibrary>
#include <QFileIconProvider>
#include <QApplication>
#include <QFileInfo>
#include <QFile>
#include <QUrl>
#include <QMimeData>
#include <QFileSystemWatcher>

#include "breadcrumbdirmodel.h"
#include "dir_helper.h"

namespace Qube
{
    namespace Gui
    {
        QDirModel * BreadCrumbDirModel::m_dirModel = 0;
        BreadCrumbDirModelMenuManager * BreadCrumbDirModel::m_menuManager = 0;

        class BreadCrumbDirModelMenuAction : public QAction
        {
        public:
            BreadCrumbDirModelMenuAction(const QIcon & icon, const QString & fileName, const QString & path, QObject * parent) :
                QAction(icon,fileName,parent) {
                setData(path);
            }
        };

////////////////////////////
//BreadCrumbDirModelMenu
///////////////////////////
        class BreadCrumbDirModelMenu : public QMenu
        {
        public:
            BreadCrumbDirModelMenu() : QMenu(0), m_need(true) { }

            inline AbstractBreadCrumbModel::Filters filter() const {
                return m_filter;
            }
            void setFilter( AbstractBreadCrumbModel::Filters filter );
            inline bool needBeRebuilt() const {
                return m_need;
            }
            inline void setNeedBeRebuilt( bool need ) {
                m_need = need;
            }

            bool m_need;

        private:
            AbstractBreadCrumbModel::Filters m_filter;
        };

        void BreadCrumbDirModelMenu::setFilter( AbstractBreadCrumbModel::Filters filter )
        {
            if ( m_filter == filter )
                return;
            m_filter = filter;
            m_need = true;
        }

///////////////////////////////////
//BreadCrumbDirModelMenuManager
///////////////////////////////////
        BreadCrumbDirModelMenuManager::BreadCrumbDirModelMenuManager( QObject * parent ) : QObject(parent)
        {
            m_watcher = new QFileSystemWatcher(this);
            m_mainMenu = new QMenu;
            m_driveMenu = new QMenu;
            connect(m_watcher,SIGNAL(directoryChanged( QString )),this,SLOT(handleDirectoryChanged( QString )));
        }
        BreadCrumbDirModelMenuManager::~BreadCrumbDirModelMenuManager()
        {
            delete m_mainMenu;
            delete m_driveMenu;
            foreach (QString path, m_menuMapping.keys()) {
                delete m_menuMapping.take(path);
            }
        }
        QMenu * BreadCrumbDirModelMenuManager::mainMenu() const
        {
            return m_mainMenu;
        }
        QMenu * BreadCrumbDirModelMenuManager::driveMenu() const
        {
            return m_driveMenu;
        }
        void BreadCrumbDirModelMenuManager::registerPath(const QString & path )
        {
            QString tempPath = path;

            if ( !tempPath.endsWith(QDir::separator()) )
                tempPath += QDir::separator();
            if ( m_watcher->directories().contains(tempPath) )
                return;
            m_watcher->addPath(tempPath);
            m_menuMapping[tempPath] = new BreadCrumbDirModelMenu;
        }
        bool BreadCrumbDirModelMenuManager::isRegisteredPath( const QString & path ) const
        {
            QString tempPath = path;

            if ( !tempPath.endsWith(QDir::separator()) )
                tempPath += QDir::separator();
            return m_watcher->directories().contains(tempPath);
        }

        BreadCrumbDirModelMenu * BreadCrumbDirModelMenuManager::menu( const QString & path) const
        {
            QString tempPath = path;

            if ( !tempPath.endsWith(QDir::separator()) )
                tempPath += QDir::separator();
            if ( !isRegisteredPath(tempPath) )
                return 0;
            return m_menuMapping[tempPath];
        }
        void BreadCrumbDirModelMenuManager::clean()
        {
            foreach (QString path, m_watcher->directories()) {
                QDir dir(path);

                if ( !dir.exists() ) {
                    m_watcher->removePath(path);
                    delete m_menuMapping.take(path);
                }
            }
        }
        void BreadCrumbDirModelMenuManager::handleDirectoryChanged( const QString & path )
        {
            m_menuMapping[path]->setNeedBeRebuilt(true);
        }


/////////////////////////
//BreadCrumbDirModel
////////////////////////
        BreadCrumbDirModel::BreadCrumbDirModel()
        {
            if ( !m_dirModel )
                m_dirModel = new QDirModel(qApp);
            if ( !m_menuManager )
                m_menuManager = new BreadCrumbDirModelMenuManager(qApp);
            setItemModel(m_dirModel);
            setFilter(0);
        }
        BreadCrumbDirModel::~BreadCrumbDirModel()
        {
        }
        QString BreadCrumbDirModel::defaultPath() const
        {
            return QDir::homePath();
        }
        QString BreadCrumbDirModel::cleanPath( const QString & path ) const
        {
            makeCurrent();
#if defined(Q_OS_WIN) && !defined(Q_OS_WINCE)
            QDir dir(QDir::cleanPath(path));
            QString tempPath;
            if ( dir.exists() ) {
                tempPath = QDir::toNativeSeparators(dir.absolutePath());
                if ( !tempPath.endsWith(QDir::separator()) )
                    tempPath += QDir::separator();
                return tempPath;
            }
#endif
            return QDir::toNativeSeparators(QDir::cleanPath(path));
        }
        bool BreadCrumbDirModel::isValid( const QString & path ) const
        {
            bool valid;

            makeCurrent();
            if ( path == DirHelper::myComputer() )
                return true;
            QModelIndex index = m_dirModel->index(cleanPath(path));
            valid = index.isValid();
            return valid;
        }
        BreadCrumbModelNodeList BreadCrumbDirModel::splitPath( const QString & path ) const
        {

            QStringList list = DirHelper::splitPath(cleanPath(path));
            QString tempPath;
            BreadCrumbModelNodeList nodeList;
            bool isDir;

            makeCurrent();
            if ( path == DirHelper::myComputer() ) {
                nodeList.append(BreadCrumbModelNode(path,BreadCrumbModelNode::Global,this));
                return nodeList;
            }
            isDir = m_dirModel->isDir(m_dirModel->index(path));
            nodeList.append(BreadCrumbModelNode(DirHelper::myComputer(),BreadCrumbModelNode::Global,this));
            for (int i = 0; i < list.count(); i++) {
                tempPath = DirHelper::setupPath(list,i);
#if defined(Q_OS_WIN) && !defined(Q_OS_WINCE)
                if ((i == list.count() - 1) && isDir)
#else
                if ((i == list.count() - 1) && isDir && list.count() > 1)
#endif
                    tempPath += QDir::separator();

                bool Dir = QDir(tempPath).exists();

                nodeList.append(BreadCrumbModelNode(tempPath,i == 0 ? BreadCrumbModelNode::Root : Dir ? BreadCrumbModelNode::Container : BreadCrumbModelNode::Leaf,this));
            }
            return nodeList;
        }
        QIcon BreadCrumbDirModel::icon( const BreadCrumbModelNode & node ) const
        {
            return m_dirModel->fileIcon(m_dirModel->index(node.path()));
        }
        QString BreadCrumbDirModel::label( const BreadCrumbModelNode & node ) const
        {

            if ( node.type() == BreadCrumbModelNode::Root )
                return DirHelper::driveLabel(node.path());
            else if ( node.type() == BreadCrumbModelNode::Global )
                return node.path();
            else {
                QModelIndex index = m_dirModel->index(node.path());

                return m_dirModel->fileName(index);
            }
        }
        QMimeData * BreadCrumbDirModel::mimeData( const BreadCrumbModelNode & node ) const
        {
            QModelIndex index;

            makeCurrent();
            index = m_dirModel->index(node.path());
            if ( !index.isValid() )
                return 0;
            return m_dirModel->mimeData(QModelIndexList() << index);

        }
        QMenu * BreadCrumbDirModel::buildMenu(const BreadCrumbModelNode & node)
        {
            QFileInfo info;
            QString name;
            QString folder;
            QModelIndex index;

            makeCurrent();
            if ( node.path().isEmpty() ) {
                QMenu * menu = m_menuManager->driveMenu();

                if ( !menu->actions().isEmpty() )
                    return menu;

                for (int i = DirHelper::Home; i <=  DirHelper::Video; i++) {
                    folder = DirHelper::specialFolder((DirHelper::SpecialFolder)i);
                    if ( folder.isEmpty() )
                        continue;
                    index = m_dirModel->index(folder);
                    menu->addAction(new BreadCrumbDirModelMenuAction(m_dirModel->fileIcon(index),m_dirModel->fileName(index),m_dirModel->filePath(index),menu));
                }
                return menu;
            }
            if ( node.type() == BreadCrumbModelNode::Global ) {
                QMenu * menu = m_menuManager->mainMenu();

                menu->clear();
                foreach (QFileInfo info, QDir::drives()) {
                    folder = info.absoluteFilePath();
                    index = m_dirModel->index(folder);
                    menu->addAction(new BreadCrumbDirModelMenuAction(m_dirModel->fileIcon(index),DirHelper::driveLabel(folder),m_dirModel->filePath(index),menu));
                }
                return menu;
            } else {
                int count;
                QModelIndex childIndex;

                m_menuManager->clean();
                index = m_dirModel->index(node.path());
                if ( !index.isValid() )
                    return 0;
                if ( !m_menuManager->isRegisteredPath(node.path()) )
                    m_menuManager->registerPath(qPrintable(node.path()));
                BreadCrumbDirModelMenu * menu = m_menuManager->menu(node.path());

                menu->setFilter(filter());
                if ( !menu || (menu && !menu->needBeRebuilt()))
                    return menu;
                menu->clear();
                count = m_dirModel->rowCount(index);
                for (int i = 0; i < count; i++) {
                    childIndex = index.child(i,0);
                    if ( childIndex.isValid() )
                        menu->addAction(new BreadCrumbDirModelMenuAction(m_dirModel->fileIcon(childIndex),m_dirModel->fileName(childIndex),m_dirModel->filePath(childIndex),menu));
                }
                menu->setNeedBeRebuilt(false);
                return menu;
            }
        }
        void BreadCrumbDirModel::makeCurrent() const
        {
            QDir::Filters filter = QDir::Dirs|QDir::NoDotAndDotDot|QDir::NoSymLinks|QDir::Readable;

            if ( this->filter() & AbstractBreadCrumbModel::AllNodes )
                filter |= QDir::Files;
            if ( this->filter() & AbstractBreadCrumbModel::Hidden )
                filter |= QDir::Hidden;
            m_dirModel->setFilter(filter);
        }
    }
}
