/* 
 *  This file is part of Qube.
 *
 *  Copyright (c) 2010 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *  Copyright (c) 2008 Matteo Bertozzi <theo.bertozzi@gmail.com>
 *
 *  Quartica is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Qube is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with Qube.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CLICKABLE_LABEL_H
#define CLICKABLE_LABEL_H

#include <QLabel>

#include "qube_global.h"

class QUBESHARED_EXPORT QClickableLabel : public QLabel
{
Q_OBJECT

public:
    QClickableLabel(QWidget *parent = 0, Qt::WindowFlags f = 0);
    QClickableLabel(const QString& text, QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~QClickableLabel();

signals:
    void clicked();
    void clicked(QClickableLabel *label);

protected:
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif  // CLICKABLE_LABEL_H
