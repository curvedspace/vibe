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

#include <QDebug>
#include <QStringList>
#include <QFileInfo>

#include <VibeCore/VStandardDirectories>
#include <VibeCore/VFileSystemWatcher>

#include "vsettings.h"
#include "vsettings_p.h"

#define VSETTINGS_DEBUG 0

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
    // Compiled schemas list loader
    loader = new VPrivate::SettingsSchemaLoader();

    watcher = new VFileSystemWatcher(q_ptr);
    // Watch for events happening to this file
    QObject::connect(watcher, SIGNAL(dirty(QString)),
                     q_ptr, SLOT(_q_dirty(QString)));
    QObject::connect(watcher, SIGNAL(deleted(QString)),
                     q_ptr, SLOT(_q_deleted(QString)));
}

VSettingsPrivate::~VSettingsPrivate()
{
    delete loader;
    delete file;
    delete watcher;
}

void VSettingsPrivate::setSchema(const QString &schemaId)
{
    schema = 0;

    // Find the schema whose identifier is the value passed as argument
    VPrivate::SettingsSchemaList list = loader->readCompiledSchemas();
    foreach(VPrivate::SettingsSchema * curSchema, list) {
#if VSETTINGS_DEBUG
        qDebug() << "Settings schema" << curSchema->id() << "vs" << schemaId;
#endif
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

    // Watch for this file
    QFileInfo fileInfo(fileName);
    watcher->addDir(fileInfo.absolutePath(), VFileSystemWatcher::WatchFiles);
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
}

void VSettingsPrivate::extractPathAndKey(const QString &setting,
                                         QString &pathName,
                                         QString &keyName) const
{
    QStringList parts = setting.split('/', QString::SkipEmptyParts);
    Q_ASSERT(!parts.isEmpty());
    keyName = parts.takeLast();
    pathName = path ? path->name() : QString();
    if (parts.size() >= 1)
        pathName = QString("/") + parts.join("/");
    Q_ASSERT(!pathName.isEmpty());
}

void VSettingsPrivate::_q_dirty(const QString &changedFileName)
{
    Q_Q(VSettings);

    if (changedFileName == fileName) {
        // Settings file has changed, it could have been modified by the user
        // with a text editor, or by another instance of VSettings in any case
        // it must be reloaded
        delete file;
        file = new QSettings(fileName, QSettings::IniFormat);
        emit q->changed();
    }
}

void VSettingsPrivate::_q_deleted(const QString &deletedFileName)
{
    Q_Q(VSettings);

    if (deletedFileName == fileName) {
        // Settings file has been delete, inform that settings changed because
        // without the settings file they are reset to default values from the
        // schema or the ones assumed by the caller
        delete file;
        file = 0;
        emit q->changed();
    }
}

/*
 * VSettings
 */

VSettings::VSettings(const QString &schema, const QString &path) :
    QObject(),
    d_ptr(new VSettingsPrivate(this))
{
    d_ptr->setSchema(schema);
    d_ptr->setPath(path);
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
    QString keyName, pathName;
    d->extractPathAndKey(key, pathName, keyName);

    // Find the key from the schema
    VPrivate::SettingsKey *actualKey = 0;
    foreach(VPrivate::SettingsPath * curPath, d->schema->paths()) {
#if VSETTINGS_DEBUG
        qDebug() << "Settings path" << curPath->name() << "vs" << pathName;
#endif
        if (curPath->name() == pathName) {
            foreach(VPrivate::SettingsKey * curKey, curPath->keys()) {
#if VSETTINGS_DEBUG
                qDebug() << "Settings key" << curKey->name() << "vs" << keyName;
#endif
                if (curKey->name() == keyName) {
                    actualKey = curKey;
                    break;
                }
            }
        }
    }
    Q_ASSERT(actualKey);

    // Take value from settings file
    if (d->file)
        return d->file->value(QString("%1/%2")
                              .arg(pathName).arg(keyName),
                              actualKey->defaultValue());

    // Take default value from schema
    return actualKey->defaultValue();
}

void VSettings::setValue(const QString &key, const QVariant &value)
{
    Q_D(VSettings);

    // Determine actual key name and path
    QString keyName, pathName;
    d->extractPathAndKey(key, pathName, keyName);

    // Set the value
    d->file->setValue(QString("%1/%2").arg(pathName).arg(keyName), value);
}

#include "vsettings.moc"
