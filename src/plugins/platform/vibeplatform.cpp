/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2010-2012 Pier Luigi Fiorini
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

#include <QDebug>
#include <QEvent>
#include <QFileInfo>
#include <QApplication>
#include <QDesktopServices>
#include <QFont>
#include <QIcon>
#include <QToolBar>
#include <QToolButton>

#include <VibeCore/VMimeType>
#include <VibeCore/VStandardDirectories>
#include <VibeGui/VColorScheme>

#include "vibeplatform.h"

using namespace VStandardDirectories;

VibePlatform::VibePlatform()
{
#if 0
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

    //else
    updateFonts();
    updateWidgetStyle();
    updateIconTheme();
    updateColorScheme();
    updateToolBarIconSize();
    updateToolButtonStyle();
#endif
}

VibePlatform::~VibePlatform()
{
}

QStringList VibePlatform::keys() const
{
    return QStringList(QStringLiteral("vibe"));
}

QString VibePlatform::styleName()
{
    return QString::fromLatin1("Vanish");
}

QPalette VibePlatform::palette()
{
    /*
    QString fileName(m_conf->property("color-scheme").toString());
    VColorScheme colorScheme("/usr/local/share/color-schemes/" + fileName);
    return colorScheme.palette();
    */
    return QGuiPlatformPlugin::palette();
}

QString VibePlatform::systemIconThemeName()
{
    //return m_conf->property("icon-theme").toString();
    return QString::fromLatin1("KFaenza");
}

QStringList VibePlatform::iconThemeSearchPaths()
{
    return QStringList() << findDirectory(UserThemesDirectory) + "/icons"
        << findDirectory(CommonThemesDirectory) + "/icons"
        << findDirectory(SystemThemesDirectory) + "/icons";
}

QIcon VibePlatform::fileSystemIcon(const QFileInfo &info)
{
#if 0
    VMimeType mime;
    mime.fromFileName(info.filePath());
    return QIcon::fromTheme(mime.iconName());
#endif
    return QIcon::fromTheme("dialog-error");
}

int VibePlatform::platformHint(PlatformHint hint)
{
    switch (hint) {
        case PH_ItemView_ActivateItemOnSingleClick:
            return 0;
        case PH_ToolBarIconSize:
            //return m_conf->property("toolbar-icon-size").toInt();
            return 16;
        case PH_ToolButtonStyle:
            //return m_conf->property("toolbar-style").toInt();
            return 1;
    }

    return QGuiPlatformPlugin::platformHint(hint);
}

void VibePlatform::updateFonts()
{
#if 0
    QFont font;
    if (font.fromString(m_conf->property("font-name").toString()))
        QApplication::setFont(font);
#endif
}

void VibePlatform::updateWidgetStyle()
{
    QApplication::setStyle(styleName());
}

void VibePlatform::updateIconTheme()
{
#if 0
    QWidgetList widgets = QApplication::allWidgets();
    foreach(QWidget * widget, widgets) {
        QEvent event(QEvent::StyleChange);
        QApplication::sendEvent(widget, &event);
    }
    //else
    QIcon::setThemeName(systemIconThemeName());
#endif
}

void VibePlatform::updateColorScheme()
{
    QWidgetList widgets = QApplication::allWidgets();
    foreach(QWidget * widget, widgets) {
        QEvent event(QEvent::PaletteChange);
        QApplication::sendEvent(widget, &event);
    }
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
}

Q_EXPORT_PLUGIN2(vibe, VibePlatform)

#include "moc_vibeplatform.cpp"
