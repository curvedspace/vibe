/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
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

#include "vsettings.h"
#include "vsettings_p.h"
#include "vsettingstypes.h"

#include <dconf-dbus-1.h>

#include <QtDBus>

static DConfDBusClient *dconfClient;

static void dconfClientInitialize()
{
    if (!dconfClient) {
        DBusConnection *sessionBus =
            (DBusConnection *)QDBusConnection::sessionBus().internalPointer();
        DBusConnection *systemBus =
            (DBusConnection *)QDBusConnection::systemBus().internalPointer();
        dconfClient = dconf_dbus_client_new(0, sessionBus, systemBus);
    }
}

static void vsettings_notify(DConfDBusClient *, const char *key, void *user_data)
{
    VSettingsPrivate *self = (VSettingsPrivate *)user_data;
    self->notify(key);
}

/*
 * VSettingsPrivate
 */

VSettingsPrivate::VSettingsPrivate(VSettings *parent)
    : schema(0)
    , path(0)
    , q_ptr(parent)
{
}

VSettingsPrivate::~VSettingsPrivate()
{
    if (path) {
        dconf_dbus_client_unsubscribe(dconfClient, vsettings_notify, this);
        delete path;
    }

    dconf_dbus_client_unref(dconfClient);
    dconfClient = 0;
}

void VSettingsPrivate::notify(const char *key)
{
    Q_Q(VSettings);

    const char *rel = key + path->size();
    int id = schema->findKey(rel);
    if (id < 0)
        return;

    GVariant *gvar = dconf_dbus_client_read(dconfClient, key);
    if (!gvar)
        gvar = schema->defaultValue(id);

    if (!g_variant_is_of_type(gvar, schema->valueType(id))) {
        g_variant_unref(gvar);
        gvar = schema->defaultValue(id);
    }

    QVariant qvar = vsettings_types_to_qvariant(gvar);
    g_variant_unref(gvar);

    emit q->changed(QString(key), qvar);
}

/*
 * VSettings
 */

VSettings::VSettings(const QString &schema, const QString &path) :
    QObject(),
    d_ptr(new VSettingsPrivate(this))
{
    dconfClientInitialize();

    setSchema(schema);
    setPath(path);
}

VSettings::~VSettings()
{
    delete d_ptr;
}

QString VSettings::schema() const
{
    Q_D(const VSettings);
    return d->schema->name();
}

void VSettings::setSchema(const QString &schema)
{
    Q_D(VSettings);

    d->schema = VSettingsSchema::getSchema(schema);

    const char *schemaPath = d->schema->path();
    if (schemaPath)
        setPath(schemaPath);
}

QString VSettings::path() const
{
    Q_D(const VSettings);
    return d->path->toUtf8();
}

void VSettings::setPath(const QString &path)
{
    Q_D(VSettings);

    if (path.isEmpty())
        return;

    if (d->path) {
        if (d->schema && d->schema->path())
            qFatal("VSettings path specifified, but schema '%s' has its own path",
                   qPrintable(d->schema->name()));
        else
            qFatal("VSettings path has already been specified!");
    }

    d->path = new QString(path);

    dconf_dbus_client_subscribe(dconfClient, d->path->toUtf8(), vsettings_notify, d);
}

QVariant VSettings::value(const QString &key) const
{
    Q_D(const VSettings);

    char *name = new char[key.length() + 1];
    (void)strncpy(name, key.toUtf8().constData(), key.length() + 1);
    int id = d->schema->findKey(name);
    if (id < 0) {
        delete name;
        return QVariant();
    }

    // Read the value from DConf
    GVariant *value = dconf_dbus_client_read(dconfClient, name);

    // If the value is missing use the default one
    if (!value)
        value = d->schema->defaultValue(id);

    // Maybe the value is not missing, but it has the wrong type,
    // in this case we ignore it and replace with the default value
    if (!g_variant_is_of_type(value, d->schema->valueType(id))) {
        g_variant_unref(value);
        value = d->schema->defaultValue(id);
    }

    QVariant retval = vsettings_types_to_qvariant(value);
    g_variant_unref(value);
    delete name;
    return retval;
}

void VSettings::setValue(const QString &key, const QVariant &value)
{
    Q_D(VSettings);

    char *name = new char[key.length() + 1];
    (void)strncpy(name, key.toUtf8().constData(), key.length() + 1);
    int id = d->schema->findKey(name);
    if (id < 0) {
        delete name;
        return;
    }

    GVariant *gvar = vsettings_types_collect(d->schema->valueType(id), value.constData());
    dconf_dbus_client_write(dconfClient, name, gvar);
    delete name;
}

#include "moc_vsettings.cpp"
