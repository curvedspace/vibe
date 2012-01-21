/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef VARCHIVEHANDLERPLUGIN_H
#define VARCHIVEHANDLERPLUGIN_H

#include <QObject>

#include <VibeCore/VGlobal>

/** \addtogroup core Core Kit
 *  @{
 */

class VArchiveHandler;

/**
 * \class VArchiveHandlerPlugin <varchivehandlerplugin.h> <VArchiveHandlerPlugin>
 *
 * \brief The VArchiveHandlerPlugin class provides an abstract base for custom
 * VArchiveHandler plugins.
 *
 * VArchiveHandlerPlugin is a simple plugin interface that makes it easy to
 * create custom archive handlers that can be loaded dynamically into
 * applications in order to provide transparent support for archives of
 * all supported formats.
 *
 * Writing an archive handler plugin is archieved by subclassing this base class,
 * reimplementing the pure virtual mimeTypes() and create() methods, and exporting
 * the class using the Q_EXPORT_PLUGIN2() macro.
 *
 * \sa VArchiveHandler
 *
 * \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VIBE_EXPORT VArchiveHandlerPlugin
{
public:
    /**
     * Destroys the archive handler plugin.
     *
     * Plugins are destroyed automatically when they are no longer used, so
     * there is no need for calling the destructor explicitly.
     */
    virtual ~VArchiveHandlerPlugin() {}

    /**
     * Returns the list of MIME Types this plugin supports.
     *
     * \sa create()
     */
    virtual QStringList mimeTypes() const = 0;

    /**
     * Creates and returns a VArchiveHandler object for the given MIME Type.
     * If a plugin cannot create an archive handler, it should return 0 instead.
     *
     * \param key the MIME Type.
     *
     * \sa keys()
     */
    virtual VArchiveHandler *create(const QString &mimeType) = 0;
};

Q_DECLARE_INTERFACE(VArchiveHandlerPlugin,
                    "org.vision.Vibe.VArchiveHandlerInterface/1.0")

/** @}*/

#endif // VARCHIVEHANDLERPLUGIN_H
