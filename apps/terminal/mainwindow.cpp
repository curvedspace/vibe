/****************************************************************************
 *
 * Copyright (c) 2010 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * All rights reserved.
 * Contact: Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * GNU Lesser General Public License Usage
 * This file may be used under the terms of the GNU Lesser
 * General Public License version 2.1 as published by the Free Software
 * Foundation and appearing in the file LICENSE.LGPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU Lesser General Public License version 2.1 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 ***************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "termwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Connect signals
    connect(ui->actionNew_Tab, SIGNAL(triggered()),
            this, SLOT(newTab()));
    connect(ui->actionNew_Window, SIGNAL(triggered()),
            this, SLOT(newWindow()));
    connect(ui->action_Close_Tab, SIGNAL(triggered()),
            this, SLOT(closeCurrentTab()));
    connect(ui->action_Open_File_Manager, SIGNAL(triggered()),
            this, SLOT(openFileManager()));
    connect(ui->action_Quit, SIGNAL(triggered()),
            this, SLOT(close()));
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)),
            this, SLOT(closeTab(int)));
    // Start with a tab
    newTab();
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::newTab()
{
    Terminal::TermWidget *console = new Terminal::TermWidget();
    console->setScrollBarPosition(Terminal::TermWidget::ScrollBarRight);
    ui->tabWidget->addTab(console, "Shell");
    ui->tabWidget->setCurrentWidget(console);
    connect(console, SIGNAL(finished()),
            this, SLOT(closeCurrentTab()));
}

void MainWindow::newWindow()
{
}

void MainWindow::openFileManager()
{
}

void MainWindow::closeCurrentTab()
{
    ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
    if (ui->tabWidget->count() == 0)
        close();
}

void MainWindow::closeTab(int index)
{
    ui->tabWidget->removeTab(index);
    if (ui->tabWidget->count() == 0)
        close();
}

#include "mainwindow.moc"
