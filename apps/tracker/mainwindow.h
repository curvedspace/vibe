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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QModelIndex>
#include <QtGui/QMainWindow>

namespace Ui
{
    class MainWindow;
}

class QFileSystemModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    QFileSystemModel *m_model;
    QModelIndex m_prevRoot;

    void setRootPath(const QString &rootPath);

private slots:
    void viewAsIcons();
    void viewAsList();
    void viewAsTree();
    void viewAsColumns();
    void goBack();
    void goForward();
    void doubleClicked(QModelIndex);
};

#endif // MAINWINDOW_H
