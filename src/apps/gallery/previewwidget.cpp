/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Vibe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Vibe.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QtEvents>
#include <QAction>

#include "previewwidget.h"
#include "ui_previewwidget.h"

PreviewWidget::PreviewWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PreviewWidget)
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
#if 1
    return QWidget::eventFilter(o, e);
#else
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
#endif
}

void PreviewWidget::closeEvent(QCloseEvent *e)
{
#if 0
    e->ignore();
#else
    QWidget::closeEvent(e);
#endif
}

#include "moc_previewwidget.cpp"
