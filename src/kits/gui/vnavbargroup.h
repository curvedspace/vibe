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

#ifndef VNAVBARGROUP_H
#define VNAVBARGROUP_H

#include <QtGui/QWidget>

#include <VibeGui/VNavBarItem>

class VNavBarGroupPrivate;

class VIBE_EXPORT VNavBarGroup : public QWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(VNavBarGroup)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(bool isStatic READ isStatic WRITE setStatic)
    Q_PROPERTY(bool isExpanded READ isExpanded WRITE expand)
public:
    explicit VNavBarGroup(QWidget *parent = 0);
    VNavBarGroup(const QString &title, QWidget *parent = 0);

    void addItem(VNavBarItem *item);
    void addItem(VNavBarItem *item, int index);

    VNavBarItem *addItem(const QString &text);
    VNavBarItem *addItem(const QPixmap &icon, const QString &text);
    VNavBarItem *addItem(const QPixmap &icon, const QString &text, int index);

    bool containsItem(VNavBarItem *item);

    QString title() const;
    void setTitle(const QString &title);

    bool isExpanded() const;

    bool isStatic() const;
    void setStatic(bool flag);

    void setTitleColor(const QColor &color);

signals:
    void selected(VNavBarGroup *group, VNavBarItem *item);
    void expanded(VNavBarGroup *group);

public slots:
    void expand(bool expand);

private slots:
    void itemSelected(VSelectableWidget *item);
    void titleClicked();

private:
    VNavBarGroupPrivate *d_ptr;
};

#endif // VNAVBARGROUP_H
