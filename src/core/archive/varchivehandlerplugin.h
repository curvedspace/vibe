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

#ifndef VARCHIVEHANDLERPLUGIN_H
#define VARCHIVEHANDLERPLUGIN_H

#include <QtCore/qplugin.h>

#include <VibeCore/VibeCoreExport>

class VArchiveHandler;

struct VIBECORE_EXPORT VArchiveHandlerFactoryInterface {
    virtual VArchiveHandler *create(const QString &mimeType) = 0;
    virtual QStringList mimeTypes() const = 0;
};

#define VArchiveHandlerFactoryInterface_iid "org.hawaii.Vibe.VArchiveHandlerFactoryInterface"

Q_DECLARE_INTERFACE(VArchiveHandlerFactoryInterface, VArchiveHandlerFactoryInterface_iid)

class VIBECORE_EXPORT VArchiveHandlerPlugin : public QObject, public VArchiveHandlerFactoryInterface
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

        \param mimeType the MIME Type.

        \sa keys()
    */
    virtual VArchiveHandler *create(const QString &mimeType) = 0;
};

#endif // VARCHIVEHANDLERPLUGIN_H
