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
