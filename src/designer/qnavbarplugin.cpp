#include <QtCore/QtPlugin>

#include "qnavbar.h"
#include "qnavbarplugin.h"

QNavBarPlugin::QNavBarPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void QNavBarPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool QNavBarPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *QNavBarPlugin::createWidget(QWidget *parent)
{
    return new QNavBar(parent);
}

QString QNavBarPlugin::name() const
{
    return QLatin1String("QNavBar");
}

QString QNavBarPlugin::group() const
{
    return QLatin1String("Qube");
}

QIcon QNavBarPlugin::icon() const
{
    return QIcon();
}

QString QNavBarPlugin::toolTip() const
{
    return QLatin1String("Navigation bar");
}

QString QNavBarPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool QNavBarPlugin::isContainer() const
{
    return false;
}

QString QNavBarPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QNavBar\" name=\"qNavBar\">\n</widget>\n");
}

QString QNavBarPlugin::includeFile() const
{
    return QLatin1String("qnavbar.h");
}

Q_EXPORT_PLUGIN2(qnavbarplugin, QNavBarPlugin)
