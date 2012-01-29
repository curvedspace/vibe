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

#ifndef VIBEPLATFORM_H
#define VIBEPLATFORM_H

#include "qguiplatformplugin_p.h"

class VibePlatform : public QGuiPlatformPlugin
{
    Q_OBJECT
public:
    VibePlatform();
    ~VibePlatform();

    virtual QStringList keys() const;
    virtual QString styleName();
    virtual QPalette palette();
    virtual QString systemIconThemeName();
    virtual QStringList iconThemeSearchPaths();
    virtual QIcon fileSystemIcon(const QFileInfo &info);
    virtual int platformHint(PlatformHint hint);

private slots:
    void updateFonts();
    void updateWidgetStyle();
    void updateIconTheme();
    void updateColorScheme();
    void updateToolBarIconSize();
    void updateToolButtonStyle();
};

#endif // VIBEPLATFORM_H
