/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

#ifndef VABOUTDIALOG_P_H
#define VABOUTDIALOG_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Vibe API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

class VIBE_EXPORT VAboutDialogPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(VAboutDialog)
public:
    VAboutDialogPrivate(VAboutDialog *q);

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

    VAboutDialog *const q_ptr;

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

private slots:
    void slotMoreInformation();
    void slotClose();
};

#endif // VABOUTDIALOG_P_H
