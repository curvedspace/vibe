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

#include "vbreadcrumbdirmodel.h"
#include "dir_helper.h"

QDirModel *VBreadCrumbDirModel::m_dirModel = 0;
VBreadCrumbDirModelMenuManager *VBreadCrumbDirModel::m_menuManager = 0;

class VBreadCrumbDirModelMenuAction : public QAction
{
public:
    VBreadCrumbDirModelMenuAction(const QIcon &icon, const QString &fileName, const QString &path, QObject *parent) :
        QAction(icon, fileName, parent) {
        setData(path);
    }
};

/*
 * VBreadCrumbDirModelMenu
 */

class VBreadCrumbDirModelMenu : public QMenu
{
public:
    VBreadCrumbDirModelMenu() : QMenu(0), m_need(true) { }

    inline VAbstractBreadCrumbModel::Filters filter() const {
        return m_filter;
    }
    void setFilter(VAbstractBreadCrumbModel::Filters filter);
    inline bool needBeRebuilt() const {
        return m_need;
    }
    inline void setNeedBeRebuilt(bool need) {
        m_need = need;
    }

    bool m_need;

private:
    VAbstractBreadCrumbModel::Filters m_filter;
};

void VBreadCrumbDirModelMenu::setFilter(VAbstractBreadCrumbModel::Filters filter)
{
    if (m_filter == filter)
        return;
    m_filter = filter;
    m_need = true;
}

/*
 * VBreadCrumbDirModelMenuManager
 */

VBreadCrumbDirModelMenuManager::VBreadCrumbDirModelMenuManager(QObject *parent) :
    QObject(parent)
{
    m_watcher = new QFileSystemWatcher(this);
    m_mainMenu = new QMenu;
    m_driveMenu = new QMenu;
    connect(m_watcher, SIGNAL(directoryChanged(QString)), this, SLOT(handleDirectoryChanged(QString)));
}

VBreadCrumbDirModelMenuManager::~VBreadCrumbDirModelMenuManager()
{
    delete m_mainMenu;
    delete m_driveMenu;
    foreach(QString path, m_menuMapping.keys()) {
        delete m_menuMapping.take(path);
    }
}

QMenu *VBreadCrumbDirModelMenuManager::mainMenu() const
{
    return m_mainMenu;
}

QMenu *VBreadCrumbDirModelMenuManager::driveMenu() const
{
    return m_driveMenu;
}

void VBreadCrumbDirModelMenuManager::registerPath(const QString &path)
{
    QString tempPath = path;

    if (!tempPath.endsWith(QDir::separator()))
        tempPath += QDir::separator();
    if (m_watcher->directories().contains(tempPath))
        return;
    m_watcher->addPath(tempPath);
    m_menuMapping[tempPath] = new VBreadCrumbDirModelMenu;
}

bool VBreadCrumbDirModelMenuManager::isRegisteredPath(const QString &path) const
{
    QString tempPath = path;

    if (!tempPath.endsWith(QDir::separator()))
        tempPath += QDir::separator();
    return m_watcher->directories().contains(tempPath);
}

VBreadCrumbDirModelMenu *VBreadCrumbDirModelMenuManager::menu(const QString &path) const
{
    QString tempPath = path;

    if (!tempPath.endsWith(QDir::separator()))
        tempPath += QDir::separator();
    if (!isRegisteredPath(tempPath))
        return 0;
    return m_menuMapping[tempPath];
}

void VBreadCrumbDirModelMenuManager::clean()
{
    foreach(QString path, m_watcher->directories()) {
        QDir dir(path);

        if (!dir.exists()) {
            m_watcher->removePath(path);
            delete m_menuMapping.take(path);
        }
    }
}

void VBreadCrumbDirModelMenuManager::handleDirectoryChanged(const QString &path)
{
    m_menuMapping[path]->setNeedBeRebuilt(true);
}

/*
 * VBreadCrumbDirModel
 */

VBreadCrumbDirModel::VBreadCrumbDirModel()
{
    if (!m_dirModel)
        m_dirModel = new QDirModel(qApp);
    if (!m_menuManager)
        m_menuManager = new VBreadCrumbDirModelMenuManager(qApp);
    setItemModel(m_dirModel);
    setFilter(0);
}

VBreadCrumbDirModel::~VBreadCrumbDirModel()
{
}

QString VBreadCrumbDirModel::defaultPath() const
{
    return QDir::homePath();
}

QString VBreadCrumbDirModel::cleanPath(const QString &path) const
{
    makeCurrent();
#if defined(Q_OS_WIN) && !defined(Q_OS_WINCE)
    QDir dir(QDir::cleanPath(path));
    QString tempPath;
    if (dir.exists()) {
        tempPath = QDir::toNativeSeparators(dir.absolutePath());
        if (!tempPath.endsWith(QDir::separator()))
            tempPath += QDir::separator();
        return tempPath;
    }
#endif
    return QDir::toNativeSeparators(QDir::cleanPath(path));
}

bool VBreadCrumbDirModel::isValid(const QString &path) const
{
    bool valid;

    makeCurrent();
    if (path == DirHelper::myComputer())
        return true;
    QModelIndex index = m_dirModel->index(cleanPath(path));
    valid = index.isValid();
    return valid;
}

VBreadCrumbModelNodeList VBreadCrumbDirModel::splitPath(const QString &path) const
{
    QStringList list = DirHelper::splitPath(cleanPath(path));
    QString tempPath;
    VBreadCrumbModelNodeList nodeList;
    bool isDir;

    makeCurrent();
    if (path == DirHelper::myComputer()) {
        nodeList.append(VBreadCrumbModelNode(path, VBreadCrumbModelNode::Global, this));
        return nodeList;
    }
    isDir = m_dirModel->isDir(m_dirModel->index(path));
    nodeList.append(VBreadCrumbModelNode(DirHelper::myComputer(), VBreadCrumbModelNode::Global, this));
    for (int i = 0; i < list.count(); i++) {
        tempPath = DirHelper::setupPath(list, i);
#if defined(Q_OS_WIN) && !defined(Q_OS_WINCE)
        if ((i == list.count() - 1) && isDir)
#else
        if ((i == list.count() - 1) && isDir && list.count() > 1)
#endif
            tempPath += QDir::separator();

        bool Dir = QDir(tempPath).exists();

        nodeList.append(VBreadCrumbModelNode(tempPath, i == 0 ? VBreadCrumbModelNode::Root : Dir ? VBreadCrumbModelNode::Container : VBreadCrumbModelNode::Leaf, this));
    }
    return nodeList;
}

QIcon VBreadCrumbDirModel::icon(const VBreadCrumbModelNode &node) const
{
    return m_dirModel->fileIcon(m_dirModel->index(node.path()));
}

QString VBreadCrumbDirModel::label(const VBreadCrumbModelNode &node) const
{

    if (node.type() == VBreadCrumbModelNode::Root)
        return DirHelper::driveLabel(node.path());
    else if (node.type() == VBreadCrumbModelNode::Global)
        return node.path();
    else {
        QModelIndex index = m_dirModel->index(node.path());

        return m_dirModel->fileName(index);
    }
}

QMimeData *VBreadCrumbDirModel::mimeData(const VBreadCrumbModelNode &node) const
{
    QModelIndex index;

    makeCurrent();
    index = m_dirModel->index(node.path());
    if (!index.isValid())
        return 0;
    return m_dirModel->mimeData(QModelIndexList() << index);

}

QMenu *VBreadCrumbDirModel::buildMenu(const VBreadCrumbModelNode &node)
{
    QFileInfo info;
    QString name;
    QString folder;
    QModelIndex index;

    makeCurrent();
    if (node.path().isEmpty()) {
        QMenu *menu = m_menuManager->driveMenu();

        if (!menu->actions().isEmpty())
            return menu;

        for (int i = DirHelper::Home; i <=  DirHelper::Video; i++) {
            folder = DirHelper::specialFolder((DirHelper::SpecialFolder)i);
            if (folder.isEmpty())
                continue;
            index = m_dirModel->index(folder);
            menu->addAction(new VBreadCrumbDirModelMenuAction(m_dirModel->fileIcon(index), m_dirModel->fileName(index), m_dirModel->filePath(index), menu));
        }
        return menu;
    }
    if (node.type() == VBreadCrumbModelNode::Global) {
        QMenu *menu = m_menuManager->mainMenu();

        menu->clear();
        foreach(QFileInfo info, QDir::drives()) {
            folder = info.absoluteFilePath();
            index = m_dirModel->index(folder);
            menu->addAction(new VBreadCrumbDirModelMenuAction(m_dirModel->fileIcon(index), DirHelper::driveLabel(folder), m_dirModel->filePath(index), menu));
        }
        return menu;
    } else {
        int count;
        QModelIndex childIndex;

        m_menuManager->clean();
        index = m_dirModel->index(node.path());
        if (!index.isValid())
            return 0;
        if (!m_menuManager->isRegisteredPath(node.path()))
            m_menuManager->registerPath(qPrintable(node.path()));
        VBreadCrumbDirModelMenu *menu = m_menuManager->menu(node.path());

        menu->setFilter(filter());
        if (!menu || (menu && !menu->needBeRebuilt()))
            return menu;
        menu->clear();
        count = m_dirModel->rowCount(index);
        for (int i = 0; i < count; i++) {
            childIndex = index.child(i, 0);
            if (childIndex.isValid())
                menu->addAction(new VBreadCrumbDirModelMenuAction(m_dirModel->fileIcon(childIndex), m_dirModel->fileName(childIndex), m_dirModel->filePath(childIndex), menu));
        }
        menu->setNeedBeRebuilt(false);
        return menu;
    }
}

void VBreadCrumbDirModel::makeCurrent() const
{
    QDir::Filters filter = QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Readable;

    if (this->filter() & VAbstractBreadCrumbModel::AllNodes)
        filter |= QDir::Files;
    if (this->filter() & VAbstractBreadCrumbModel::Hidden)
        filter |= QDir::Hidden;
    m_dirModel->setFilter(filter);
}

#include "vbreadcrumbdirmodel.moc"
