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

#ifndef QUBEGUIPLATFORM_H
#define QUBEGUIPLATFORM_H

#include "qguiplatformplugin_p.h"
#include "settingsinterface.h"

class QubeGuiPlatform : public QGuiPlatformPlugin
{
    Q_OBJECT
public:
    QubeGuiPlatform();
    ~QubeGuiPlatform();

    virtual QStringList keys() const;
    virtual QString styleName();
    virtual QPalette palette();
    virtual QString systemIconThemeName();
    virtual QStringList iconThemeSearchPaths();
    virtual QIcon fileSystemIcon(const QFileInfo &info);
    virtual int platformHint(PlatformHint hint);

private slots:
    void updateWidgetStyle();
    void updateIconTheme();
    void updateColorScheme();
    void updateToolBarIconSize();
    void updateToolButtonStyle();

private:
    org::qubeos::Settings *m_settings;
};

#endif // QUBEGUIPLATFORM_H
