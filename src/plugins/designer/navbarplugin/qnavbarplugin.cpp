#include <QtDesigner/QDesignerFormEditorInterface>
#include <QtDesigner/QExtensionManager>

#include "qnavbar.h"
#include "qnavbarplugin.h"
#include "qnavbarplugintaskmenu.h"
#include "qnavbarplugintaskmenufactory.h"

QNavBarPlugin::QNavBarPlugin(const QIcon &icon, QObject *parent)
    : QObject(parent),
      m_icon(icon)
{
    m_initialized = false;
}

void QNavBarPlugin::initialize(QDesignerFormEditorInterface *formEditor)
{
    if (m_initialized)
        return;
    QExtensionManager *manager = formEditor->extensionManager();
    Q_ASSERT(manager != 0);
#if 0
    manager->registerExtensions(new QNavBarPluginTaskMenuFactory(manager),
                                Q_TYPEID(QDesignerTaskMenuExtension));
#endif
    m_initialized = true;
}

bool QNavBarPlugin::isInitialized() const
{
    return m_initialized;
}

bool QNavBarPlugin::isContainer() const
{
    return false;
}

QWidget *QNavBarPlugin::createWidget(QWidget *parent)
{
    QNavBar *navBar = new QNavBar(parent);
    QNavBarGroup *group = navBar->addGroup("GROUP");
    group->addItem(QPixmap(":/qt.png"), "Item #0");
    group->addItem(QPixmap(":/qt.png"), "Item #1");
    return navBar;
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
    return m_icon;
}

QString QNavBarPlugin::toolTip() const
{
    return tr("Navigation Bar");
}

QString QNavBarPlugin::whatsThis() const
{
    return QLatin1String("");
}

QString QNavBarPlugin::domXml() const
{
    return QLatin1String(""
                         "<ui language=\"c++\">"
                         "    <widget class=\"QNavBar\" name=\"navBar\">"
                         "        <property name=\"geometry\">"
                         "            <rect>"
                         "                <x>0</x>"
                         "                <y>0</y>"
                         "                <width>100</width>"
                         "                <height>80</height>"
                         "            </rect>"
                         "        </property>"
                         "    </widget>"
                         "</ui>");
}

QString QNavBarPlugin::includeFile() const
{
    return QLatin1String("QubeGui/QNavBar");
}

#include "qnavbarplugin.moc"
