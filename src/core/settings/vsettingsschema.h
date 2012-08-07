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

#ifndef VSETTINGSSCHEMA_H
#define VSETTINGSSCHEMA_H

#include <glib.h>

#include <QObject>

typedef struct _VSettingsSchemaPrivate VSettingsSchemaPrivate;

class VSettingsSchema
{
public:
    static const VSettingsSchema *getSchema(const QString &name);

    const GVariantType *valueType(int id) const;
    GVariant *defaultValue(int id) const;
    const char *keyName(int id) const;
    const char *path() const;
    const QString &name() const;
    int count() const;
    int findKey(const char *key) const;

private:
    VSettingsSchema(const QString &name);

    VSettingsSchemaPrivate *priv;
};

#endif // VSETTINGSSCHEMA_H
