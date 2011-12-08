/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2010-2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Vibe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Vibe.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QtCore/QEvent>
#include <QtCore/QFileInfo>
#include <QtDBus/QDBusConnection>
#include <QtGui/QApplication>
#include <QtGui/QDesktopServices>
#include <QtGui/QFont>
#include <QtGui/QIcon>
#include <QtGui/QToolBar>
#include <QtGui/QToolButton>

#include <VibeCore/VMimeType>
#include <VibeGui/VColorScheme>

#include <qconf.h>

#include "vibeplatform.h"

#ifdef Q_WS_X11
extern void qt_x11_apply_settings_in_all_apps();
#endif

VibePlatform::VibePlatform()
{
    m_conf = new QConf("org.vision.desktop", "/desktop/mirage");
    QDBusConnection connection = QDBusConnection::sessionBus();
    m_settings =
        new org::qubeos::Settings(
        "org.qubeos.Settings", "/", connection);

    connect(m_settings, SIGNAL(plainFontChanged(QString)),
            this, SLOT(updateFonts()));
    connect(m_settings, SIGNAL(fixedSizeFontChanged(QString)),
            this, SLOT(updateFonts()));
    connect(m_settings, SIGNAL(styleChanged(QString)),
            this, SLOT(updateWidgetStyle()));
    connect(m_settings, SIGNAL(iconThemeChanged(QString)),
            this, SLOT(updateIconTheme()));
    connect(m_settings, SIGNAL(colorSchemeChanged(QString)),
            this, SLOT(updateColorScheme()));
    connect(m_settings, SIGNAL(toolBarIconSizeChanged(int)),
            this, SLOT(updateToolBarIconSize()));
    connect(m_settings, SIGNAL(toolButtonStyleChanged(int)),
            this, SLOT(updateToolButtonStyle()));
}

VibePlatform::~VibePlatform()
{
    delete m_conf;
}

QStringList VibePlatform::keys() const
{
    return QStringList() << "default";
}

QString VibePlatform::styleName()
{
    return m_conf->style();
}

QPalette VibePlatform::palette()
{
    VColorScheme colorScheme(m_conf->colorScheme());
    return colorScheme.palette();
}

QString VibePlatform::systemIconThemeName()
{
    return m_conf->iconTheme();
}

QStringList VibePlatform::iconThemeSearchPaths()
{
    return QStringList()
           << "/usr/share/icons"
           << "/data/themes/icons"
           << QDesktopServices::storageLocation(QDesktopServices::HomeLocation) +
           "/.data/themes/icons";
}

QIcon VibePlatform::fileSystemIcon(const QFileInfo &info)
{
    VMimeType mime;
    mime.fromFileName(info.filePath());
    return QIcon::fromTheme(mime.iconName());
}

int VibePlatform::platformHint(PlatformHint hint)
{
    switch (hint) {
        case PH_ItemView_ActivateItemOnSingleClick:
            return 0;
        case PH_ToolBarIconSize:
            return m_conf->toolbarIconSize();
        case PH_ToolButtonStyle:
            return m_conf->toolbarStyle();
    }

    return QGuiPlatformPlugin::platformHint(hint);
}

void VibePlatform::updateFonts()
{
    QFont font;
    if (font.fromString(m_conf->fontName())) {
        QApplication::setFont(font);

#ifdef Q_WS_X11
        qt_x11_apply_settings_in_all_apps();
#endif
    }
}

void VibePlatform::updateWidgetStyle()
{
    QApplication::setStyle(styleName());

#ifdef Q_WS_X11
    qt_x11_apply_settings_in_all_apps();
#endif
}

void VibePlatform::updateIconTheme()
{
#if 0
    QWidgetList widgets = QApplication::allWidgets();
    foreach(QWidget * widget, widgets) {
        QEvent event(QEvent::StyleChange);
        QApplication::sendEvent(widget, &event);
    }
#else
    QIcon::setThemeName(systemIconThemeName());
#endif

#ifdef Q_WS_X11
    qt_x11_apply_settings_in_all_apps();
#endif
}

void VibePlatform::updateColorScheme()
{
    QWidgetList widgets = QApplication::allWidgets();
    foreach(QWidget * widget, widgets) {
        QEvent event(QEvent::PaletteChange);
        QApplication::sendEvent(widget, &event);
    }

#ifdef Q_WS_X11
    qt_x11_apply_settings_in_all_apps();
#endif
}

void VibePlatform::updateToolBarIconSize()
{
    QWidgetList widgets = QApplication::allWidgets();
    foreach(QWidget * widget, widgets) {
        if (qobject_cast<QToolBar *>(widget)) {
            QEvent event(QEvent::ToolBarChange);
            QApplication::sendEvent(widget, &event);
        }
    }

#ifdef Q_WS_X11
    qt_x11_apply_settings_in_all_apps();
#endif
}

void VibePlatform::updateToolButtonStyle()
{
    QWidgetList widgets = QApplication::allWidgets();
    foreach(QWidget * widget, widgets) {
        if (qobject_cast<QToolButton *>(widget)) {
            QEvent event(QEvent::StyleChange);
            QApplication::sendEvent(widget, &event);
        }
    }

#ifdef Q_WS_X11
    qt_x11_apply_settings_in_all_apps();
#endif
}

Q_EXPORT_PLUGIN2(VibePlatform, VibePlatform)

#include "vibeplatform.moc"
