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

#ifndef QUBE_ABOUTDIALOG_H
#define QUBE_ABOUTDIALOG_H

#include <QtCore/QUrl>
#include <QtGui/QDialog>

namespace Qube
{
    namespace Gui
    {
        class AboutDialogPrivate;

        class AboutDialog : public QDialog
        {
            Q_OBJECT
            Q_DECLARE_PRIVATE(AboutDialog)
            Q_PROPERTY(QPixmap logo READ logo WRITE setLogo)
            Q_PROPERTY(QString description READ description WRITE setDescription)
            Q_PROPERTY(QString copyright READ copyright WRITE setCopyright)
            Q_PROPERTY(QString license READ licenseText WRITE setLicenseText)
            Q_PROPERTY(QStringList authors READ authors WRITE setAuthors)
            Q_PROPERTY(QUrl link READ link WRITE setLink)
        public:
            explicit AboutDialog(QWidget *parent = 0);
            virtual ~AboutDialog();

            QPixmap *logo() const;
            void setLogo(const QPixmap &pixmap);

            QString description() const;
            void setDescription(const QString &descr);

            QString copyright() const;
            void setCopyright(const QString &text);

            QString licenseText() const;
            void setLicenseText(const QString &text);

            QStringList authors() const;
            void setAuthors(const QStringList &authors);

            QUrl link() const;
            void setLink(const QUrl &url);

        protected:
            AboutDialogPrivate * const d_ptr;
        };
    }
}

#endif // QUBE_ABOUTDIALOG_H
