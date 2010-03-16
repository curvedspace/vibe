/****************************************************************************
 *
 * Copyright (c) 2010 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * All rights reserved.
 * Contact: Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * GNU General Public License Usage
 * This file may be used under the terms of the GNU General Public
 * License version 2 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging
 * of this file.  Please review the following information to
 * ensure the GNU General Public License version 2 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/gpl-2.0.html.
 *
 ***************************************************************************/

#include <QtDesigner/QDesignerFormEditorInterface>
#include <QtDesigner/QExtensionManager>

#include "navbar.h"
#include "navbarplugin.h"
#include "navbarplugintaskmenu.h"
#include "navbarplugintaskmenufactory.h"

NavBarPlugin::NavBarPlugin(const QIcon &icon, QObject *parent)
    : QObject(parent),
      m_icon(icon)
{
    m_initialized = false;
}

void NavBarPlugin::initialize(QDesignerFormEditorInterface *formEditor)
{
    if (m_initialized)
	return;
    QExtensionManager *manager = formEditor->extensionManager();
    Q_ASSERT(manager != 0);
#if 0
    manager->registerExtensions(new NavBarPluginTaskMenuFactory(manager),
				Q_TYPEID(QDesignerTaskMenuExtension));
#endif
    m_initialized = true;
}

bool NavBarPlugin::isInitialized() const
{
    return m_initialized;
}

bool NavBarPlugin::isContainer() const
{
    return false;
}

QWidget *NavBarPlugin::createWidget(QWidget *parent)
{
    QubeGui::NavBar *navBar = new QubeGui::NavBar(parent);
    QubeGui::NavBarGroup *group = navBar->addGroup("GROUP");
    group->addItem(QPixmap(":/qt.png"), "Item #0");
    group->addItem(QPixmap(":/qt.png"), "Item #1");
    return navBar;
}

QString NavBarPlugin::name() const
{
    return QLatin1String("NavBar");
}

QString NavBarPlugin::group() const
{
    return QLatin1String("Qube");
}

QIcon NavBarPlugin::icon() const
{
    return m_icon;
}

QString NavBarPlugin::toolTip() const
{
    return tr("Navigation Bar");
}

QString NavBarPlugin::whatsThis() const
{
    return QLatin1String("");
}

QString NavBarPlugin::domXml() const
{
    return QLatin1String(""
			 "<ui language=\"c++\">"
			 "    <widget class=\"QubeGui::NavBar\" name=\"navBar\">"
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

QString NavBarPlugin::includeFile() const
{
    return QLatin1String("QubeGui/NavBar");
}

#include "navbarplugin.moc"
