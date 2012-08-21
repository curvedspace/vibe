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

#include <QDesignerCustomWidgetCollectionInterface>

#include "fileplacesviewplugin.h"
#include "lineeditplugin.h"
#include "navigationbarplugin.h"
#include "titlewidgetplugin.h"

class VibeWidgets : public QObject, QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetCollectionInterface" FILE "vibewidgets.json")
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)
public:
    explicit VibeWidgets(QObject *parent = 0) {
        //m_plugins.append(new FilePlacesViewPlugin(icon, this));
        m_plugins.append(new LineEditPlugin(this));
        m_plugins.append(new NavigationBarPlugin(this));
        m_plugins.append(new TitleWidgetPlugin(this));
    }

    virtual QList<QDesignerCustomWidgetInterface *> customWidgets() const {
        return m_plugins;
    }

private:
    QList<QDesignerCustomWidgetInterface *> m_plugins;
};

#include "main.moc"