/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2010-2012 Pier Luigi Fiorini
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#include <QPlatformThemePlugin>

#include "mauitheme.h"

class VisionThemePlugin : public QPlatformThemePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPlatformThemeFactoryInterface" FILE "mauitheme.json")
public:
    explicit VisionThemePlugin(QObject *parent = 0);

    QStringList keys() const;
    QPlatformTheme *create(const QString &key, const QStringList &paramList);
};

VisionThemePlugin::VisionThemePlugin(QObject *parent) :
    QPlatformThemePlugin(parent)
{
}

QStringList VisionThemePlugin::keys() const
{
    return QStringList() << "Maui";
}

QPlatformTheme *VisionThemePlugin::create(const QString &key, const QStringList &paramList)
{
    if (key.toLower() == "maui")
        return new MauiTheme();
    return 0;
}

#include "main.moc"
