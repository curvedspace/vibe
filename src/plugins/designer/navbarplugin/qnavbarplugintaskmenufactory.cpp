#include <QtDesigner/QtDesigner>

#include "qnavbar.h"
#include "qnavbarplugintaskmenu.h"
#include "qnavbarplugintaskmenufactory.h"

QNavBarPluginTaskMenuFactory::QNavBarPluginTaskMenuFactory(QExtensionManager *parent)
    : QExtensionFactory(parent)
{
}

QObject *QNavBarPluginTaskMenuFactory::createExtension(QObject *object,
                                                       const QString &iid, QObject *parent) const
{
    if (iid != Q_TYPEID(QDesignerTaskMenuExtension))
        return 0;

    if (QNavBar *navBar = qobject_cast<QNavBar *>(object))
        return new QNavBarPluginTaskMenu(navBar, parent);

    return 0;
}
