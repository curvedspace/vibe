/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 * Copyright (c) 2003 Cornelius Schumacher
 * Copyright (c) 2003 Waldo Bastian
 * Copyright (c) 2003 Zack Rusin
 * Copyright (c) 2006 Michaël Larouche
 * Copyright (c) 2008 Allen Winter
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Cornelius Schumacher <schumacher@kde.org>
 *    Waldo Bastian <bastian@kde.org>
 *    Zack Rusin <zack@kde.org>
 *    Michaël Larouche <michael.larouche@kdemail.net>
 *    Allen Winter <winter@kde.org>
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

#include <QDebug>
#include <QFile>
#include <QStandardPaths>

#include "vsettingsschema_p.h"

VSettingsSchema::VSettingsSchema(const QString &schema)
{
    m_validNameRegexp = new QRegExp("[a-zA-Z_-][a-zA-Z0-9_-]*");

    QString fileName = QStandardPaths::locate(QStandardPaths::GenericDataLocation,
                                              QLatin1String("settings/") + schema + QLatin1String(".settings"));

    if (fileName.isEmpty())
        qWarning("Couldn't find \"%s\" configuration schema!", schema.toUtf8().constData());
    else
        process(fileName);
}

VSettingsSchema::~VSettingsSchema()
{
    delete m_validNameRegexp;
}

bool VSettingsSchema::process(const QString &fileName)
{
    QFile input(fileName);

    QDomDocument doc;
    QString errorMsg;
    int errorRow;
    int errorCol;
    if (!doc.setContent(&input, &errorMsg, &errorRow, &errorCol)) {
        qWarning() << "Parse error in " << fileName << ", line " << errorRow << ", col " << errorCol << ": " << errorMsg;
        return false;
    }

    QDomElement cfgElement = doc.documentElement();

    if (cfgElement.isNull()) {
        qWarning() << "No document in settings file!";
        return false;
    }

    QList<VSettingsParam> parameters;

    for (QDomElement e = cfgElement.firstChildElement(); !e.isNull(); e = e.nextSiblingElement()) {
        QString tag = e.tagName();

        if (tag == "schema") {
            for (QDomElement e2 = e.firstChildElement(); !e2.isNull(); e2 = e2.nextSiblingElement()) {
                if (e2.tagName() == "parameter") {
                    VSettingsParam p;
                    p.name = e2.attribute("name");
                    p.type = e2.attribute("type");
                    if (p.type.isEmpty())
                        p.type = "String";
                    parameters.append(p);
                }
            }
        } else if (tag == "group") {
            QString group = e.attribute("name");

            if (group.isEmpty()) {
                qWarning() << "Group without name!";
                return false;
            }

            for (QDomElement e2 = e.firstChildElement(); !e2.isNull(); e2 = e2.nextSiblingElement()) {
                if (e2.tagName() != "key")
                    continue;

                VSettingsKey *entry = parseEntry(group, e2);
                if (entry)
                    m_entries.append(entry);
                else {
                    qWarning() << "Cannot parse key!";
                    return false;
                }
            }
        }
    }

    return true;
}

VSettingsKey *VSettingsSchema::parseEntry(const QString &group, const QDomElement &element)
{
    QString type = element.attribute("type");
    QString name = element.attribute("name");
    QString key = element.attribute("key");
    QString hidden = element.attribute("hidden");
    QString context = element.attribute("context");
    QString summary;
    QString description;
    QString toolTip;
    QString defaultValue;
    QString param;
    QString param_name;
    QString paramType;
    VSettingsKey::Choices choices;
    QStringList paramValues;
    QStringList paramDefaultValues;
    QString minValue;
    QString maxValue;
    int paramMax = 0;

    for (QDomElement e = element.firstChildElement(); !e.isNull(); e = e.nextSiblingElement()) {
        QString tag = e.tagName();
        if (tag == "summary") {
            summary = e.text();
            context = e.attribute("context");
        } else if (tag == "description") {
            description = e.text();
            context = e.attribute("context");
        } else if (tag == "tooltip") {
            toolTip = e.text();
            context = e.attribute("context");
        } else if (tag == "min")
            minValue = e.text();
        else if (tag == "max")
            maxValue = e.text();
        else if (tag == "parameter") {
            param = e.attribute("name");
            paramType = e.attribute("type");
            if (param.isEmpty()) {
                qWarning() << "Parameter must have a name: " << dumpNode(e);
                return 0;
            }
            if (paramType.isEmpty()) {
                qWarning() << "Parameter must have a type: " << dumpNode(e);
                return 0;
            }
            if ((paramType == "Int") || (paramType == "UInt")) {
                bool ok;
                paramMax = e.attribute("max").toInt(&ok);
                if (!ok) {
                    qWarning() << "Integer parameter must have a maximum (e.g. max=\"0\"): "
                               << dumpNode(e);
                    return 0;
                }
            } else if (paramType == "Enum") {
                for (QDomElement e2 = e.firstChildElement(); !e2.isNull(); e2 = e2.nextSiblingElement()) {
                    if (e2.tagName() == "values") {
                        for (QDomElement e3 = e2.firstChildElement(); !e3.isNull(); e3 = e3.nextSiblingElement()) {
                            if (e3.tagName() == "value")
                                paramValues.append(e3.text());
                        }
                        break;
                    }
                }
                if (paramValues.isEmpty()) {
                    qWarning() << "No values specified for parameter '" << param;
                    return 0;
                }
                paramMax = paramValues.count() - 1;
            } else {
                qWarning("Parameter '%s' has type %s but must be of type int, uint or Enum.",
                         param.toUtf8().constData(), paramType.toUtf8().constData());
                return 0;
            }
        } else if (tag == "default") {
            if (e.attribute("param").isEmpty())
                defaultValue = e.text();
        } else if (tag == "choices") {
            QString name = e.attribute("name");
            QString prefix = e.attribute("prefix");
            QList<VSettingsKey::Choice> chlist;
            for (QDomElement e2 = e.firstChildElement(); !e2.isNull(); e2 = e2.nextSiblingElement()) {
                if (e2.tagName() == "choice") {
                    VSettingsKey::Choice choice;
                    choice.name = e2.attribute("name");

                    if (choice.name.isEmpty())
                        qWarning() << "Tag <choice> requires attribute 'name'.";

                    for (QDomElement e3 = e2.firstChildElement(); !e3.isNull(); e3 = e3.nextSiblingElement()) {
                        if (e3.tagName() == "summary") {
                            choice.summary = e3.text();
                            choice.context = e3.attribute("context");
                        }
                        if (e3.tagName() == "description") {
                            choice.description = e3.text();
                            choice.context = e3.attribute("context");
                        }
                        if (e3.tagName() == "tooltip") {
                            choice.toolTip = e3.text();
                            choice.context = e3.attribute("context");
                        }
                    }
                    chlist.append(choice);
                }
            }
            choices = VSettingsKey::Choices(chlist, name, prefix);
        }
    }

    bool nameIsEmpty = name.isEmpty();
    if (nameIsEmpty && key.isEmpty()) {
        qWarning() << "Entry must have a name or a key: " << dumpNode(element);
        return 0;
    }

    if (key.isEmpty())
        key = name;

    if (nameIsEmpty) {
        name = key;
        name.remove(' ');
    } else if (name.contains(' ')) {
        qWarning("Entry '%s' contains spaces! <name> elements can not contain spaces!", name.toUtf8().constData());
        name.remove(' ');
    }

    if (name.contains("$(")) {
        if (param.isEmpty()) {
            qWarning() << "Name may not be parameterized: " << name;
            return 0;
        }
    } else {
        if (!param.isEmpty()) {
            qWarning("Name must contain '$(%s)': %s", param.toUtf8().constData(),
                     name.toUtf8().constData());
            return 0;
        }
    }

    if (summary.isEmpty())
        summary = key;

    // XXX : implicit type might be bad
    if (type.isEmpty())
        type = "String";

    if (!param.isEmpty()) {
        // Adjust name
        param_name = name;
        name.remove("$(" + param + ')');

        // Lookup defaults for indexed entries
        for (int i = 0; i <= paramMax; i++)
            paramDefaultValues.append(QString());

        for (QDomElement e = element.firstChildElement(); !e.isNull(); e = e.nextSiblingElement()) {
            QString tag = e.tagName();
            if (tag == "default") {
                QString index = e.attribute("param");
                if (index.isEmpty())
                    continue;

                bool ok;
                int i = index.toInt(&ok);
                if (!ok) {
                    i = paramValues.indexOf(index);
                    if (i == -1) {
                        qWarning("Index '%s' for default value is unknown.", index.toUtf8().constData());
                        return 0;
                    }
                }

                if ((i < 0) || (i > paramMax)) {
                    qWarning("Index '%d' for default value is out of range [0, %d].", i, paramMax);
                    return 0;
                }

                paramDefaultValues[i] = e.text();
            }
        }
    }

    if (!m_validNameRegexp->exactMatch(name)) {
        if (nameIsEmpty)
            qWarning("The key '%s' can not be used as name for the key because "
                     "it is not a valid name. You need to specify a valid name for this entry.",
                     key.toUtf8().constData());
        else
            qWarning("The name '%s' is not a valid name for a key.", name.toUtf8().constData());
        return 0;
    }

    if (m_allNames.contains(name)) {
        if (nameIsEmpty)
            qWarning("The key '%s' can not be used as name for the key because "
                     "it does not result in a unique name. You need to specify a unique name for this key.",
                     key.toUtf8().constData());
        else
            qWarning("The name '%s' is not unique.", name.toUtf8().constData());
        return 0;
    }
    m_allNames.append(name);

    VSettingsKey *result = new VSettingsKey(group, type, key, name, context, summary, toolTip, description,
                                            defaultValue, choices, hidden == "true");
    if (!param.isEmpty()) {
        result->setParam(param);
        result->setParamName(param_name);
        result->setParamType(paramType);
        result->setParamValues(paramValues);
        result->setParamDefaultValues(paramDefaultValues);
        result->setParamMax(paramMax);
    }
    result->setMinValue(minValue);
    result->setMaxValue(maxValue);

    return result;
}

QString VSettingsSchema::dumpNode(const QDomNode &node)
{
    QString msg;
    QTextStream s(&msg, QIODevice::WriteOnly);
    node.save(s, 0);

    msg = msg.simplified();
    if (msg.length() > 40)
        return msg.left(37) + QString::fromLatin1("...");
    return msg;
}

VSettingsKey *VSettingsSchema::lookupKey(const QString &keyName)
{
    // Split keyName to get group and key name
    QStringList parts = keyName.split(QLatin1Char('/'));
    if (parts.length() != 2) {
        qWarning("Settings key '%s' has a wrong notation!", keyName.toUtf8().constData());
        return 0;
    }

    // FIXME: This need to be optimized, for example instead of a list of
    // VSettingsKey we need a map between groups and lists of keys.
    foreach(VSettingsKey * key, m_entries) {
        // Ignore keys of other groups
        if (key->group() != parts.at(0))
            continue;

        if (key->key() == parts.at(1))
            return key;
    }

    return 0;
}
