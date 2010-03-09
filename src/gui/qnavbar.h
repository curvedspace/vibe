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

#ifndef NAVBAR_H_
#define NAVBAR_H_

#include <QtGui/QWidget>

#include "qube_global.h"
#include "qnavbargroup.h"

class QUBESHARED_EXPORT QNavBar : public QWidget
{
Q_OBJECT

Q_PROPERTY(QColor selectionColor READ selectionColor WRITE setSelectionColor)
Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
public:
    QNavBar(QWidget *parent = 0);
    ~QNavBar();

    // Methods - Groups Related
    void addGroup(QNavBarGroup *group);
    QNavBarGroup *addGroup(const QString& title);
    void addGroups(QList<QNavBarGroup *> groups);

    void removeGroup(QNavBarGroup *group);

    void clear();

    QList<QNavBarGroup *> groups() const;
    bool containsGroup(QNavBarGroup *group);

    // GET Properties
    QColor selectionColor(void) const;
    QColor backgroundColor(void) const;

    // SET Properties
    void setBackgroundColor(const QColor& bgColor);
    void setSelectionColor(const QColor& selectionColor);

protected:
    void paintEvent (QPaintEvent *event);

private slots:
    void onGroupExpanded(QNavBarGroup *group);
    void onItemSelected(QNavBarGroup *group, QNavBarItem *item);

private:
    class Private;
    Private *d;
};

#endif  // NAVBAR_H_
