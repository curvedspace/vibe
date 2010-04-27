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
#include <QtGui/QIcon>
#include <QtGui/QStyleFactory>

#include "settingsserver.h"

const QFont defaultPlainFont("Droid Sans", 10);
const QFont defaultFixedFont("Droid Sans Mono", 10);
const QString defaultStyle("Qube");
const QString defaultIconTheme("Qube");
const QString defaultColorScheme("Qube");
const int defaultToolBarIconSize = 24;
const QString defaultToolButtonStyle("icononly");

SettingsServer::SettingsServer(QObject *parent)
    : QObject(parent)
{
    m_mutex = new QMutex();
}

SettingsServer::~SettingsServer()
{
    delete m_mutex;
}

QString SettingsServer::plainFont() const
{
    m_mutex->lock();
    QSettings settings(QLatin1String("Trolltech"));
    settings.beginGroup(QLatin1String("Qt"));
    QString v = settings.value(QLatin1String("font"),
                               defaultPlainFont.toString()).toString();
    m_mutex->unlock();

    return v;
}

void SettingsServer::setPlainFont(const QString &font)
{
    m_mutex->lock();

    QFont f;
    if (f.fromString(font)) {
        QSettings settings(QLatin1String("Trolltech"));
        settings.beginGroup(QLatin1String("Qt"));
        settings.setValue(QLatin1String("font"), font);

        emit plainFontChanged(font);
    }

    m_mutex->unlock();
}

QString SettingsServer::fixedSizeFont() const
{
    m_mutex->lock();
    QSettings settings(QLatin1String("Trolltech"));
    settings.beginGroup(QLatin1String("Qt"));
    QString v = settings.value(QLatin1String("Qube/fixedFont"),
                               defaultFixedFont.toString()).toString();
    m_mutex->unlock();

    return v;
}

void SettingsServer::setFixedSizeFont(const QString &font)
{
    m_mutex->lock();

    QFont f;
    if (f.fromString(font)) {
        QSettings settings(QLatin1String("Trolltech"));
        settings.beginGroup(QLatin1String("Qt"));
        settings.setValue(QLatin1String("Qube/fixedFont"), font);

        emit fixedSizeFontChanged(font);
    }

    m_mutex->unlock();
}

QString SettingsServer::style() const
{
    m_mutex->lock();
    QSettings settings(QLatin1String("Trolltech"));
    settings.beginGroup(QLatin1String("Qt"));
    QString v = settings.value(QLatin1String("style"),
                               defaultStyle).toString();
    m_mutex->unlock();

    return v;
}

void SettingsServer::setStyle(const QString &style)
{
    m_mutex->lock();

    if (QStyleFactory::keys().contains(style)) {
        QSettings settings(QLatin1String("Trolltech"));
        settings.beginGroup(QLatin1String("Qt"));
        settings.setValue(QLatin1String("style"), style);

        emit styleChanged(style);
    }

    m_mutex->unlock();
}

QString SettingsServer::iconTheme() const
{
    m_mutex->lock();
    QSettings settings(QLatin1String("Trolltech"));
    settings.beginGroup(QLatin1String("Qt"));
    QString v = settings.value(QLatin1String("Qube/iconTheme"),
                               defaultIconTheme).toString();
    m_mutex->unlock();

    return v;
}

void SettingsServer::setIconTheme(const QString &iconTheme)
{
    m_mutex->lock();

    if (QIcon::hasThemeIcon(iconTheme)) {
        QSettings settings(QLatin1String("Trolltech"));
        settings.beginGroup(QLatin1String("Qt"));
        settings.setValue(QLatin1String("Qube/iconTheme"), iconTheme);

        emit iconThemeChanged(iconTheme);
    }

    m_mutex->unlock();
}

QString SettingsServer::colorScheme() const
{
    m_mutex->lock();
    QSettings settings(QLatin1String("Trolltech"));
    settings.beginGroup(QLatin1String("Qt"));
    QString v = settings.value(QLatin1String("Qube/colorScheme"),
                               defaultColorScheme).toString();
    m_mutex->unlock();

    return v;
}

void SettingsServer::setColorScheme(const QString &colorScheme)
{
    m_mutex->lock();
    QSettings settings(QLatin1String("Trolltech"));
    settings.beginGroup(QLatin1String("Qt"));
    settings.setValue(QLatin1String("Qube/colorScheme"), colorScheme);
    m_mutex->unlock();

    emit colorSchemeChanged(colorScheme);
}

int SettingsServer::toolBarIconSize() const
{
    m_mutex->lock();
    QSettings settings(QLatin1String("Trolltech"));
    settings.beginGroup(QLatin1String("Qt"));
    int v = settings.value(QLatin1String("Qube/toolBarIconSize"),
                           QVariant(defaultToolBarIconSize)).toInt();
    m_mutex->unlock();

    return v;
}

void SettingsServer::setToolBarIconSize(int size)
{
    m_mutex->lock();
    QSettings settings(QLatin1String("Trolltech"));
    settings.beginGroup(QLatin1String("Qt"));
    settings.setValue(QLatin1String("Qube/toolBarIconSize"), QVariant(size));
    m_mutex->unlock();

    emit toolBarIconSizeChanged(size);
}

int SettingsServer::toolButtonStyle() const
{
    int v;

    m_mutex->lock();
    QSettings settings(QLatin1String("Trolltech"));
    settings.beginGroup(QLatin1String("Qt"));
    QString val = settings.value(QLatin1String("Qube/toolButtonStyle"),
                                 defaultToolButtonStyle).toString();
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
            style = Qt::ToolButtonIconOnly;
            break;
    }

    m_mutex->lock();
    QSettings settings(QLatin1String("Trolltech"));
    settings.beginGroup(QLatin1String("Qt"));
    settings.setValue(QLatin1String("Qube/toolButtonStyle"), v);
    m_mutex->unlock();

    emit toolButtonStyleChanged(style);
}

#include "settingsserver.moc"
