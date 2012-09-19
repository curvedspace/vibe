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

#ifndef VARCHIVEHANDLERPLUGIN_H
#define VARCHIVEHANDLERPLUGIN_H

#include <QtCore/qplugin.h>

#include <VibeCore/VGlobal>

class VArchiveHandler;

struct VIBE_EXPORT VArchiveHandlerFactoryInterface {
    virtual VArchiveHandler *create(const QString &mimeType) = 0;
    virtual QStringList mimeTypes() const = 0;
};

#define VArchiveHandlerFactoryInterface_iid "org.hawaii.Vibe.VArchiveHandlerFactoryInterface"

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

        \param mimeType the MIME Type.

        \sa keys()
    */
    virtual VArchiveHandler *create(const QString &mimeType) = 0;
};

#endif // VARCHIVEHANDLERPLUGIN_H
