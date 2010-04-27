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

#include "settingsserver.h"
#include "settingsadaptor.h"

int
main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    SettingsServer *server = new SettingsServer();
    (void)new SettingsAdaptor(server);

    QDBusConnection connection = QDBusConnection::sessionBus();
    if (connection.registerService("org.qubeos.Settings"))
        (void)connection.registerObject("/", server);

    return app.exec();
}