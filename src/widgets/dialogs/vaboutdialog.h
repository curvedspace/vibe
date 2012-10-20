/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
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

#ifndef VABOUTDIALOG_H
#define VABOUTDIALOG_H

#include <QUrl>
#include <QDialog>

#include <VibeCore/VGlobal>

class VIBEWIDGETS_EXPORT VAboutDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QPixmap logo READ logo WRITE setLogo)
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(QString copyright READ copyright WRITE setCopyright)
    Q_PROPERTY(QString license READ licenseText WRITE setLicenseText)
    Q_PROPERTY(QStringList authors READ authors WRITE setAuthors)
    Q_PROPERTY(QUrl link READ link WRITE setLink)
public:
    explicit VAboutDialog(QWidget *parent = 0);
    virtual ~VAboutDialog();

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

private:
    Q_PRIVATE_SLOT(d, void _q_moreInformation())
    Q_PRIVATE_SLOT(d, void _q_close())

    class Private;
    Private *const d;
    friend class Private;
};

#endif // VABOUTDIALOG_H
