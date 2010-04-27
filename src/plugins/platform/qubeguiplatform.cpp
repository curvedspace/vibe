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

#include <QtCore/QEvent>
#include <QtCore/QFileInfo>
#include <QtDBus/QDBusConnection>
#include <QtGui/QApplication>
#include <QtGui/QDesktopServices>
#include <QtGui/QFont>
#include <QtGui/QIcon>
#include <QtGui/QToolBar>
#include <QtGui/QToolButton>

#include "qubeguiplatform.h"
#include "colorscheme.h"
#include "mimetype.h"

#ifdef Q_WS_X11
extern void qt_x11_apply_settings_in_all_apps();
#endif

QubeGuiPlatform::QubeGuiPlatform()
{
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

QubeGuiPlatform::~QubeGuiPlatform()
{
    delete m_settings;
}

QStringList QubeGuiPlatform::keys() const
{
    return QStringList() << "default";
}

QString QubeGuiPlatform::styleName()
{
    return m_settings->style();
}

QPalette QubeGuiPlatform::palette()
{
    QubeGui::ColorScheme colorScheme(
        m_settings->colorScheme());
    return colorScheme.palette();
}

QString QubeGuiPlatform::systemIconThemeName()
{
    return m_settings->iconTheme();
}

QStringList QubeGuiPlatform::iconThemeSearchPaths()
{
    return QStringList()
            << "/usr/share/icons" << "/data/themes/icons"
            << QDesktopServices::storageLocation(QDesktopServices::HomeLocation) +
               "/.data/themes/icons";
}

QIcon QubeGuiPlatform::fileSystemIcon(const QFileInfo &info)
{
    QubeCore::MimeType mime;
    mime.fromFileName(info.filePath());
    return QIcon::fromTheme(mime.iconName());
}

int QubeGuiPlatform::platformHint(PlatformHint hint)
{
    switch (hint) {
        case PH_ItemView_ActivateItemOnSingleClick:
            return 0;
        case PH_ToolBarIconSize:
            return m_settings->toolBarIconSize();
        case PH_ToolButtonStyle:
            return m_settings->toolButtonStyle();
    }

    return QGuiPlatformPlugin::platformHint(hint);
}

void QubeGuiPlatform::updateFonts()
{
    QFont font;
    if (font.fromString(m_settings->plainFont())) {
        QApplication::setFont(font);

#ifdef Q_WS_X11
        qt_x11_apply_settings_in_all_apps();
#endif
    }
}

void QubeGuiPlatform::updateWidgetStyle()
{
    QApplication::setStyle(styleName());

#ifdef Q_WS_X11
    qt_x11_apply_settings_in_all_apps();
#endif
}

void QubeGuiPlatform::updateIconTheme()
{
#if 0
    QWidgetList widgets = QApplication::allWidgets();
    foreach(QWidget *widget, widgets) {
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

void QubeGuiPlatform::updateColorScheme()
{
    QWidgetList widgets = QApplication::allWidgets();
    foreach(QWidget *widget, widgets) {
        QEvent event(QEvent::PaletteChange);
        QApplication::sendEvent(widget, &event);
    }

#ifdef Q_WS_X11
    qt_x11_apply_settings_in_all_apps();
#endif
}

void QubeGuiPlatform::updateToolBarIconSize()
{
    QWidgetList widgets = QApplication::allWidgets();
    foreach(QWidget *widget, widgets) {
        if (qobject_cast<QToolBar *>(widget)) {
            QEvent event(QEvent::ToolBarChange);
            QApplication::sendEvent(widget, &event);
        }
    }

#ifdef Q_WS_X11
    qt_x11_apply_settings_in_all_apps();
#endif
}

void QubeGuiPlatform::updateToolButtonStyle()
{
    QWidgetList widgets = QApplication::allWidgets();
    foreach(QWidget *widget, widgets) {
        if (qobject_cast<QToolButton *>(widget)) {
            QEvent event(QEvent::StyleChange);
            QApplication::sendEvent(widget, &event);
        }
    }

#ifdef Q_WS_X11
    qt_x11_apply_settings_in_all_apps();
#endif
}

Q_EXPORT_PLUGIN2(QubeGuiPlatform, QubeGuiPlatform)

#include "qubeguiplatform.moc"
