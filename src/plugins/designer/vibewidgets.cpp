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

#include <QtCore/QtPlugin>

#include "vibewidgets.h"
#include "navbarplugin/navbarplugin.h"

VibeWidgets::VibeWidgets(QObject *parent)
    : QObject(parent)
{
    const QIcon icon(":/qt.png");
    m_plugins.append(new NavBarPlugin(icon, this));
}

QList<QDesignerCustomWidgetInterface *> VibeWidgets::customWidgets() const
{
    return m_plugins;
}

Q_EXPORT_PLUGIN2(VibeWidgetsPlugin, VibeWidgets)

#include "vibewidgets.moc"