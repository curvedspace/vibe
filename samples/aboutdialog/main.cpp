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

#include <QubeGui/AboutDialog>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Test Application");
    app.setApplicationVersion("42.0");

    QubeGui::AboutDialog *dialog = new QubeGui::AboutDialog();
    dialog->setDescription("This is just a test application.");
    dialog->setCopyright("Copyright (C) 2011 Pier Luigi Fiorini");
    dialog->setLogo(QPixmap(":/icons/app_icon.png"));
    dialog->show();

    return app.exec();
}
