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

#include "desktopitem.h"

DesktopItem::DesktopItem(QObject *obj) :
    QObject(obj)
{
    connect(&desktopWidget, SIGNAL(resized(int)),
            this, SIGNAL(screenGeometryChanged()));
    connect(&desktopWidget, SIGNAL(resized(int)),
            this, SIGNAL(availableGeometryChanged()));
    connect(&desktopWidget, SIGNAL(workAreaResized(int)),
            this, SIGNAL(availableGeometryChanged()));
    connect(&desktopWidget, SIGNAL(screenCountChanged(int)),
            this, SIGNAL(screenCountChanged()));
}

int DesktopItem::screenCount() const
{
    return desktopWidget.screenCount();
}

QRect DesktopItem::screenGeometry(int screen) const
{
    return desktopWidget.screenGeometry(screen);
}

QRect DesktopItem::availableGeometry(int screen) const
{
    return desktopWidget.availableGeometry(screen);
}

int DesktopItem::screenWidth() const
{
    return desktopWidget.screenGeometry().width();
}

int DesktopItem::screenHeight() const
{
    return desktopWidget.screenGeometry().height();
}

int DesktopItem::availableWidth() const
{
    return desktopWidget.availableGeometry().width();
}

int DesktopItem::availableHeight() const
{
    return desktopWidget.availableGeometry().height();
}

DesktopItem *DesktopItem::qmlAttachedProperties(QObject *obj)
{
    return new DesktopItem(obj);
}

#include "moc_desktopitem.cpp"
