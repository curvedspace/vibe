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

#ifndef QNAVBAR_H
#define QNAVBAR_H

#include <QtGui/QWidget>

#include "qube_global.h"
#include "qnavbargroup.h"

class QUBESHARED_EXPORT QNavBar : public QWidget
{
Q_OBJECT
public:
    explicit QNavBar(QWidget *parent = 0);
    ~QNavBar();

    void addGroup(QNavBarGroup *group);
    QNavBarGroup *addGroup(const QString& title);
    void addGroups(QList<QNavBarGroup *> groups);

    void removeGroup(QNavBarGroup *group);

    void clear();

    QList<QNavBarGroup *> groups() const;
    bool containsGroup(QNavBarGroup *group);

protected:
    void paintEvent (QPaintEvent *event);

private slots:
    void itemSelected(QNavBarGroup *group, QNavBarItem *item);
    void groupExpanded(QNavBarGroup *);

private:
    class Private;
    Private *d;
};

#endif // QNAVBAR_H
