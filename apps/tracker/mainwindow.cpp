/****************************************************************************
 *
 * Copyright (c) 2010 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * All rights reserved.
 * Contact: Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * GNU General Public License Usage
 * This file may be used under the terms of the GNU General Public
 * License version 2 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging
 * of this file.  Please review the following information to
 * ensure the GNU General Public License version 2 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/gpl-2.0.html.
 *
 ***************************************************************************/

#include <QtGui/QActionGroup>
#include <QtGui/QDesktopServices>
#include <QtGui/QFileSystemModel>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow())
{
    ui->setupUi(this);

    // Set icons
    ui->actionNew_Window->setIcon(QIcon::fromTheme("window-new"));
    ui->actionNew_Tab->setIcon(QIcon::fromTheme("tab-new"));
    ui->actionNew_Folder->setIcon(QIcon::fromTheme("folder-new"));
    //ui->actionNew_Smart_Folder
    //ui->actionNew_Burning_Folder
    ui->actionOpen->setIcon(QIcon::fromTheme("document-open"));
    ui->actionPrint->setIcon(QIcon::fromTheme("document-print"));
    ui->actionClose->setIcon(QIcon::fromTheme("window-close"));
    ui->actionGet_Information->setIcon(QIcon::fromTheme("documentinfo"));
    ui->actionEject->setIcon(QIcon::fromTheme("media-eject"));
    ui->actionUndo->setIcon(QIcon::fromTheme("edit-undo"));
    ui->actionRedo->setIcon(QIcon::fromTheme("edit-redo"));
    ui->actionCut->setIcon(QIcon::fromTheme("edit-cut"));
    ui->actionCopy->setIcon(QIcon::fromTheme("edit-copy"));
    ui->actionPaste->setIcon(QIcon::fromTheme("edit-paste"));
    ui->actionSelect_All->setIcon(QIcon::fromTheme("edit-select-all"));
    ui->actionBack->setIcon(QIcon::fromTheme("draw-arrow-back"));
    ui->actionForward->setIcon(QIcon::fromTheme("draw-arrow-forward"));

    // Set actions
    QActionGroup viewMode(this);
    viewMode.addAction(ui->actionAs_Icons);
    viewMode.addAction(ui->actionAs_List);
    viewMode.addAction(ui->actionAs_Tree);
    viewMode.addAction(ui->actionAs_Columns);
    connect(ui->actionAs_Icons, SIGNAL(triggered()),
	    this, SLOT(viewAsIcons()));
    connect(ui->actionAs_List, SIGNAL(triggered()),
	    this, SLOT(viewAsList()));
    connect(ui->actionAs_Tree, SIGNAL(triggered()),
	    this, SLOT(viewAsTree()));
    connect(ui->actionAs_Columns, SIGNAL(triggered()),
	    this, SLOT(viewAsColumns()));
    connect(ui->actionBack, SIGNAL(triggered()),
	    this, SLOT(goBack()));
    connect(ui->actionForward, SIGNAL(triggered()),
	    this, SLOT(goForward()));

    // Set views
    m_model = new QFileSystemModel(this);
    m_model->setReadOnly(false);
    ui->listView->setModel(m_model);
    ui->treeView->setModel(m_model);
    ui->columnView->setModel(m_model);
    QModelIndex root = m_model->setRootPath(QDir::homePath());
    ui->listView->setRootIndex(root);
    ui->treeView->setRootIndex(root);
    ui->columnView->setRootIndex(root);
    connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)),
	    this, SLOT(doubleClicked(QModelIndex)));
    connect(ui->treeView, SIGNAL(doubleClicked(QModelIndex)),
	    this, SLOT(doubleClicked(QModelIndex)));
    connect(ui->columnView, SIGNAL(doubleClicked(QModelIndex)),
	    this, SLOT(doubleClicked(QModelIndex)));

    // Set navigation bar (devices);
    QubeGui::NavBarGroup *devices = ui->navbar->addGroup(tr("DEVICES"));

    // Set navigation bar (locations)
    QHash<QDesktopServices::StandardLocation, QString> locationsHash;
    locationsHash[QDesktopServices::DesktopLocation] = tr("Desktop");
    locationsHash[QDesktopServices::HomeLocation] = tr("Home");
    locationsHash[QDesktopServices::DocumentsLocation] = tr("Documents");
    QubeGui::NavBarGroup *locations = ui->navbar->addGroup(tr("LOCATIONS"));
    QHashIterator<QDesktopServices::StandardLocation, QString>
    locationsIterator(locationsHash);
    while (locationsIterator.hasNext()) {
	locationsIterator.next();
	QString path = QDesktopServices::storageLocation(locationsIterator.key());
	QModelIndex index = m_model->index(path);
	QPixmap pixmap = m_model->fileIcon(index).pixmap(QSize(48, 48), QIcon::Normal, QIcon::On);
	locations->addItem(pixmap, locationsIterator.value());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_model;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
	case QEvent::LanguageChange:
	    ui->retranslateUi(this);
	    break;
	default:
	    break;
    }
}

void MainWindow::setRootPath(const QString &rootPath)
{
    m_prevRoot = m_model->index(m_model->rootPath());
    QModelIndex root = m_model->setRootPath(rootPath);
    ui->listView->setRootIndex(root);
    ui->treeView->setRootIndex(root);
    ui->columnView->setRootIndex(root);
}

void MainWindow::viewAsIcons()
{
    ui->listView->setViewMode(QListView::IconMode);
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::viewAsList()
{
    ui->listView->setViewMode(QListView::ListMode);
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::viewAsTree()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::viewAsColumns()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::goBack()
{
    if (m_prevRoot.isValid())
	setRootPath(m_model->filePath(m_prevRoot));
}

void MainWindow::goForward()
{
}

void MainWindow::doubleClicked(QModelIndex index)
{
    setRootPath(m_model->filePath(index));
}

#include "mainwindow.moc"
