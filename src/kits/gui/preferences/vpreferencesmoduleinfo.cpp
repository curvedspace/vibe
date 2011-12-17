/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

#include <QString>
#include <QStringList>

#include <VibeCore/VDesktopFile>

#include "vpreferencesmoduleinfo.h"
#include "vpreferencesmoduleinfo_p.h"

/*
 * VPreferencesModuleInfoPrivate
 */

VPreferencesModuleInfoPrivate::VPreferencesModuleInfoPrivate(const QString &desktopFile) :
    fileName(desktopFile)
{
    // Append .desktop extension if missing
    if (!fileName.endsWith(".desktop"))
        fileName += ".desktop";

    entry = new VDesktopFile(fileName);
}

/*
 * VPreferencesModuleInfo
 */

VPreferencesModuleInfo::VPreferencesModuleInfo(const QString &desktopFile) :
    d_ptr(new VPreferencesModuleInfoPrivate(desktopFile))
{
}

VPreferencesModuleInfo::~VPreferencesModuleInfo()
{
    delete d_ptr;
}

VPreferencesModuleInfo &VPreferencesModuleInfo::operator =(const VPreferencesModuleInfo &rhs)
{
    *d_ptr = *rhs.d_ptr;
    return *this;
}

bool VPreferencesModuleInfo::operator ==(const VPreferencesModuleInfo &rhs)
{
    return (d_ptr->fileName == rhs.d_ptr->fileName && name() == rhs.name());
}

bool VPreferencesModuleInfo::operator !=(const VPreferencesModuleInfo &rhs)
{
    return !operator==(rhs);
}

QString VPreferencesModuleInfo::fileName() const
{
    Q_D(const VPreferencesModuleInfo);
    return d->fileName;
}

QStringList VPreferencesModuleInfo::keywords() const
{
    Q_D(const VPreferencesModuleInfo);
    return d->entry->value("X-Mirage-Keywords").toString().split(";", QString::SkipEmptyParts);
}

QString VPreferencesModuleInfo::name() const
{
    Q_D(const VPreferencesModuleInfo);
    return d->entry->name();
}

QString VPreferencesModuleInfo::comment() const
{
    Q_D(const VPreferencesModuleInfo);
    return d->entry->comment();
}

QString VPreferencesModuleInfo::iconName() const
{
    Q_D(const VPreferencesModuleInfo);
    return d->entry->iconName();
}

QString VPreferencesModuleInfo::pluginName() const
{
    Q_D(const VPreferencesModuleInfo);
    return d->entry->value("X-Mirage-PreferenceModule").toString();
}

int VPreferencesModuleInfo::weight() const
{
    Q_D(const VPreferencesModuleInfo);
    return d->entry->value("X-Mirage-PreferenceModule-Weight").toInt();
}
