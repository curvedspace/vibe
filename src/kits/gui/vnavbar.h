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

#ifndef VNAVBAR_H
#define VNAVBAR_H

#include <QtGui/QWidget>

#include <VibeGui/VNavBarGroup>

class VNavBarPrivate;

class VIBE_EXPORT VNavBar : public QWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(VNavBar)
public:
    explicit VNavBar(QWidget *parent = 0);

    void addGroup(VNavBarGroup *group);
    VNavBarGroup *addGroup(const QString &title);
    void addGroups(QList<VNavBarGroup *> groups);

    void removeGroup(VNavBarGroup *group);

    void clear();

    QList<VNavBarGroup *> groups() const;
    bool containsGroup(VNavBarGroup *group);

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void itemSelected(VNavBarGroup *group, VNavBarItem *item);
    void groupExpanded(VNavBarGroup *);

private:
    VNavBarPrivate *d_ptr;
};

#endif // VNAVBAR_H
