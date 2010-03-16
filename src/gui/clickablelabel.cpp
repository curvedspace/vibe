/****************************************************************************
 *
 * Copyright (c) 2010 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 * Copyright (c) 2008 Matteo Bertozzi <theo.bertozzi@gmail.com>
 * All rights reserved.
 *
 * Contact: Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * GNU Lesser General Public License Usage
 * This file may be used under the terms of the GNU Lesser
 * General Public License version 2.1 as published by the Free Software
 * Foundation and appearing in the file LICENSE.LGPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU Lesser General Public License version 2.1 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 ***************************************************************************/

#include <QtGui/QMouseEvent>

#include "clickablelabel.h"

namespace QubeGui
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

#include "clickablelabel.moc"
