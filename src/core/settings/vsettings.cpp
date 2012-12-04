/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPL2$
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include <QSettings>
#include <QStandardPaths>

#include <VibeCore/VFileSystemWatcher>

#include "vsettingsschema_p.h"
#include "vsettings.h"
#include "vsettings_p.h"

/*
 * VSettingsPrivate
 */

VSettingsPrivate::VSettingsPrivate(VSettings *parent, const QString &_schema)
    : schemaName(_schema)
    , q_ptr(parent)
{
    // Determine the file path
    fileName = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) +
            QLatin1String("/hawaii/") + schemaName + ".ini";

    // Create the storage
    storage = new QSettings(fileName, QSettings::IniFormat);
    watcher = new VFileSystemWatcher();
    watcher->addFile(fileName);

    // Schema
    schema = new VSettingsSchema(_schema);
}

VSettingsPrivate::~VSettingsPrivate()
{
    delete watcher;
    delete storage;
    delete schema;
}

void VSettingsPrivate::_q_fileChanged()
{
    Q_Q(VSettings);

    // Reload the file
    delete storage;
    storage = new QSettings(fileName, QSettings::IniFormat);

    emit q->changed();
}

/*
 * VSettings
 */

VSettings::VSettings(const QString &schemaName)
    : QObject()
    , d_ptr(new VSettingsPrivate(this, schemaName))
{
    connect(d_ptr->watcher, SIGNAL(created(QString)),
            this, SLOT(_q_fileChanged()));
    connect(d_ptr->watcher, SIGNAL(deleted(QString)),
            this, SLOT(_q_fileChanged()));
    connect(d_ptr->watcher, SIGNAL(dirty(QString)),
            this, SLOT(_q_fileChanged()));
}

VSettings::~VSettings()
{
    delete d_ptr;
}

/*!
    Returns the schema name.
*/
QString VSettings::schemaName() const
{
    Q_D(const VSettings);

    return d->schemaName;
}

/*!
    Returns the value of a key.
    \param key the key, with the complete path.
*/
QVariant VSettings::value(const QString &key) const
{
    Q_D(const VSettings);

    VSettingsKey *rawKey = d->schema->lookupKey(key);
    QVariant defaultValue = rawKey ? rawKey->defaultValue : QVariant();
    return d->storage->value(key, defaultValue);
}

/*!
    Sets the value for the specified key.
    \param key full path of the key.
    \param value the value to set the key to.
*/
void VSettings::setValue(const QString &key, const QVariant &value)
{
    Q_D(VSettings);

    d->storage->setValue(key, value);
}

#include "moc_vsettings.cpp"
