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

#include <QStringList>

#include <VibeCore/VStandardDirectories>

#include "vsettings.h"
#include "vsettings_p.h"

/*
 * VSettingsPrivate
 */

VSettingsPrivate::VSettingsPrivate(VSettings *parent) :
    q_ptr(parent),
    schema(0),
    path(0),
    dynamic(false),
    file(0)
{
    loader = new VPrivate::SettingsSchemaLoader();
}

VSettingsPrivate::~VSettingsPrivate()
{
    delete loader;
    delete file;
}

void VSettingsPrivate::setSchema(const QString &schemaId)
{
    schema = 0;

    // Find the schema whose identifier is the value passed as argument
    VPrivate::SettingsSchemaList list = loader->readCompiledSchemas();
    foreach(VPrivate::SettingsSchema * curSchema, list) {
        if (curSchema->id() == schemaId) {
            schema = curSchema;
            break;
        }
    }
    Q_ASSERT(schema);

    // Determine user's settings file by schema identifier
    fileName = QString("%1/vsettings/%2.conf")
               .arg(VStandardDirectories::findDirectory(VStandardDirectories::UserSettingsDirectory))
               .arg(schemaId);

    // Create a new settings object
    delete file;
    file = new QSettings(fileName, QSettings::IniFormat);
}

void VSettingsPrivate::setPath(const QString &pathName)
{
    Q_ASSERT(schema);

    path = 0;

    // When an empty path name is passed it means the user don't
    // want to stick to a specific path
    if (pathName.isEmpty()) {
        file->endGroup();
        return;
    }

    // Find the path whose name is the value passed as argument
    foreach(VPrivate::SettingsPath * curPath, schema->paths()) {
        if (curPath->name() == pathName) {
            path = curPath;
            break;
        }
    }
    Q_ASSERT(path);

    file->beginGroup(pathName);
}

/*
 * VSettings
 */

VSettings::VSettings(const QString &schema, const QString &path) :
    QObject(),
    d_ptr(new VSettingsPrivate(this))
{
    d_ptr->setSchema(schema);
}

VSettings::~VSettings()
{
    delete d_ptr;
}

QString VSettings::schema() const
{
    Q_D(const VSettings);
    return d->schema->id();
}

void VSettings::setSchema(const QString &schema)
{
    Q_D(VSettings);
    d->setSchema(schema);
}

QString VSettings::path() const
{
    Q_D(const VSettings);
    return d->path ? d->path->name() : QString();
}

void VSettings::setPath(const QString &path)
{
    Q_D(VSettings);
    d->setPath(path);
}

QVariant VSettings::value(const QString &key) const
{
    Q_D(const VSettings);

    // Determine actual key name and path
    QStringList parts = key.split('/', QString::SkipEmptyParts);
    Q_ASSERT(!parts.isEmpty());
    QString keyName = parts.takeLast();
    QString pathName = path();
    if (parts.size() > 0)
        pathName = QString("/") + parts.join("/");
    Q_ASSERT(!pathName.isEmpty());

    // Find the key from the schema
    VPrivate::SettingsKey *actualKey = 0;
    foreach(VPrivate::SettingsPath * curPath, d->schema->paths()) {
        if (curPath->name() == pathName) {
            foreach(VPrivate::SettingsKey * curKey, curPath->keys()) {
                if (curKey->name() == keyName) {
                    actualKey = curKey;
                    break;
                }
            }
        }
    }
    Q_ASSERT(actualKey);

    return d->file->value(keyName, actualKey->defaultValue());
}

void VSettings::setValue(const QString &key, const QVariant &value)
{
    Q_D(VSettings);

    d->file->setValue(key, value);
}

#include "vsettings.moc"
