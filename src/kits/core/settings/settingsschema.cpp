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

#include "settingsschema.h"

namespace VPrivate
{
    /*
     * SettingsKey
     */

    SettingsKey::SettingsKey() :
        QObject()
    {
    }

    SettingsKey::SettingsKey(const QString &name, const QString &type) :
        QObject(),
        m_name(name),
        m_type(type)
    {
    }

    SettingsKey &SettingsKey::operator=(const SettingsKey &other)
    {
        m_name = other.name();
        m_type = other.type();
        m_summary = other.summary();
        m_description = other.description();
        m_defValue = other.defaultValue();
        return *this;
    }

    QDataStream &operator<<(QDataStream &out, const SettingsKey &key)
    {
        out << key.name() << key.type() << key.summary() << key.description()
            << key.defaultValue();
        return out;
    }

    QDataStream &operator>>(QDataStream &in, SettingsKey &key)
    {
        QString name;
        QString type;
        QString summary;
        QString description;
        QVariant defaultValue;

        in >> name >> type >> summary >> description >> defaultValue;

        key = SettingsKey(name, type);
        key.setSummary(summary);
        key.setDescription(description);
        key.setDefaultValue(defaultValue);

        return in;
    }

    QDataStream &operator<<(QDataStream &out, const SettingsKeyList &keyList)
    {
        out << keyList.size();
        foreach(SettingsKey * key, keyList)
        out << *key;
        return out;
    }

    QDataStream &operator>>(QDataStream &in, SettingsKeyList &keyList)
    {
        qint32 size;

        in >> size;
        for (qint32 i = 0; i < size; i++) {
            SettingsKey key;
            in >> key;
            keyList.append(&key);
        }

        return in;
    }

    /*
     * SettingsPath
     */

    SettingsPath::SettingsPath() :
        QObject()
    {
    }

    SettingsPath::SettingsPath(const QString &name) :
        QObject(),
        m_name(name)
    {
    }

    SettingsPath &SettingsPath::operator=(const SettingsPath &other)
    {
        m_name = other.name();
        m_keys = other.keys();
        return *this;
    }

    QDataStream &operator<<(QDataStream &out, const SettingsPath &path)
    {
        out << path.name() << path.keys();
        return out;
    }

    QDataStream &operator>>(QDataStream &in, SettingsPath &path)
    {
        QString name;
        SettingsKeyList keys;

        in >> name >> keys;

        path = SettingsPath(name);
        path.setKeys(keys);

        return in;
    }

    QDataStream &operator<<(QDataStream &out, const SettingsPathList &pathList)
    {
        out << pathList.size();
        foreach(SettingsPath * path, pathList)
        out << *path;
        return out;
    }

    QDataStream &operator>>(QDataStream &in, SettingsPathList &pathList)
    {
        qint32 size;

        in >> size;
        for (qint32 i = 0; i < size; i++) {
            SettingsPath path;
            in >> path;
            pathList.append(&path);
        }

        return in;
    }

    /*
     * SettingsSchema
     */

    SettingsSchema::SettingsSchema() :
        QObject()
    {
    }

    SettingsSchema::SettingsSchema(const QString &schema) :
        QObject(),
        m_id(schema)
    {
    }

    SettingsSchema::SettingsSchema(const SettingsSchema &schema) :
        QObject(),
        m_id(schema.id()),
        m_paths(schema.paths())
    {
    }

    SettingsSchema &SettingsSchema::operator=(const SettingsSchema &other)
    {
        m_id = other.id();
        m_paths = other.paths();
        return *this;
    }

    void SettingsSchema::printToStream()
    {
        qDebug() << id().toLatin1().constData();

        foreach(SettingsPath * path, paths()) {
            qDebug() << "\t" << path->name().toLatin1().constData();

            foreach(SettingsKey * key, path->keys()) {
                qDebug().nospace() << "\t\t" << key->name()
                                   << " (" << key->type().toLatin1().constData()
                                   << ")";
                if (!key->defaultValue().isNull())
                    qDebug() << "\t\t\t\tdefault value:" << key->defaultValue().toString();
            }
        }
    }

    QDataStream &operator<<(QDataStream &out, const SettingsSchema &schema)
    {
        out << schema.id() << schema.paths();
        return out;
    }

    QDataStream &operator>>(QDataStream &in, SettingsSchema &schema)
    {
        QString id;
        SettingsPathList paths;

        in >> id >> paths;

        schema = SettingsSchema(id);
        schema.setPaths(paths);

        return in;
    }

    /*
     * SettingsSchemaList
     */

    QDataStream &operator<<(QDataStream &out, const SettingsSchemaList &schemaList)
    {
        out << schemaList.size();
        foreach(SettingsSchema * schema, schemaList)
        out << *schema;

        return out;
    }

    QDataStream &operator>>(QDataStream &in, SettingsSchemaList &schemaList)
    {
        qint32 size;

        in >> size;
        schemaList.clear();
        for (qint32 i = 0; i < size; i++) {
            SettingsSchema schema;
            in >> schema;
            schemaList.append(new SettingsSchema(schema));
        }

        return in;
    }
}

#include "settingsschema.moc"
