/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
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

#ifndef DESKTOPITEM_H
#define DESKTOPITEM_H

#include <QDesktopWidget>
#include <QQmlComponent>

class DesktopItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int screenWidth READ screenWidth NOTIFY screenGeometryChanged)
    Q_PROPERTY(int screenHeight READ screenHeight NOTIFY screenGeometryChanged)
    Q_PROPERTY(int availableWidth READ availableWidth NOTIFY availableGeometryChanged)
    Q_PROPERTY(int availableHeight READ availableHeight NOTIFY availableGeometryChanged)
    Q_PROPERTY(int screenCount READ screenCount NOTIFY screenCountChanged)
public:
    explicit DesktopItem(QObject *obj);

    int screenCount() const;
    Q_INVOKABLE QRect screenGeometry(int screen) const;
    Q_INVOKABLE QRect availableGeometry(int screen) const;
    int screenWidth() const;
    int screenHeight() const;
    int availableWidth() const;
    int availableHeight() const;
    static DesktopItem *qmlAttachedProperties(QObject *obj);

private:
    QDesktopWidget desktopWidget;

Q_SIGNALS:
    void screenGeometryChanged();
    void availableGeometryChanged();
    void screenCountChanged();
};

QML_DECLARE_TYPEINFO(DesktopItem, QML_HAS_ATTACHED_PROPERTIES)

#endif // DESKTOPITEM_H
