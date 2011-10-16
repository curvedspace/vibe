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

#include <QtGui/QApplication>

#include "navbar.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QubeGui::NavBar *navbar = new QubeGui::NavBar();
    QubeGui::NavBarGroup *group = navbar->addGroup("APPLICATIONS");
    group->addItem(QPixmap(":/icons/allApps.png"), "All Applications");
    group->addItem(QPixmap(":/icons/updates.png"), "Updates");
    group->addItem(QPixmap(":/icons/upToDate.png"), "Up To Date");
    group->addItem(QPixmap(":/icons/unknown.png"), "Unknown");
    group = navbar->addGroup("BY KIND");
    group->setStatic(true);
    group->addItem(QPixmap(":/icons/apps.png"), "Applications");
    group->addItem(QPixmap(":/icons/plugins.png"), "Plugins");
    group->addItem(QPixmap(":/icons/widgets.png"), "Widgets");
    group->addItem(QPixmap(":/icons/prefApp.png"), "Preference Panes");
    group->addItem(QPixmap(":/icons/systemUpdates.png"), "System Updates");
    group = navbar->addGroup("VENDORS");
    group->addItem(QPixmap(":/icons/apple.png"), "Apple");
    navbar->show();

    return app.exec();
}
