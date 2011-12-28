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

#ifndef SETTINGSSCHEMA_H
#define SETTINGSSCHEMA_H

#include <QDebug>
#include <QVariant>
#include <QDataStream>

namespace VPrivate
{
    class SettingsKey : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QString name READ name WRITE setName)
        Q_PROPERTY(QString type READ type WRITE setType)
        Q_PROPERTY(QVariant defaultValue READ defaultValue WRITE setDefaultValue)
        Q_PROPERTY(QString summary READ summary WRITE setSummary)
        Q_PROPERTY(QString description READ description WRITE setDescription)
    public:
        explicit SettingsKey();
        explicit SettingsKey(const QString &name, const QString &type);

        QString name() const {
            return m_name;
        }
        void setName(const QString &name) {
            m_name = name;
        }

        QString type() const {
            return m_type;
        }
        void setType(const QString &type) {
            m_type = type;
        }

        QVariant::Type variantType() const {
            return m_variantType;
        }
        void setVariantType(QVariant::Type type) {
            m_variantType = type;
        }

        QVariant defaultValue() const {
            return m_defValue;
        }
        void setDefaultValue(const QVariant &defaultValue) {
            m_defValue = defaultValue;
        }

        QString summary() const {
            return m_summary;
        }
        void setSummary(const QString &summary) {
            m_summary = summary;
        }

        QString description() const {
            return m_description;
        }
        void setDescription(const QString &description) {
            m_description = description;
        }

        SettingsKey &operator=(const SettingsKey &other);

    private:
        QString m_name;
        QString m_type;
        QVariant::Type m_variantType;
        QVariant m_defValue;
        QString m_summary;
        QString m_description;
    };

    typedef QList<SettingsKey *> SettingsKeyList;

    QDataStream &operator<<(QDataStream &out, const SettingsKey &key);
    QDataStream &operator>>(QDataStream &in, SettingsKey &key);

    QDataStream &operator<<(QDataStream &out, const SettingsKeyList &keyList);
    QDataStream &operator>>(QDataStream &in, SettingsKeyList &keyList);

    class SettingsPath : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QString name READ name WRITE setName)
        Q_PROPERTY(SettingsKeyList keys READ keys WRITE setKeys)
    public:
        explicit SettingsPath();
        explicit SettingsPath(const QString &name);

        QString name() const {
            return m_name;
        }
        void setName(const QString &name) {
            m_name = name;
        }

        SettingsKeyList keys() const {
            return m_keys;
        }
        void setKeys(const SettingsKeyList &list) {
            m_keys = list;
        }
        void appendKey(const SettingsKey *key) {
            m_keys.append((SettingsKey *)key);
        }

        SettingsPath &operator=(const SettingsPath &other);

    private:
        QString m_name;
        SettingsKeyList m_keys;
    };

    typedef QList<SettingsPath *> SettingsPathList;

    QDataStream &operator<<(QDataStream &out, const SettingsPath &path);
    QDataStream &operator>>(QDataStream &in, SettingsPath &path);

    QDataStream &operator<<(QDataStream &out, const SettingsPathList &pathList);
    QDataStream &operator>>(QDataStream &in, SettingsPathList &pathList);

    class SettingsSchema : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QString id READ id WRITE setId)
        Q_PROPERTY(SettingsPathList paths READ paths WRITE setPaths)
    public:
        explicit SettingsSchema();
        explicit SettingsSchema(const QString &schema);
        explicit SettingsSchema(const SettingsSchema &schema);

        QString id() const {
            return m_id;
        }
        void setId(const QString &id) {
            m_id = id;
        }

        SettingsPathList paths() const {
            return m_paths;
        }
        void setPaths(const SettingsPathList &list) {
            m_paths = list;
        }
        void appendPath(const SettingsPath *path) {
            m_paths.append((SettingsPath *)path);
        }

        SettingsSchema &operator=(const SettingsSchema &other);

        void printToStream();

    private:
        QString m_id;
        SettingsPathList m_paths;
    };

    typedef QList<SettingsSchema *> SettingsSchemaList;

    QDataStream &operator<<(QDataStream &out, const SettingsSchema &schema);
    QDataStream &operator>>(QDataStream &in, SettingsSchema &schema);

    QDataStream &operator<<(QDataStream &out, const SettingsSchemaList &schemaList);
    QDataStream &operator>>(QDataStream &in, SettingsSchemaList &schemaList);
}

#endif // SETTINGSSCHEMA_H
