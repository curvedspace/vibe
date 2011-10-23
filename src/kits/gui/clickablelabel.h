/****************************************************************************
 * This file is part of Qube.
 *
 * Copyright (c) 2008 Matteo Bertozzi
 * Copyright (c) 2010-2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Matteo Bertozzi <theo.bertozzi@gmail.com>
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Qube is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Qube is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Qube.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef QUBE_CLICKABLELABEL_H
#define QUBE_CLICKABLELABEL_H

#include <QtGui/QLabel>

namespace Qube
{
    namespace Gui
    {
        class ClickableLabel : public QLabel
        {
            Q_OBJECT
        public:
            explicit ClickableLabel(QWidget *parent = 0, Qt::WindowFlags f = 0);
            ClickableLabel(const QString &text, QWidget *parent = 0, Qt::WindowFlags f = 0);

        signals:
            void clicked();
            void clicked(ClickableLabel *label);

        protected:
            void mouseReleaseEvent(QMouseEvent *event);
        };
    }
}

#endif // QUBE_CLICKABLELABEL_H
