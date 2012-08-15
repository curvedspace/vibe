/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
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

#include <VibeWidgets/VLineEdit>

#include "lineeditplugin.h"

LineEditPlugin::LineEditPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void LineEditPlugin::initialize(QDesignerFormEditorInterface *formEditor)
{
    if (m_initialized)
        return;

    QExtensionManager *manager = formEditor->extensionManager();
    Q_ASSERT(manager != 0);

    m_initialized = true;
}

bool LineEditPlugin::isInitialized() const
{
    return m_initialized;
}

bool LineEditPlugin::isContainer() const
{
    return false;
}

QWidget *LineEditPlugin::createWidget(QWidget *parent)
{
    VLineEdit *widget = new VLineEdit(parent);
    return widget;
}

QString LineEditPlugin::name() const
{
    return QStringLiteral("VLineEdit");
}

QString LineEditPlugin::group() const
{
    return QStringLiteral("Vibe");
}

QIcon LineEditPlugin::icon() const
{
    return QIcon(":/qt.png");
}

QString LineEditPlugin::toolTip() const
{
    return tr("Line Edit");
}

QString LineEditPlugin::whatsThis() const
{
    return QStringLiteral("");
}

QString LineEditPlugin::domXml() const
{
    return QStringLiteral(""
                          "<ui language=\"c++\">"
                          "    <widget class=\"VLineEdit\" name=\"lineEdit\">"
                          "        <property name=\"geometry\">"
                          "            <rect>"
                          "                <x>0</x>"
                          "                <y>0</y>"
                          "                <width>100</width>"
                          "                <height>20</height>"
                          "            </rect>"
                          "        </property>"
                          "    </widget>"
                          "</ui>");
}

QString LineEditPlugin::includeFile() const
{
    return QStringLiteral("VibeWidgets/VLineEdit");
}

#include "moc_lineeditplugin.cpp"
