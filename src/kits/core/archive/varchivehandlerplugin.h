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

#ifndef VARCHIVEHANDLERPLUGIN_H
#define VARCHIVEHANDLERPLUGIN_H

#include <QtCore/qplugin.h>

#include <VibeCore/VGlobal>

class VArchiveHandler;

struct VIBE_EXPORT VArchiveHandlerFactoryInterface {
    virtual VArchiveHandler *create(const QString &mimeType) = 0;
    virtual QStringList mimeTypes() const = 0;
};

#define VArchiveHandlerFactoryInterface_iid "org.vision-os.Vibe.VArchiveHandlerFactoryInterface"

Q_DECLARE_INTERFACE(VArchiveHandlerFactoryInterface, VArchiveHandlerFactoryInterface_iid)

class VIBE_EXPORT VArchiveHandlerPlugin : public QObject, public VArchiveHandlerFactoryInterface
{
    Q_OBJECT
    Q_INTERFACES(VArchiveHandlerFactoryInterface)
public:
    explicit VArchiveHandlerPlugin(QObject *parent = 0);

    virtual ~VArchiveHandlerPlugin();

    /*!
        Returns the list of MIME Types this plugin supports.

        \sa create()
    */
    virtual QStringList mimeTypes() const = 0;

    /*!
        Creates and returns a VArchiveHandler object for the given MIME Type.
        If a plugin cannot create an archive handler, it should return 0 instead.

        \param key the MIME Type.

        \sa keys()
    */
    virtual VArchiveHandler *create(const QString &mimeType) = 0;
};

#endif // VARCHIVEHANDLERPLUGIN_H
