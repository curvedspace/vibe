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

#include <QtCore/QtPlugin>

#include "vibewidgets.h"
#include "fileplacesviewplugin.h"
#include "navigationbarplugin.h"
#include "titlewidgetplugin.h"

VibeWidgets::VibeWidgets(QObject *parent)
    : QObject(parent)
{
    const QIcon icon(":/qt.png");
    m_plugins.append(new FilePlacesViewPlugin(icon, this));
    m_plugins.append(new NavigationBarPlugin(icon, this));
    m_plugins.append(new TitleWidgetPlugin(icon, this));
}

QList<QDesignerCustomWidgetInterface *> VibeWidgets::customWidgets() const
{
    return m_plugins;
}

Q_EXPORT_PLUGIN2(VibeWidgetsPlugin, VibeWidgets)

#include "vibewidgets.moc"
