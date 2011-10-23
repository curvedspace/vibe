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

#include <Qube/Gui/AboutDialog>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Test Application");
    app.setApplicationVersion("42.0");

    QStringList authors;
    authors << QString("John Doe") << QString("Walter White") << QString("Jesse Pinkman");

    Qube::Gui::AboutDialog *dialog = new Qube::Gui::AboutDialog();
    dialog->setDescription("This is just a test application.");
    dialog->setCopyright("Copyright (C) 2010 John Doe\nCopyright (C) 2011 Walter White\nCopyright (C) 2011 Jesse Pinkman");
    dialog->setLogo(QPixmap(":/icons/app_icon.png"));
    dialog->setAuthors(authors);
    dialog->setLicenseText("License text goes here.");
    dialog->setLink(QUrl("http://www.google.com/"));
    dialog->show();

    return app.exec();
}
