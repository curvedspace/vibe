/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the tools applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtEvents>
#include <QAction>

#include "previewwidget.h"
#include "ui_previewwidget.h"

QT_BEGIN_NAMESPACE

PreviewWidget::PreviewWidget(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::PreviewWidget)
{
    ui->setupUi(this);

    ui->actionItem1->setIcon(QIcon::fromTheme("document-new"));
    ui->actionItem2->setIcon(QIcon::fromTheme("document-open"));
    ui->ToolBar1->addAction(ui->actionItem1);
    ui->ToolBar1->addSeparator();
    ui->ToolBar1->addAction(ui->actionItem2);
    ui->actionItem5->setIcon(QIcon::fromTheme("folder-open"));
    ui->ToolBar2->addAction(ui->actionItem4);
    ui->ToolBar2->addAction(ui->actionItem5);
    ui->ToolBar3->addAction(ui->actionItem2);
    ui->ToolBar3->addAction(ui->actionItem3);

#if 0
    // install event filter on child widgets
    QList<QWidget *> l = findChildren<QWidget *>();
    foreach(QWidget * w, l) {
        w->installEventFilter(this);
        w->setFocusPolicy(Qt::NoFocus);
    }
#endif
}

PreviewWidget::~PreviewWidget()
{
    delete ui;
}

bool PreviewWidget::eventFilter(QObject *o, QEvent *e)
{
    return QWidget::eventFilter(o, e);
    switch (e->type()) {
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseButtonDblClick:
        case QEvent::MouseMove:
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
        case QEvent::Enter:
        case QEvent::Leave:
            return true; // ignore;
        default:
            break;
    }
    return false;
}

void PreviewWidget::closeEvent(QCloseEvent *e)
{
    //e->ignore();
    QWidget::closeEvent(e);
}

QT_END_NAMESPACE

#include "previewwidget.moc"
