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

#include "settingsserver.h"

SettingsServer::SettingsServer(QObject *parent)
    : QObject(parent)
{
    m_mutex = new QMutex();
    m_settings = new QSettings("QubeOS", "Desktop");
}

SettingsServer::~SettingsServer()
{
    delete m_settings;
    delete m_mutex;
}

QDBusVariant &SettingsServer::plainFont() const
{
    QDBusVariant v;

    m_mutex->lock();
    QFont font("Droid Sans", 10);
    m_settings->beginGroup("Fonts");
    v = QDBusVariant(QVariant(m_settings->value("Plain", QVariant(font))));
    m_mutex->unlock();

    return v;
}

void SettingsServer::setPlainFont(const QDBusVariant &font)
{
    m_mutex->lock();
    m_settings->beginGroup("Fonts");
    m_settings->setValue("Plain", font.variant());
    m_mutex->unlock();

    emit plainFontChanged(font);
}

QDBusVariant &SettingsServer::boldFont() const
{
    QDBusVariant v;

    m_mutex->lock();
    QFont font("Droid Sans", 10);
    m_settings->beginGroup("Fonts");
    v = QDBusVariant(QVariant(m_settings->value("Bold", QVariant(font))));
    m_mutex->unlock();

    return v;
}

void SettingsServer::setBoldFont(const QDBusVariant &font)
{
    m_mutex->lock();
    m_settings->beginGroup("Fonts");
    m_settings->setValue("Bold", font.variant());
    m_mutex->unlock();

    emit boldFontChanged(font);
}

QDBusVariant &SettingsServer::fixedSizeFont() const
{
    QDBusVariant v;

    m_mutex->lock();
    QFont font("Droid Sans Mono", 10);
    m_settings->beginGroup("Fonts");
    v = QDBusVariant(QVariant(m_settings->value("FixedSize", QVariant(font))));
    m_mutex->unlock();

    return v;
}

void SettingsServer::setFixedSizeFont(const QDBusVariant &font)
{
    m_mutex->lock();
    m_settings->beginGroup("Fonts");
    m_settings->setValue("FixedSize", font.variant());
    m_mutex->unlock();

    emit fixedSizeFontChanged(font);
}

QString &SettingsServer::style() const
{
    QString v;

    m_mutex->lock();
    m_settings->beginGroup("Appearance");
    v = m_settings->value("Style", QVariant("Qube")).toString();
    m_mutex->unlock();

    return v;
}

void SettingsServer::setStyle(const QString &style)
{
    m_mutex->lock();
    m_settings->beginGroup("Appearance");
    m_settings->setValue("Style", QVariant(style));
    m_mutex->unlock();

    emit styleChanged(style);
}

QString &SettingsServer::iconTheme() const
{
    QString v;

    m_mutex->lock();
    m_settings->beginGroup("Appearance");
    v = m_settings->value("IconTheme", QVariant("Qube")).toString();
    m_mutex->unlock();

    return v;
}

void SettingsServer::setIconTheme(const QString &iconTheme)
{
    m_mutex->lock();
    m_settings->beginGroup("Appearance");
    m_settings->setValue("IconTheme", QVariant(iconTheme));
    m_mutex->unlock();

    emit iconThemeChanged(iconTheme);
}

QString &SettingsServer::colorScheme() const
{
    QString v;

    m_mutex->lock();
    m_settings->beginGroup("Appearance");
    v = m_settings->value("ColorScheme", QVariant("Qube")).toString();
    m_mutex->unlock();

    return v;
}

void SettingsServer::setColorScheme(const QString &colorScheme)
{
    m_mutex->lock();
    m_settings->beginGroup("Appearance");
    m_settings->setValue("ColorScheme", QVariant(colorScheme));
    m_mutex->unlock();

    emit colorSchemeChanged(colorScheme);
}

int SettingsServer::toolBarIconSize() const
{
    int v;

    m_mutex->lock();
    m_settings->beginGroup("ToolBar");
    v = m_settings->value("IconSize", QVariant(24)).toInt();
    m_mutex->unlock();

    return v;
}

void SettingsServer::setToolBarIconSize(int size)
{
    m_mutex->lock();
    m_settings->beginGroup("ToolBar");
    m_settings->setValue("IconSize", QVariant(size));
    m_mutex->unlock();

    emit toolBarIconSizeChanged(size);
}

int SettingsServer::toolButtonStyle() const
{
    int v;

    m_mutex->lock();
    m_settings->beginGroup("ToolBar");

    QString val = m_settings->value("Style", QVariant("IconOnly")).toString().toLower();
    if (val == "icononly")
        v = Qt::ToolButtonIconOnly;
    else if (val == "textonly")
        v = Qt::ToolButtonTextOnly;
    else if (val == "textbesideicon")
        v = Qt::ToolButtonTextBesideIcon;
    else if (val == "textundericon")
        v = Qt::ToolButtonTextUnderIcon;

    m_mutex->unlock();

    return v;
}

void SettingsServer::setToolButtonStyle(int style)
{
    QVariant v;

    m_mutex->lock();
    m_settings->beginGroup("ToolBar");

    switch (style) {
        case Qt::ToolButtonTextOnly:
            v = QVariant("TextOnly");
            break;
        case Qt::ToolButtonTextBesideIcon:
            v = QVariant("TextBesideIcon");
            break;
        case Qt::ToolButtonTextUnderIcon:
            v = QVariant("TextUnderIcon");
            break;
        default:
            v = QVariant("IconOnly");
            break;
    }

    m_settings->setValue("Style", v);
    m_mutex->unlock();

    emit toolButtonStyleChanged(style);
}

#include "settingsserver.moc"
