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

#ifndef SETTINGSSERVER_H
#define SETTINGSSERVER_H

#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtCore/QSettings>
#include <QtDBus/QDBusVariant>
#include <QtGui/QFont>

class SettingsServer : public QObject
{
    Q_OBJECT
public:
    explicit SettingsServer(QObject *parent = 0);
    ~SettingsServer();

signals:
    void plainFontChanged(const QDBusVariant &font);
    void boldFontChanged(const QDBusVariant &font);
    void fixedSizeFontChanged(const QDBusVariant &font);
    void styleChanged(const QString &style);
    void iconThemeChanged(const QString &iconTheme);
    void colorSchemeChanged(const QString &colorScheme);
    void toolBarIconSizeChanged(int size);
    void toolButtonStyleChanged(int style);

public slots:
    QDBusVariant &plainFont() const;
    void setPlainFont(const QDBusVariant &font);

    QDBusVariant &boldFont() const;
    void setBoldFont(const QDBusVariant &font);

    QDBusVariant &fixedSizeFont() const;
    void setFixedSizeFont(const QDBusVariant &font);

    QString &style() const;
    void setStyle(const QString &style);

    QString &iconTheme() const;
    void setIconTheme(const QString &iconTheme);

    QString &colorScheme() const;
    void setColorScheme(const QString &colorScheme);

    int toolBarIconSize() const;
    void setToolBarIconSize(int size);

    int toolButtonStyle() const;
    void setToolButtonStyle(int style);

private:
    QMutex *m_mutex;
    QSettings *m_settings;
};

#endif // SETTINGSSERVER_H
