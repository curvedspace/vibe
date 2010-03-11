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

#ifndef QNAVBARGROUP_H
#define QNAVBARGROUP_H

#include <QtGui/QWidget>

#include "qube_global.h"
#include "qnavbaritem.h"

class QUBESHARED_EXPORT QNavBarGroup : public QWidget
{
Q_OBJECT
Q_PROPERTY(QString title READ title WRITE setTitle)
Q_PROPERTY(bool isStatic READ isStatic WRITE setStatic)
Q_PROPERTY(bool isExpanded READ isExpanded WRITE expand)
public:
    explicit QNavBarGroup(QWidget *parent = 0);
    QNavBarGroup(const QString &title, QWidget *parent = 0);
    ~QNavBarGroup();

    void addItem(QNavBarItem *item);
    void addItem(QNavBarItem *item, int index);

    QNavBarItem *addItem(const QString &text);
    QNavBarItem *addItem(const QPixmap &icon, const QString &text);
    QNavBarItem *addItem(const QPixmap &icon, const QString &text, int index);

    bool containsItem(QNavBarItem *item);

    QString title() const;
    void setTitle(const QString &title);

    bool isExpanded() const;

    bool isStatic() const;
    void setStatic(bool flag);

    void setTitleColor(const QColor &color);

signals:
    void selected(QNavBarGroup *group, QNavBarItem *item);
    void expanded(QNavBarGroup *group);

public slots:
    void expand(bool expand);

private slots:
    void itemSelected(QSelectableWidget *item);
    void titleClicked();

private:
    class Private;
    Private *d;
};

#endif // QNAVBARGROUP_H
