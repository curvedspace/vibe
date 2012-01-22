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

#ifndef VBREADCRUMBDIRMODEL_H
#define VBREADCRUMBDIRMODEL_H

#include <QObject>
#include <QMap>

#include <VibeCore/VGlobal>

#include "vabstractbreadcrumbmodel.h"

class QDateTime;
class QDirModel;
class QDir;
class QFileSystemWatcher;
class QMenu;
class QStringList;

class VBreadCrumbDirModelMenu;

class VIBE_EXPORT VBreadCrumbDirModelMenuManager : public QObject
{
    Q_OBJECT
public:
    VBreadCrumbDirModelMenuManager(QObject *parent = 0);
    ~VBreadCrumbDirModelMenuManager();

    VBreadCrumbDirModelMenu *menu(const QString &path) const;
    void registerPath(const QString &path);
    bool isRegisteredPath(const QString &path) const;
    QMenu *mainMenu() const;
    QMenu *driveMenu() const;

    void clearAll();
    void clean();

protected slots:
    void handleDirectoryChanged(const QString &path);

private:
    QMap< QString, VBreadCrumbDirModelMenu * > m_menuMapping;
    QMenu *m_mainMenu;
    QMenu *m_driveMenu;
    QFileSystemWatcher *m_watcher;
};

class VIBE_EXPORT VBreadCrumbDirModel : public VAbstractBreadCrumbModel
{
public:
    VBreadCrumbDirModel();
    virtual ~VBreadCrumbDirModel();

    QString defaultPath() const;
    QString cleanPath(const QString &) const;
    bool isValid(const QString &path) const;
    VBreadCrumbModelNodeList splitPath(const QString &path) const;

    QIcon icon(const VBreadCrumbModelNode &) const;
    QString label(const VBreadCrumbModelNode &) const;

    QMimeData *mimeData(const VBreadCrumbModelNode &) const;

    QMenu *buildMenu(const VBreadCrumbModelNode &);

    inline bool supportsMenuNavigation() const {
        return true;
    }

protected:
    void makeCurrent() const;

private:
    static VBreadCrumbDirModelMenuManager *m_menuManager;
    static QDirModel *m_dirModel;
};

#endif // VBREADCRUMBDIRMODEL_H
