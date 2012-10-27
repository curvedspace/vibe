/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPL2$
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include "varchivehandlerplugin.h"

/*!
   \class VArchiveHandlerPlugin
   \brief The VArchiveHandlerPlugin class provides an abstract base for custom
   VArchiveHandler plugins.

   \ingroup archives

   VArchiveHandlerPlugin is a simple plugin interface that makes it easy to
   create custom archive handlers that can be loaded dynamically into
   applications in order to provide transparent support for archives of
   all supported formats.

   Writing an archive handler plugin is archieved by subclassing this base class,
   reimplementing the pure virtual mimeTypes() and create() methods, and exporting
   the class using the Q_EXPORT_PLUGIN2() macro.

   \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>

   \sa VArchiveHandler
*/

/*!
    Constructs a VArchiveHandlerPlugin with the given \a parent object.

    \param parent The parent object.
*/
VArchiveHandlerPlugin::VArchiveHandlerPlugin(QObject *parent)
    : QObject(parent)
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
