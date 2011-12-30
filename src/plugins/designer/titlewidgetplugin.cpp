/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2010-2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Vibe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Vibe.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QtDesigner/QDesignerFormEditorInterface>
#include <QtDesigner/QExtensionManager>

#include <VibeGui/VTitleWidget>

#include "titlewidgetplugin.h"

TitleWidgetPlugin::TitleWidgetPlugin(const QIcon &icon, QObject *parent) :
    QObject(parent),
    m_icon(icon)
{
    m_initialized = false;
}

void TitleWidgetPlugin::initialize(QDesignerFormEditorInterface *formEditor)
{
    if (m_initialized)
        return;

    QExtensionManager *manager = formEditor->extensionManager();
    Q_ASSERT(manager != 0);

    m_initialized = true;
}

bool TitleWidgetPlugin::isInitialized() const
{
    return m_initialized;
}

bool TitleWidgetPlugin::isContainer() const
{
    return false;
}

QWidget *TitleWidgetPlugin::createWidget(QWidget *parent)
{
    VTitleWidget *widget = new VTitleWidget(parent);
    return widget;
}

QString TitleWidgetPlugin::name() const
{
    return QLatin1String("VTitleWidget");
}

QString TitleWidgetPlugin::group() const
{
    return QLatin1String("Vibe");
}

QIcon TitleWidgetPlugin::icon() const
{
    return m_icon;
}

QString TitleWidgetPlugin::toolTip() const
{
    return tr("Title Widget");
}

QString TitleWidgetPlugin::whatsThis() const
{
    return QLatin1String("");
}

QString TitleWidgetPlugin::domXml() const
{
    return QLatin1String(""
                         "<ui language=\"c++\">"
                         "    <widget class=\"VTitleWidget\" name=\"titleWidget\">"
                         "        <property name=\"geometry\">"
                         "            <rect>"
                         "                <x>0</x>"
                         "                <y>0</y>"
                         "                <width>100</width>"
                         "                <height>40</height>"
                         "            </rect>"
                         "        </property>"
                         "    </widget>"
                         "</ui>");
}

QString TitleWidgetPlugin::includeFile() const
{
    return QLatin1String("VibeGui/VTitleWidget");
}

#include "titlewidgetplugin.moc"