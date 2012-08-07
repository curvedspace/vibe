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

#include <stdio.h>
#include <stdlib.h>
#include <libintl.h>

#include "gvdb/gvdb-reader.h"
#include "vsettingstypes.h"
#include "vsettingsschema.h"

struct _VSettingsSchemaPrivate {
    QString name;

    QList<char *> names;
    QList<GVariant *> values;

    const char *gettext_domain;
    const char *path;
};

/* The following function is lifted verbatim out of gsettingsschema.c in GLib.  Do not modify it without first
 * modifying GLib, since doing so would introduce incompatible behaviour for schema lookups which is likely to
 * be quite confusing for everyone.
 */
static GSList *schema_sources;

static void
initialise_schema_sources(void)
{
    static gsize initialised;

    if G_UNLIKELY(g_once_init_enter(&initialised)) {
        const gchar *const *dir;
        const gchar *path;

        for (dir = g_get_system_data_dirs(); *dir; dir++) {
            gchar *filename;
            GvdbTable *table;

            filename = g_build_filename(*dir, "glib-2.0", "schemas",
                                        "gschemas.compiled", NULL);
            table = gvdb_table_new(filename, TRUE, NULL);

            if (table != NULL)
                schema_sources = g_slist_prepend(schema_sources, table);

            g_free(filename);
        }

        schema_sources = g_slist_reverse(schema_sources);

        if ((path = g_getenv("GSETTINGS_SCHEMA_DIR")) != NULL) {
            gchar *filename;
            GvdbTable *table;

            filename = g_build_filename(path, "gschemas.compiled", NULL);
            table = gvdb_table_new(filename, TRUE, NULL);

            if (table != NULL)
                schema_sources = g_slist_prepend(schema_sources, table);

            g_free(filename);
        }

        g_once_init_leave(&initialised, TRUE);
    }
}

const VSettingsSchema *VSettingsSchema::getSchema(const QString &name)
{
    static QHash<QString, VSettingsSchema *> schemas;

    initialise_schema_sources();

    VSettingsSchema *schema = schemas.value(name);
    if (!schema) {
        schema = new VSettingsSchema(name);
        schemas.insert(name, schema);
    }

    return schema;
}

static const char *lookup_string(GvdbTable *table, const char *key)
{
    const char *result = 0;
    GVariant *value;

    value = gvdb_table_get_raw_value(table, key);

    if (value) {
        result = g_variant_get_string(value, 0);
        g_variant_unref(value);
    }

    return result;
}

VSettingsSchema::VSettingsSchema(const QString &name)
{
    GvdbTable *table = 0;
    GSList *source;
    char **list;
    int i;

    priv = new VSettingsSchemaPrivate;

    for (source = schema_sources; source; source = source->next) {
        GvdbTable *file = (GvdbTable *) source->data;

        if ((table = gvdb_table_get_table(file, name.toUtf8())))
            break;
    }

    if (!table)
        qFatal("Settings schema '%s' is not installed\n", name.toUtf8().constData());

    list = gvdb_table_list(table, "");

    for (i = 0; list[i]; i++) {
        if (list[i][0] != '.') {
            GVariant *value;

            value = gvdb_table_get_raw_value(table, list[i]);

            // Skip values that we are incapable of supporting
            if (!g_variant_type_is_tuple(g_variant_get_type(value)) ||
                    vsettings_types_convert(g_variant_type_first(g_variant_get_type(value))) == QVariant::Invalid)
                continue;

            priv->names.append(g_strdup(list[i]));

            Q_ASSERT(value);

            priv->values.append(value);
        }
    }

    g_strfreev(list);

    priv->name = name;
    priv->gettext_domain = lookup_string(table, ".gettext-domain");
    priv->path = lookup_string(table, ".path");
    gvdb_table_unref(table);

    if (priv->gettext_domain)
        bind_textdomain_codeset(priv->gettext_domain, "UTF-8");
}

GVariant *VSettingsSchema::defaultValue(int id) const
{
    return g_variant_get_child_value(priv->values[id], 0);
}

const GVariantType *VSettingsSchema::valueType(int id) const
{
    return g_variant_type_first(g_variant_get_type(priv->values[id]));
}

const char *VSettingsSchema::keyName(int id) const
{
    return priv->names[id];
}

const char *VSettingsSchema::path() const
{
    return priv->path;
}

int VSettingsSchema::count() const
{
    Q_ASSERT(priv->names.size() == priv->values.size());

    return priv->names.size();
}

const QString &VSettingsSchema::name() const
{
    return priv->name;
}

int VSettingsSchema::findKey(const char *key) const
{
    for (int i = 0; i < priv->names.size(); i++) {
        if (strcmp(priv->names[i], key) == 0)
            return i;
    }

    return -1;
}
