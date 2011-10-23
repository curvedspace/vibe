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

#ifndef QUBE_ABOUTDIALOG_P_H
#define QUBE_ABOUTDIALOG_P_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QUrl>

class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class QPushButton;
class QSpacerItem;

namespace Qube
{
    namespace Gui
    {
        class AboutDialog;

        class AboutDialogPrivate : public QObject
        {
            Q_OBJECT
        public:
            AboutDialogPrivate(AboutDialog *parent);

            QString licenseText() const {
                return m_licenseText;
            }
            void setLicenseText(const QString &text);

            QStringList authors() const {
                return m_authors;
            }
            void setAuthors(const QStringList &authors);

            QUrl link() const {
                return m_link;
            }
            void setLink(const QUrl &url);

            AboutDialog * const q_ptr;
            Q_DECLARE_PUBLIC(AboutDialog)

            QVBoxLayout *verticalLayout;
            QLabel *logoLabel;
            QLabel *appVerLabel;
            QLabel *descrLabel;
            QLabel *copyrightLabel;
            QLabel *websiteLabel;
            QSpacerItem *verticalSpacer;
            QHBoxLayout *horizontalLayout;
            QSpacerItem *horizontalSpacer;
            QPushButton *moreInfoButton;
            QPushButton *closeButton;

        private:
            QString m_licenseText;
            QStringList m_authors;
            QUrl m_link;

        private Q_SLOTS:
            void slotMoreInformation();

            void slotClose();
        };
    }
}

#endif // QUBE_ABOUTDIALOG_P_H
