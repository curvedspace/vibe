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

#include <QFileInfo>
#include <QEvent>
#include <QDesktopServices>
#include <QFont>
#include <QVariant>

#include <VibeCore/VSettings>
#include <VibeCore/VStandardDirectories>
#include <VibeGui/VColorScheme>

#include "hawaiiheme.h"

using namespace VStandardDirectories;

HawaiiTheme::HawaiiTheme()
{
    m_settings = new VSettings("org.maui.desktop.interface");
}

HawaiiTheme::~HawaiiTheme()
{
    delete m_settings;
}

bool HawaiiTheme::usePlatformNativeDialog(DialogType type) const
{
    return false;
}

QPlatformDialogHelper *HawaiiTheme::createPlatformDialogHelper(DialogType type) const
{
    return 0;
}

const QPalette *HawaiiTheme::palette(Palette type) const
{
#if 0
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
        default:
            break;
    }
#endif

    return new QPalette();
}

const QFont *HawaiiTheme::font(Font type) const
{
    QString fontName = m_settings->value("font-name").toString();
    int fontSize = m_settings->value("font-size").toInt();
    QFont *font = new QFont(fontName, fontSize);

    switch (type) {
        case TitleBarFont:
        case MdiSubWindowTitleFont:
        case DockWidgetTitleFont:
            font->setBold(true);
            break;
        case SmallFont:
            font->setPointSize(fontSize / 1.10);
            break;
        case MiniFont:
            font->setPointSize(fontSize / 1.25);
            break;
        default:
            break;
    }

    return font;
}

QVariant HawaiiTheme::themeHint(ThemeHint hint) const
{
    switch (hint) {
        case CursorFlashTime:
        case KeyboardInputInterval:
        case MouseDoubleClickInterval:
        case StartDragDistance:
        case StartDragTime:
        case KeyboardAutoRepeatRate:
        case PasswordMaskDelay:
        case StartDragVelocity:
            return QVariant();
        case TextCursorWidth:
            return QVariant(1);
        case DropShadow:
            return QVariant(true);
        case MaximumScrollBarDragDistance:
            return QVariant(-1);
        case ToolButtonStyle: {
            QString val = m_settings->value("toolbutton-style").toString();

            if (val == "icon-only")
                return QVariant(int(Qt::ToolButtonIconOnly));
            else if (val == "text-only")
                return QVariant(int(Qt::ToolButtonTextOnly));
            else if (val == "text-beside-icon")
                return QVariant(int(Qt::ToolButtonTextBesideIcon));
            else if (val == "text-under-icon")
                return QVariant(int(Qt::ToolButtonTextUnderIcon));

            return QVariant(int(Qt::ToolButtonFollowStyle));
        }
        case ToolBarIconSize:
            return m_settings->value("toolbar-icon-size");
        case ItemViewActivateItemOnSingleClick:
            return QVariant(false);
        case SystemIconThemeName:
            return m_settings->value("icon-theme");
        case SystemIconFallbackThemeName:
            return QVariant("hicolor");
        case IconThemeSearchPaths: {
            QStringList paths;
            paths << findDirectory(UserDataDirectory) + "/icons"
                  << findDirectory(CommonDataDirectory) + "/icons"
                  << findDirectory(SystemDataDirectory) + "/icons";
            return QVariant(paths);
        }
        case StyleNames: {
            QStringList styles;
            styles << m_settings->value("style").toString();
            return QVariant(styles);
        }
        case WindowAutoPlacement:
            return QVariant(true);
        case DialogButtonBoxLayout:
            return QVariant(1); // QDialogButtonBox::MacLayout
        case DialogButtonBoxButtonsHaveIcons:
            return QVariant(false);
        case UseFullScreenForPopupMenu:
            return QVariant(true);
        case KeyboardScheme:
            return QVariant(int(MacKeyboardScheme));
        case UiEffects:
            return AnimateMenuUiEffect | FadeMenuUiEffect |
                   AnimateComboUiEffect | AnimateTooltipUiEffect |
                   FadeTooltipUiEffect | AnimateToolBoxUiEffect;
        case SpellCheckUnderlineStyle:
            return QVariant();
    }

    return QPlatformTheme::themeHint(hint);
}
