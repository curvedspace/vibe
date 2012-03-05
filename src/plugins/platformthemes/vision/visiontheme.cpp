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

#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QEvent>
#include <QtGui/QDesktopServices>
#include <QtGui/QFont>
#include <QtWidgets/QApplication>
#include <QtWidgets/QIcon>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>

#include <VibeCore/VSettings>
#include <VibeCore/VMimeType>
#include <VibeCore/VStandardDirectories>
#include <VibeGui/VColorScheme>

#include "visiontheme.h"

using namespace VStandardDirectories;

VisionTheme::VisionTheme()
{
    qDebug() << "ciao";
    m_settings = new VSettings("org.vision.desktop", "/interface");
}

VisionTheme::~VisionTheme()
{
    delete m_settings;
}

bool VisionTheme::usePlatformNativeDialog(DialogType type) const
{
    return false;
}

QPlatformDialogHelper *VisionTheme::createPlatformDialogHelper(DialogType type) const
{
    return 0;
}

const QPalette *VisionTheme::palette(Palette type) const
{
    switch (type) {
        case SystemPalette: {
            QString colorSchemeName = m_settings->value("color-scheme").toString();
            if (colorSchemeName.isEmpty())
                return QPlatformTheme::palette(type);

            QStringList paths;
            paths << findDirectory(UserThemesDirectory) + "/color-schemes"
                  << findDirectory(CommonThemesDirectory) + "/color-schemes"
                  << findDirectory(SystemThemesDirectory) + "/color-schemes";

            foreach(QString path, paths) {
                VColorScheme colorScheme(QString("%1/%2.colors").arg(path).arg(colorSchemeName));
                return colorScheme.palette();
            }
        }
        case ToolTipPalette:
            break;
    }

    return new QPalette();
}

QVariant VisionTheme::themeHint(ThemeHint hint) const
{
    switch (hint) {
        case QPlatformTheme::TextCursorWidth:
            return QVariant(1);
        case QPlatformTheme::DropShadow:
            return QVariant(true);
        case QPlatformTheme::MaximumScrollBarDragDistance:
            return QVariant(-1);
        case ToolButtonStyle:
            return m_settings->value("toolbutton-style");
        case ToolBarIconSize:
            return m_settings->value("toolbar-icon-size");
        case ItemViewActivateItemOnSingleClick:
            return QVariant(false);
        case SystemIconThemeName:
            return QVariant("KFaenza");
            return m_settings->value("icon-theme");
        case SystemIconFallbackThemeName:
            return QVariant("hicolor");
        case IconThemeSearchPaths: {
            QStringList paths;
            paths << findDirectory(UserThemesDirectory) + "/icons"
                  << findDirectory(CommonThemesDirectory) + "/icons"
                  << findDirectory(SystemThemesDirectory) + "/icons";
            return QVariant(paths);
        }
        case StyleNames:
            return QVariant("Vibe");
    }

    return QVariant();
}

#include "moc_visiontheme.cpp"
