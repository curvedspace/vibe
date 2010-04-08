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

#include <QtCore/QFileInfo>
#include <QtGui/QIcon>

#include "qubeguiplatform.h"
#include "mimetype.h"

QubeGuiPlatform::QubeGuiPlatform()
{
}

QStringList QubeGuiPlatform::keys() const
{
}

QString QubeGuiPlatform::styleName()
{
    return QLatin1String("Classic");
}

QPalette QubeGuiPlatform::palette()
{
}

QString QubeGuiPlatform::systemIconThemeName()
{
    return QLatin1String("Humanity");
}

QStringList QubeGuiPlatform::iconThemeSearchPaths()
{
}

QIcon QubeGuiPlatform::fileSystemIcon(const QFileInfo &info)
{
    QubeCore::MimeType mime;
    mime.fromFileName(info.filePath());
    return QIcon(mime.iconName());
}

int QubeGuiPlatform::platformHint(PlatformHint hint)
{
}

Q_EXPORT_PLUGIN2(QubeGuiPlatform, QubeGuiPlatform)
