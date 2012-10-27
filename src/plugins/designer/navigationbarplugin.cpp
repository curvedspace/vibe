/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2010-2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPL2.1+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include <QtDesigner/QDesignerFormEditorInterface>
#include <QtDesigner/QExtensionManager>

#include <VibeWidgets/VNavigationBar>

#include "navigationbarplugin.h"

NavigationBarPlugin::NavigationBarPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void NavigationBarPlugin::initialize(QDesignerFormEditorInterface *formEditor)
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

bool NavigationBarPlugin::isInitialized() const
{
    return m_initialized;
}

bool NavigationBarPlugin::isContainer() const
{
    return false;
}

QWidget *NavigationBarPlugin::createWidget(QWidget *parent)
{
    VNavigationBar *navBar = new VNavigationBar(parent);
    return navBar;
}

QString NavigationBarPlugin::name() const
{
    return QStringLiteral("VNavigationBar");
}

QString NavigationBarPlugin::group() const
{
    return QStringLiteral("Vibe");
}

QIcon NavigationBarPlugin::icon() const
{
    return QIcon(":/qt.png");
}

QString NavigationBarPlugin::toolTip() const
{
    return tr("Navigation Bar");
}

QString NavigationBarPlugin::whatsThis() const
{
    return QStringLiteral("");
}

QString NavigationBarPlugin::domXml() const
{
    return QStringLiteral(""
                          "<ui language=\"c++\">"
                          "    <widget class=\"VNavigationBar\" name=\"navigationBar\">"
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

QString NavigationBarPlugin::includeFile() const
{
    return QStringLiteral("VibeWidgets/VNavigationBar");
}

#include "moc_navigationbarplugin.cpp"
