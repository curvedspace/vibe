#include <QtCore/QtPlugin>

#include "qubewidgets.h"
#include "navbarplugin/qnavbarplugin.h"

QubeWidgets::QubeWidgets(QObject *parent)
    : QObject(parent)
{
    const QIcon icon(":/qt.png");
    m_plugins.append(new QNavBarPlugin(icon, this));
}

QList<QDesignerCustomWidgetInterface *> QubeWidgets::customWidgets() const
{
    return m_plugins;
}

Q_EXPORT_PLUGIN2(QubeWidgetsPlugin, QubeWidgets)

#include "qubewidgets.moc"
