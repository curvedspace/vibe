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

#ifndef NAVBARGROUP_H
#define NAVBARGROUP_H

#include <QWidget>

#include "qube_global.h"
#include "qnavbaritem.h"

class QUBESHARED_EXPORT QNavBarGroup : public QWidget
{
Q_OBJECT

Q_PROPERTY(QString title READ title WRITE setTitle)
Q_PROPERTY(bool isStatic READ isStatic WRITE setStatic)
Q_PROPERTY(bool isExpanded READ isExpanded WRITE expand)
public:
    QNavBarGroup(QWidget *parent = 0);
    QNavBarGroup(const QString& title, QWidget *parent = 0);
    ~QNavBarGroup();

    // Methods - Add Item
    void addItem(QNavBarItem *item);
    void addItem(QNavBarItem *item, int index);

    // Methods - Create and Add Item
    QNavBarItem *addItem(const QString& text);
    QNavBarItem *addItem(const QPixmap& icon, const QString& text);
    QNavBarItem *addItem(const QPixmap& icon, const QString& text, int index);

    // Methods
    bool containsItem(QNavBarItem *item);

    // GET Properties
    QString title() const;
    bool isStatic() const;
    bool isExpanded() const;

    // SET Properties
    void setTitle(const QString& title);
    void setTitleColor(const QColor& color);
    void setStatic(bool flag);

signals:
    void selected(QNavBarGroup *group, QNavBarItem *item);
    void expanded(QNavBarGroup *group);

public slots:
    void expand(bool expand);

private slots:
    void onItemSelected(QSelectableWidget *item);
    void onTitleClicked();

private:
    class Private;
    Private *d;
};

#endif  // NAVBARGROUP_H
