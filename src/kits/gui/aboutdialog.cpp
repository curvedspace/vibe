/****************************************************************************
 * This file is part of Qube.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Qube is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Qube is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Qube.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QtGui/QApplication>
#include <QtGui/QLabel>

#include "aboutdialog.h"
#include "aboutdialog_p.h"

namespace Qube
{
    namespace Gui
    {
        AboutDialog::AboutDialog(QWidget *parent) :
            QDialog(parent),
            d_ptr(new AboutDialogPrivate(this))
        {
            setWindowTitle(tr("About"));
            setSizeGripEnabled(false);
        }

        AboutDialog::~AboutDialog()
        {
            delete d_ptr;
        }

        QPixmap *AboutDialog::logo() const
        {
            Q_D(const AboutDialog);
            return const_cast<QPixmap *>(d->logoLabel->pixmap());
        }

        void AboutDialog::setLogo(const QPixmap &pixmap)
        {
            Q_D(AboutDialog);
            d->logoLabel->setPixmap(pixmap);
        }

        QString AboutDialog::description() const
        {
            Q_D(const AboutDialog);
            return d->descrLabel->text();
        }

        void AboutDialog::setDescription(const QString &descr)
        {
            Q_D(AboutDialog);
            d->descrLabel->setText(descr);
        }

        QString AboutDialog::copyright() const
        {
            Q_D(const AboutDialog);
            return d->copyrightLabel->text();
        }

        void AboutDialog::setCopyright(const QString &text)
        {
            Q_D(AboutDialog);
            d->copyrightLabel->setText(text);
        }

        QString AboutDialog::licenseText() const
        {
            Q_D(const AboutDialog);
            return d->licenseText();
        }

        void AboutDialog::setLicenseText(const QString &text)
        {
            Q_D(AboutDialog);
            d->setLicenseText(text);
        }

        QStringList AboutDialog::authors() const
        {
            Q_D(const AboutDialog);
            return d->authors();
        }

        void AboutDialog::setAuthors(const QStringList &authors)
        {
            Q_D(AboutDialog);
            d->setAuthors(authors);
        }

        QUrl AboutDialog::link() const
        {
            Q_D(const AboutDialog);
            return d->link();
        }

        void AboutDialog::setLink(const QUrl &url)
        {
            Q_D(AboutDialog);
            d->setLink(url);
        }
    }
}
