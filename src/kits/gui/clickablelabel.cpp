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

#include <QtGui/QMouseEvent>

#include "clickablelabel.h"

namespace Qube
{
    namespace Gui
    {
        ClickableLabel::ClickableLabel(QWidget *parent, Qt::WindowFlags f)
            : QLabel(parent, f)
        {
        }

        ClickableLabel::ClickableLabel(const QString &text, QWidget *parent, Qt::WindowFlags f)
            : QLabel(text, parent, f)
        {
        }

        void ClickableLabel::mouseReleaseEvent(QMouseEvent *event)
        {
            QLabel::mouseReleaseEvent(event);

            // Accept event
            event->setAccepted(true);

            // Raise clicked event
            emit clicked();
            emit clicked(this);
        }
    }
}
