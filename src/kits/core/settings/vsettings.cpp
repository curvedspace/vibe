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

#include <QSettings>
#include <QStringList>

#include <VibeCore/VStandardDirectories>

#include "vsettings.h"
#include "vsettings_p.h"

#include "settingsschemaloader.h"

/*
 * VSettingsPrivate
 */

VSettingsPrivate::VSettingsPrivate(VSettings *parent) :
    q_ptr(parent),
    schema(0),
    path(0),
    dynamic(false)
{
    loader = new VPrivate::SettingsSchemaLoader();
}

VSettingsPrivate::~VSettingssPrivate()
{
    delete loader;
}

void VSettingsPrivate::setSchema(const QString &schemaId)
{
    schema = 0;

    // Find the schema whose identifier is the value passed as argument
    VPrivate::SettingsSchemaList list = d_ptr->loader->readCompiledSchemas();
    foreach (VPrivate::SettingsSchema *curSchema, list) {
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
    file = QSettings(fileName, QSettings::IniFormat);
}

void VSettingsPrivate::setPath(const QString &pathName)
{
    Q_ASSERT(schema);

    path = 0;

    // When an empty path name is passed it means the user don't
    // want to stick to a specific path
    if (pathName.isEmpty()) {
        file.endGroup();
        return;
    }

    // Find the path whose name is the value passed as argument
    foreach (VPrivate::SettingsPath *curPath, list) {
        if (curPath->name() == pathName) {
            path = curPath;
            break;
        }
    }
    Q_ASSERT(path);

    file.beginGroup(pathName);
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
    Q_D(VSettings);
    return d->schema->id();
}

void VSettings::setSchema(const QString &schema)
{
    Q_D(VSettings);
    d->setSchema(schema);
}

QString VSettings::path() const
{
    Q_D(VSettings);
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
    foreach (VPrivate::SettingsPath *curPath, d->schema->paths()) {
        if (curPath->name() == pathName) {
            foreach (VPrivate::SettingsKey *curKey, curPath->keys()) {
                if (curKey->name() == keyName) {
                    actualKey = curKey;
                    break;
                }
            }
        }
    }
    Q_ASSERT(actualKey);

    return d->file.value(keyName, actualKey->defaultValue());
}

void VSettings::setValue(const QString &key, const QVariant &value)
{
    Q_D(VSettings);

    d->file.setValue(key, value);
}

const QMetaObject *VSettings::metaObject() const
{
    Q_D(VSettings);

    /*
     * First we respond to qt_metacall() requests as per our staticMetaObject
     * table.  After this function is called, we handle them dynamically.
     * This gives QML the chance to set the "schema" property before we start
     * sending the property sets to
     */
    d->dynamic = true;
}

void *VSettings::qt_metacast(const char *name)
{
    if (name == NULL)
        return NULL;

    if (strcmp(name, "VSettings") == 0)
        return static_cast<void *>(const_cast<VSettings *>(this));

    return QObject::qt_metacast(name);
}

int VSettings::qt_metacall(QMetaObject::Call call, int id, void **params)
{
    id = QObject::qt_metacall(call, id, params);
    if (id < 0)
        return id;

    Q_D(VSettings);

    if (d->dynamic) {
        switch (call) {
        case QMetaObject::WriteProperty:
            break;
        case QMetaObject::ReadProperty:
            break;
        case QMetaObject::ResetProperty:
            break;
        case QMetaObject::QueryPropertyDesignable:
        case QMetaObject::QueryPropertyEditable:
        case QMetaObject::QueryPropertyScriptable:
        case QMetaObject::QueryPropertyStored:
        case QMetaObject::QueryPropertyUser:
            //id -= d->schema->count();
            break;
        case QMetaObject::CreateInstance:
            Q_ASSERT(false);
        case QMetaObject::InvokeMetaMethod:
            break;
        }
    } else {
        // Handle static properties (for QML construction)
        switch (call) {
        case QMetaObject::WriteProperty:
            if (id == 0)
                setSchema(*reinterpret_cast<QString *>(params[0]));
            else if (id == 1)
                setPath(*reinterpret_cast<QString *>(params[0]));

            // Fall through
        case QMetaObject::ResetProperty:
        case QMetaObject::ReadProperty:
        case QMetaObject::QueryPropertyDesignable:
        case QMetaObject::QueryPropertyEditable:
        case QMetaObject::QueryPropertyScriptable:
        case QMetaObject::QueryPropertyStored:
        case QMetaObject::QueryPropertyUser:
            id -= 2;
            break;

        case QMetaObject::CreateInstance:
        case QMetaObject::InvokeMetaMethod:
            Q_ASSERT(false);
        }
    }

    return id;
}

// For QML construction
static const char static_string_data[] = {  // offset
    "VSettings\0",                          // 0
    "QString\0",                            // 6
    "schema\0",                             // 14
    "path\0"                                // 21
};

static const uint static_data[] = {
    5,                                            // revision
    0,                                            // classname
    0, 0,                                         // classinfo
    0, 0,                                         // methods
    2, 14,                                        // properties
    0, 0,                                         // enums/sets
    0, 0,                                         // constructors
    0,                                            // flags
    0,                                            // signalCount
    14, 6, (QVariant::String << 24) | 0x4002,     // QString schema (Scriptable, Writable)
    21, 6, (QVariant::String << 24) | 0x4002,     // QString path (Scriptable, Writable)
    0                                             // EOD
};

const QMetaObject VSettings::staticMetaObject = {
    {
        &QObject::staticMetaObject,         // superclass data
        static_string_data,                 // string data
        static_data,                        // data
        NULL                                // extra data
    }
};
