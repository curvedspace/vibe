/* 
 *  This file is part of Quartica.
 *
 *  Copyright (c) 2008 Matteo Bertozzi <theo.bertozzi@gmail.com>
 *
 *  Quartica is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Quartica is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Quartica.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QMouseEvent>

#include "qclickablelabel.h"

QClickableLabel::QClickableLabel(QWidget *parent, Qt::WindowFlags f)
    : QLabel(parent, f)
{
}

QClickableLabel::QClickableLabel(const QString& text, QWidget *parent, Qt::WindowFlags f)
    : QLabel(text, parent, f)
{
}

QClickableLabel::~QClickableLabel()
{
}

void QClickableLabel::mouseReleaseEvent(QMouseEvent *event)
{
    QLabel::mouseReleaseEvent(event);

    // Accept Event
    event->setAccepted(true);

    // Raise Clicked Event
    emit clicked();
    emit clicked(this);
}
