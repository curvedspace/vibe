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

#include <VibeWidgets/VFilePlacesView>

#include "fileplacesviewplugin.h"

FilePlacesViewPlugin::FilePlacesViewPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void FilePlacesViewPlugin::initialize(QDesignerFormEditorInterface *formEditor)
{
    if (m_initialized)
        return;

    QExtensionManager *manager = formEditor->extensionManager();
    Q_ASSERT(manager != 0);

    m_initialized = true;
}

bool FilePlacesViewPlugin::isInitialized() const
{
    return m_initialized;
}

bool FilePlacesViewPlugin::isContainer() const
{
    return false;
}

QWidget *FilePlacesViewPlugin::createWidget(QWidget *parent)
{
    VFilePlacesView *view = new VFilePlacesView(parent);
    return view;
}

QString FilePlacesViewPlugin::name() const
{
    return QStringLiteral("VFilePlacesView");
}

QString FilePlacesViewPlugin::group() const
{
    return QStringLiteral("Vibe");
}

QIcon FilePlacesViewPlugin::icon() const
{
    return QIcon(":/qt.png");
}

QString FilePlacesViewPlugin::toolTip() const
{
    return tr("File Places View");
}

QString FilePlacesViewPlugin::whatsThis() const
{
    return QStringLiteral("");
}

QString FilePlacesViewPlugin::domXml() const
{
    return QStringLiteral(""
                          "<ui language=\"c++\">"
                          "    <widget class=\"VFilePlacesView\" name=\"filePlaces\">"
                          "        <property name=\"geometry\">"
                          "            <rect>"
                          "                <x>0</x>"
                          "                <y>0</y>"
                          "                <width>145</width>"
                          "                <height>280</height>"
                          "            </rect>"
                          "        </property>"
                          "    </widget>"
                          "</ui>");
}

QString FilePlacesViewPlugin::includeFile() const
{
    return QStringLiteral("VibeWidgets/VFilePlacesView");
}

#include "moc_fileplacesviewplugin.cpp"
