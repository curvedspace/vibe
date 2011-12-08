/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2008 Matteo Bertozzi
 * Copyright (c) 2010-2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Matteo Bertozzi <theo.bertozzi@gmail.com>
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

#ifndef VCLICKABLELABEL_H
#define VCLICKABLELABEL_H

#include <QtGui/QLabel>

#include <VibeCore/VGlobal>

class VIBE_EXPORT VClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit VClickableLabel(QWidget *parent = 0, Qt::WindowFlags f = 0);
    VClickableLabel(const QString &text, QWidget *parent = 0, Qt::WindowFlags f = 0);

signals:
    void clicked();
    void clicked(VClickableLabel *label);

protected:
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // VCLICKABLELABEL_H
