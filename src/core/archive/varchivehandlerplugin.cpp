/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPL-ONLY$
 *
 * This file may be used under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation and
 * appearing in the file LICENSE.LGPL included in the packaging of
 * this file, either version 2.1 of the License, or (at your option) any
 * later version.  Please review the following information to ensure the
 * GNU Lesser General Public License version 2.1 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 * If you have questions regarding the use of this file, please contact
 * us via http://www.maui-project.org/.
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
