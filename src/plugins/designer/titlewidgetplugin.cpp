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

#include <VibeWidgets/VTitleWidget>

#include "titlewidgetplugin.h"

TitleWidgetPlugin::TitleWidgetPlugin(QObject *parent)
    : QObject(parent)
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
    return QStringLiteral("VTitleWidget");
}

QString TitleWidgetPlugin::group() const
{
    return QStringLiteral("Vibe");
}

QIcon TitleWidgetPlugin::icon() const
{
    return QIcon(":/qt.png");
}

QString TitleWidgetPlugin::toolTip() const
{
    return tr("Title Widget");
}

QString TitleWidgetPlugin::whatsThis() const
{
    return QStringLiteral("");
}

QString TitleWidgetPlugin::domXml() const
{
    return QStringLiteral(""
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
    return QStringLiteral("VibeWidgets/VTitleWidget");
}

#include "moc_titlewidgetplugin.cpp"
