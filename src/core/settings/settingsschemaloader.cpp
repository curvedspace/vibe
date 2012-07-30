/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
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

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QSize>
#include <QUrl>
#include <QXmlStreamReader>

#include <VibeCore/VGlobal>
#include <VibeCore/VStandardDirectories>

#include "settingsschemaloader.h"

const quint32 kMagicNumber = 'VSSL';
const quint32 kVersion = (1 << 16) + (0 << 8) + 0;
const QString kCompiledSchemas = QLatin1String("schemas.compiled");

using namespace VPrivate;

SettingsSchemaLoader::SettingsSchemaLoader() :
    m_schema(0)
{
}

SettingsSchemaLoader::~SettingsSchemaLoader()
{
    delete m_schema;
    m_schema = 0;
}

bool SettingsSchemaLoader::parseSchemaSource(const QString &fileName)
{
    // Open schema file
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Unable to read schema file \"" << fileName << "\"!";
        return false;
    }

    QXmlStreamReader xml;
    xml.setDevice(&file);

    SettingsPath *currentPath = 0;
    SettingsKey *currentKey = 0;
    QString currentTag;

    delete m_schema;
    m_schema = 0;

    // Parse it
    while (!xml.atEnd()) {
        xml.readNext();

        if (xml.isStartElement()) {
            if (xml.name() == "schema") {
                // Create a schema object if a valid schema is found
                QString schemaId = xml.attributes().value("id").toString();
                if (!schemaId.isEmpty())
                    m_schema = new SettingsSchema(schemaId);
            } else if (xml.name() == "path" && m_schema) {
                // Create path object if it has a name
                QString pathName = xml.attributes().value("name").toString();
                if (!pathName.isEmpty())
                    currentPath = new SettingsPath(pathName);
            } else if (xml.name() == "key" && currentPath) {
                // Check if name is valid
                QString name = xml.attributes().value("name").toString();
                if (name.isEmpty()) {
                    xml.raiseError(QString("Path \"%1\" has a key without name").arg(currentPath->name()));
                    break;
                }

                // Check if type is valid
                QString type = xml.attributes().value("type").toString();
                if (type.isEmpty()) {
                    xml.raiseError(QString("Key \"%1\" of path \"%2\" doesn't have a type defined")
                                   .arg(name).arg(currentPath->name()));
                    break;
                }

                QVariant::Type variantType;
                if (type == "char")
                    variantType = QVariant::Char;
                else if (type == "string")
                    variantType = QVariant::String;
                else if (type == "bytearray")
                    variantType = QVariant::ByteArray;
                else if (type == "int32")
                    variantType = QVariant::Int;
                else if (type == "uint32")
                    variantType = QVariant::UInt;
                else if (type == "int64")
                    variantType = QVariant::LongLong;
                else if (type == "uint64")
                    variantType =  QVariant::ULongLong;
                else if (type == "color")
                    variantType = QVariant::Color;
                else if (type == "size")
                    variantType = QVariant::Size;
                else if (type == "url")
                    variantType = QVariant::Url;
                else {
                    xml.raiseError(QString("Key \"%1\" of path \"%2\" is of type \"%3\" which is not supported")
                                   .arg(name).arg(currentPath->name()).arg(type));
                    break;
                }

                // Create key object
                currentKey = new SettingsKey(name, type);
                currentKey->setVariantType(variantType);
            }

            currentTag = xml.name().toString();
        } else if (xml.isEndElement()) {
            if (xml.name() == "path" && m_schema && currentPath)
                // Append path to the schema
                m_schema->appendPath(currentPath);
            else if (xml.name() == "key" && currentPath && currentKey)
                // Append key to its path
                currentPath->appendKey(currentKey);
        } else if (xml.isCharacters() && !xml.isWhitespace() && currentKey) {
            if (currentTag == "default") {
                QVariant value;

                switch (currentKey->variantType()) {
                    case QVariant::Char:
                    case QVariant::String:
                    case QVariant::ByteArray:
                    case QVariant::Int:
                    case QVariant::UInt:
                    case QVariant::LongLong:
                    case QVariant::ULongLong:
                        value = QVariant::fromValue(xml.text().toString());
                        break;
                    case QVariant::Size: {
                        QStringList sizeValues = xml.text().toString().split("x");
                        Q_ASSERT(sizeValues.size() == 2);
                        int w = sizeValues[0].toInt();
                        int h = sizeValues[1].toInt();
                        value = QVariant(QSize(w, h));
                    }
                    break;
                    case QVariant::Url:
                        value = QVariant(QUrl(xml.text().toString()));
                        break;
                    default: {
                        QByteArray a(xml.text().toString().toLatin1());
                        QDataStream stream(&a, QIODevice::ReadOnly);
                        stream.setVersion(QDataStream::Qt_4_0);
                        stream >> value;
                    }
                    break;
                }

                currentKey->setDefaultValue(value);
            } else if (currentTag == "summary")
                currentKey->setSummary(xml.text().toString());
            else if (currentTag == "description")
                currentKey->setDescription(xml.text().toString());
        }
    }

    // Error handling
    if (xml.hasError()) {
        qWarning() << "Parser error at line" << xml.lineNumber()
                   << "-" << xml.errorString().toLatin1().constData();
        file.close();
        return false;
    }

    file.close();
    return true;
}

SettingsSchemaList SettingsSchemaLoader::readCompiledSchemas()
{
    SettingsSchemaList list;

    // If the compiled schemas file doesn't exist yet, just return an empty list
    QFileInfo fileInfo(compiledSchemasFileName());
    if (!fileInfo.exists())
        return list;

    // If the file cannot be read return an empty list
    QFile file(compiledSchemasFileName());
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning().nospace() << "Unable to read the compiled schemas file "
                             << "\""
                             << compiledSchemasFileName().toLatin1().constData()
                             << "\": "
                             << file.errorString().toLatin1().constData();
        return list;
    }

    QDataStream in(&file);
    in.setByteOrder(QDataStream::BigEndian);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);
    in.setVersion(QDataStream::Qt_4_7);

    quint32 magicNumber, version;
    in >> magicNumber >> version;

    if (magicNumber != kMagicNumber) {
        qWarning().nospace() << "\""
                             << compiledSchemasFileName().toLatin1().constData()
                             << "\" is not a valid "
                             << "compiled schemas file or it's corrupted.";
        return list;
    }

    if (version > kVersion) {
        qWarning().nospace() << "Compiled schemas file \""
                             << compiledSchemasFileName().toLatin1().constData()
                             << "\" was created by a newer version of this program, "
                             << "file format is at version"
                             << decodeVersion(version).toLatin1().constData()
                             << "instead of "
                             << decodeVersion(kVersion).toLatin1().constData() << "!";
        return list;
    }

    qint32 schemasSize;
    in >> schemasSize;

    for (qint32 i = 0; i < schemasSize; i++) {
        QString id;
        qint32 pathsSize;
        SettingsPathList paths;

        in >> id >> pathsSize;
        for (qint32 j = 0; j < pathsSize; j++) {
            QString name;
            qint32 keysSize;
            SettingsKeyList keys;

            in >> name >> keysSize;

            SettingsPath *path = new SettingsPath(name);
            for (qint32 k = 0; k < keysSize; k++) {
                QString name;
                QString type;
                QVariant::Type variantType;
                QVariant defaultValue;
                QString summary;
                QString description;
                in >> name >> type >> variantType
                   >> defaultValue >> summary >> description;

                SettingsKey *key = new SettingsKey(name, type);
                key->setDefaultValue(defaultValue);
                key->setSummary(summary);
                key->setDescription(description);
                keys.append(key);
            }
            path->setKeys(keys);
            paths.append(path);
        }

        SettingsSchema *schema = new SettingsSchema(id);
        schema->setPaths(paths);
        list.append(schema);
    }

    file.close();
    return list;
}

bool SettingsSchemaLoader::appendParsedSchema()
{
    // Sanity check
    if (!m_schema) {
        qDebug().nospace() << Q_FUNC_INFO << ": parse a XML schema file before!";
        return false;
    }

    // Reload binary file
    SettingsSchemaList list = readCompiledSchemas();

    // Replace a previous version of this schema with the new one
    bool found = false;
    foreach(SettingsSchema * curSchema, list) {
        if (curSchema->id() == m_schema->id()) {
            list.replace(list.indexOf(curSchema), m_schema);
            found = true;
            break;
        }
    }

    // Or append it to the list
    if (!found)
        list.append(m_schema);

    // Open the binary file for writing
    QFile file(compiledSchemasFileName());
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning().nospace() << "Unable to write to the compiled schemas file "
                             << "\""
                             << compiledSchemasFileName().toLatin1().constData()
                             << "\": "
                             << file.errorString().toLatin1().constData();
        return false;
    }

    QDataStream out(&file);
    out.setByteOrder(QDataStream::BigEndian);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint32(kMagicNumber);
    out << quint32(kVersion);

    out << list.size();
    foreach(SettingsSchema * schema, list) {
        out << schema->id() << schema->paths().size();

        foreach(SettingsPath * path, schema->paths()) {
            out << path->name() << path->keys().size();

            foreach(SettingsKey * key, path->keys()) {
                out << key->name() << key->type() << key->variantType()
                    << key->defaultValue() << key->summary()
                    << key->description();
            }
        }
    }

    file.close();
    return true;
}

QString SettingsSchemaLoader::decodeVersion(quint32 version) const
{
    quint32 major = (version >> 16) & 0xff;
    quint32 minor = (version >> 8) & 0xff;
    quint32 release = version & 0xff;

    return QString("%1.%2.%3").arg(major).arg(minor).arg(release);
}

QString SettingsSchemaLoader::compiledSchemasFileName() const
{
    QString path = VStandardDirectories::findDirectory(VStandardDirectories::SystemDataDirectory);
    return QString("%1/vsettings/%2").arg(path).arg(kCompiledSchemas);
}
