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

#ifndef QCLICKABLELABEL_H
#define QCLICKABLELABEL_H

#include <QtGui/QLabel>

#include "qube_global.h"

class QUBESHARED_EXPORT QClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit QClickableLabel(QWidget *parent = 0, Qt::WindowFlags f = 0);
    QClickableLabel(const QString &text, QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~QClickableLabel();

signals:
    void clicked();
    void clicked(QClickableLabel *label);

protected:
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // QCLICKABLELABEL_H
