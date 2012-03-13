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

#include "varchivehandlerplugin.h"

/*!
   \class VArchiveHandlerPlugin
   \ingroup core Vibe Core

   \brief The VArchiveHandlerPlugin class provides an abstract base for custom
   VArchiveHandler plugins.

   VArchiveHandlerPlugin is a simple plugin interface that makes it easy to
   create custom archive handlers that can be loaded dynamically into
   applications in order to provide transparent support for archives of
   all supported formats.

   Writing an archive handler plugin is archieved by subclassing this base class,
   reimplementing the pure virtual mimeTypes() and create() methods, and exporting
   the class using the Q_EXPORT_PLUGIN2() macro.

   \sa VArchiveHandler

   \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
*/

/*!
    Constructor.
*/
VArchiveHandlerPlugin::VArchiveHandlerPlugin(QObject *parent) :
    QObject(parent)
{
}

/*!
    Destroys the archive handler plugin.

    Plugins are destroyed automatically when they are no longer used, so
    there is no need for calling the destructor explicitly.
*/
VArchiveHandlerPlugin::~VArchiveHandlerPlugin()
{
}

#include "moc_varchivehandlerplugin.cpp"
