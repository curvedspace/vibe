/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 * Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1
 * as published by the Free Software Foundation.
 *
 * Vibe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Vibe.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef VCOLORBUTTON_H
#define VCOLORBUTTON_H

#include <QAbstractButton>

#include <VibeCore/VGlobal>

class VIBE_EXPORT VColorButton : public QAbstractButton
{
    Q_OBJECT
public:
    VColorButton(QWidget *);
    VColorButton(const QColor &, QWidget *);

    const QColor &color() const {
        return col;
    }

    void setColor(const QColor &);
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void dragEnterEvent(QDragEnterEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);

signals:
    void colorChanged(const QColor &);

protected:
    void paintEvent(QPaintEvent *);
    void drawButton(QPainter *);
    void drawButtonLabel(QPainter *);

private slots:
    void changeColor();


private:
    QColor col;
    QPoint presspos;
    bool mousepressed;
};

#endif // VCOLORBUTTON_H
