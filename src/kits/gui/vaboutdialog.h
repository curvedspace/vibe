/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
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

#ifndef VABOUTDIALOG_H
#define VABOUTDIALOG_H

#include <QtCore/QUrl>
#include <QtGui/QDialog>

#include <VibeCore/VGlobal>

class VIBE_EXPORT VAboutDialog : public QDialog
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
