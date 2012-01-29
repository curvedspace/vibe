/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * Vibe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Vibe.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "fluidplugin.h"
#include "desktopitem.h"
#include "iconimageprovider.h"
#include "styleitem.h"
#include "tooltiparea.h"

void FluidPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<StyleItem>(uri, 0, 1, "StyleItem");
    qmlRegisterType<TooltipArea>(uri, 0, 1, "TooltipArea");

    qmlRegisterUncreatableType<DesktopItem>("QtQuick", 2, 0, "Desktop",
                                            QLatin1String("Do not create objects of type Desktop"));
}

void FluidPlugin::initializeEngine(QDeclarativeEngine *engine, const char *uri)
{
    Q_UNUSED(uri);
    engine->addImageProvider("icon", new IconImageProvider);
}

Q_EXPORT_PLUGIN2(vibefluidplugin, FluidPlugin);

#include "moc_fluidplugin.cpp"
