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

#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QMainWindow>

namespace Ui
{
    class PreviewWidget;
}

class PreviewWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit PreviewWidget(QWidget *parent = 0);
    ~PreviewWidget();

    bool eventFilter(QObject *, QEvent *);

private:
    void closeEvent(QCloseEvent *);
    Ui::PreviewWidget *ui;
};

#endif // PREVIEWWIDGET_H
