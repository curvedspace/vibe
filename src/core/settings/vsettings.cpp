/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPL-ONLY$
 *
 * This file may be used under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation and
 * appearing in the file LICENSE.LGPL included in the packaging of
 * this file, either version 2.1 of the License, or (at your option) any
 * later version.  Please review the following information to ensure the
 * GNU Lesser General Public License version 2.1 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 * If you have questions regarding the use of this file, please contact
 * us via http://www.maui-project.org/.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include <dconf/dconf.h>

#include <QDebug>

#include "vsettings.h"
#include "vsettings_p.h"
#include "vsettingstypes.h"

static void vsettings_notify(DConfClient *client, const gchar *path,
                             const gchar * const *items, gint n_items,
                             const gchar *tag, gpointer data)
{
    qDebug() << "Received notification for key" << path;

    VSettingsPrivate *self = (VSettingsPrivate *)data;
    self->notify(path);
}

static void vsettings_wrote(GObject *object, GAsyncResult *result, gpointer data)
{
    Q_UNUSED(object);

    VSettingsPrivate *self = (VSettingsPrivate *)data;
    if (self)
        dconf_client_write_finish(self->client, result, NULL, NULL);
}

static void vsettings_watch(GObject *object, GAsyncResult *result, gpointer data)
{
    Q_UNUSED(object);

    VSettingsPrivate *self = (VSettingsPrivate *)data;
    if (self)
        dconf_client_watch_finish(self->client, result, NULL);
}

static void vsettings_unwatch(GObject *object, GAsyncResult *result, gpointer data)
{
    Q_UNUSED(object);

    VSettingsPrivate *self = (VSettingsPrivate *)data;
    if (self)
        dconf_client_unwatch_finish(self->client, result, NULL);
}

/*
 * VSettingsPrivate
 */

VSettingsPrivate::VSettingsPrivate(VSettings *parent)
    : schema(0)
    , path(0)
    , q_ptr(parent)
{
    // Create the client object
    client = dconf_client_new(NULL, vsettings_notify, this, NULL);
    if (!client)
        qWarning() << "Unable to create a new DConfClient object!";
}

VSettingsPrivate::~VSettingsPrivate()
{
    client = 0;
}

void VSettingsPrivate::notify(const char *key)
{
    Q_Q(VSettings);

    emit q->changed(QString(key));
}

/*
 * VSettings
 */

VSettings::VSettings()
    : QObject()
    , d_ptr(new VSettingsPrivate(this))
{
}

VSettings::~VSettings()
{
    delete d_ptr;
}

void VSettings::addWatch(const QString &path)
{
    Q_D(VSettings);

    dconf_client_watch_async(d->client, path.toUtf8().constData(),
                             NULL, vsettings_watch, d);
}

void VSettings::removeWatch(const QString &path)
{
    Q_D(VSettings);

    return dconf_client_unwatch_async(d->client, path.toUtf8().constData(),
                                      NULL, vsettings_unwatch, d);
}

QVariant VSettings::value(const QString &key) const
{
    Q_D(const VSettings);

    // Read the value from DConf
    GVariant *value = dconf_client_read(d->client, key.toUtf8());

    // Read user's value
    if (!value)
        value = dconf_client_read_no_default(d->client, key.toUtf8().constData());

    // If the value is missing use the default one
    if (!value)
        value = dconf_client_read_default(d->client, key.toUtf8().constData());

    // If still not valid returns an empty value
    if (!value) {
        qWarning("No valid setting for '%s'!", key.toUtf8().constData());
        return QVariant();
    }

    // Convert GVariant to QVariant
    QVariant retval = vsettings_types_to_qvariant(value);
    g_variant_unref(value);
    return retval;
}

void VSettings::setValue(const QString &key, const QVariant &value)
{
    Q_D(VSettings);

    GVariant *defaultValue = NULL;

    // Get the current value, well convert its type to QVariant's
    defaultValue = dconf_client_read(d->client, key.toUtf8().constData());
    if (!defaultValue) {
        defaultValue = dconf_client_read_default(d->client, key.toUtf8().constData());
        if (!defaultValue)
            defaultValue = dconf_client_read_no_default(d->client, key.toUtf8().constData());
    }

    // Find out which type the key is
    const GVariantType *type = g_variant_get_type(defaultValue);

    // Convert the value we are about to set to GVariant
    GVariant *gvar = vsettings_types_collect(type, value.constData());
    dconf_client_write_async(d->client, key.toUtf8().constData(), gvar, NULL, vsettings_wrote, d);
}

#include "moc_vsettings.cpp"
