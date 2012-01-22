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

#ifndef VSETTINGS_P_H
#define VSETTINGS_P_H

#include <QSettings>

#include "settingsschemaloader.h"

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Vibe API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

class VFileSystemWatcher;

class VSettingsPrivate
{
    Q_DECLARE_PUBLIC(VSettings)
public:
    VSettingsPrivate(VSettings *parent);
    ~VSettingsPrivate();

    void setSchema(const QString &schemaId);
    void setPath(const QString &pathName);

    VPrivate::SettingsSchemaLoader *loader;
    VPrivate::SettingsSchema *schema;
    VPrivate::SettingsPath *path;
    bool dynamic;
    QSettings *file;
    QString fileName;
    VFileSystemWatcher *watcher;

    void extractPathAndKey(const QString &setting,
                           QString &pathName,
                           QString &keyName) const;

    void _q_dirty(const QString &fileName);
    void _q_deleted(const QString &fileName);

protected:
    VSettings *const q_ptr;
};

#endif // VSETTINGS_P_H
