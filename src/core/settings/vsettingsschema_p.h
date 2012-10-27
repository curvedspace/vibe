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

#ifndef VSETTINGSSCHEMA_P_H
#define VSETTINGSSCHEMA_P_H

#include <QDomAttr>
#include <QRect>
#include <QSize>
#include <QPoint>
#include <QUrl>
#include <QList>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Vibe API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

class VSettingsKey
{
public:
    struct Choice {
        QString name;
        QString context;
        QString summary;
        QString description;
        QString toolTip;
    };

    class Choices
    {
    public:
        Choices() {}
        Choices(const QList<Choice> &d, const QString &n, const QString &p)
            : prefix(p)
            , choices(d)
            , m_name(n) {
            int i = n.indexOf(QLatin1String("::"));
            if (i >= 0)
                m_externalQual = n.left(i + 2);
        }

        QString prefix;
        QList<Choice> choices;

        const QString &name() const  {
            return m_name;
        }

        const QString &externalQualifier() const  {
            return m_externalQual;
        }

        bool external() const  {
            return !m_externalQual.isEmpty();
        }

    private:
        QString m_name;
        QString m_externalQual;
    };

    VSettingsKey(const QString &group, const QString &type, const QString &key,
                 const QString &name, const QString &context, const QString &summary,
                 const QString &toolTip, const QString &description,
                 const QString &defaultValue, const Choices &choices, bool hidden)
        : m_group(group)
        , m_type(type)
        , m_key(key)
        , m_name(name)
        , m_context(context)
        , m_summary(summary)
        , m_toolTip(toolTip)
        , m_description(description)
        , m_choices(choices)
        , m_hidden(hidden) {
        m_defaultValue = stringToVariant(defaultValue, m_type);
    }

    void setGroup(const QString &group) {
        m_group = group;
    }
    QString group() const {
        return m_group;
    }

    void setType(const QString &type) {
        m_type = type;
    }
    QString type() const {
        return m_type;
    }

    void setKey(const QString &key) {
        m_key = key;
    }
    QString key() const {
        return m_key;
    }

    void setName(const QString &name) {
        m_name = name;
    }
    QString name() const {
        return m_name;
    }

    void setContext(const QString &context) {
        m_context = context;
    }
    QString context() const {
        return m_context;
    }

    void setSummary(const QString &summary) {
        m_summary = summary;
    }
    QString summary() const {
        return m_summary;
    }

    void setDescription(const QString &description) {
        m_description = description;
    }
    QString description() const {
        return m_description;
    }

    void setToolTip(const QString &toolTip) {
        m_toolTip = toolTip;
    }
    QString toolTip() const {
        return m_toolTip;
    }

    void setDefaultValue(const QVariant &d) {
        m_defaultValue = d;
    }
    QVariant defaultValue() const {
        return m_defaultValue;
    }

    void setMinValue(const QString &d) {
        m_min = d;
    }
    QString minValue() const {
        return m_min;
    }

    void setMaxValue(const QString &d) {
        m_max = d;
    }
    QString maxValue() const {
        return m_max;
    }

    void setParam(const QString &d) {
        m_param = d;
    }
    QString param() const {
        return m_param;
    }

    void setParamName(const QString &d) {
        m_paramName = d;
    }
    QString paramName() const {
        return m_paramName;
    }

    void setParamType(const QString &d) {
        m_paramType = d;
    }
    QString paramType() const {
        return m_paramType;
    }

    void setChoices(const QList<Choice> &d, const QString &n, const QString &p) {
        m_choices = Choices(d, n, p);
    }
    Choices choices() const {
        return m_choices;
    }

    void setParamValues(const QStringList &d) {
        m_paramValues = d;
    }
    QStringList paramValues() const {
        return m_paramValues;
    }

    void setParamDefaultValues(const QStringList &d) {
        m_paramDefaultValues = d;
    }
    QString paramDefaultValue(int i) const {
        return m_paramDefaultValues[i];
    }

    void setParamMax(int d) {
        m_paramMax = d;
    }
    int paramMax() const {
        return m_paramMax;
    }

    bool hidden() const {
        return m_hidden;
    }

    void dump() const {
#if 0
        cerr << "<key>" << endl;
        cerr << "  group: " << m_group << endl;
        cerr << "  type: " << m_type << endl;
        cerr << "  key: " << m_key << endl;
        cerr << "  name: " << m_name << endl;
        cerr << "  context: " << m_context << endl;
        cerr << "  summary: " << m_summary << endl;
        //      cerr << "  values: " << mValues.join(":") << endl;

        if (!param().isEmpty()) {
            cerr << "  param name: " << m_paramName << endl;
            cerr << "  param type: " << m_paramType << endl;
            cerr << "  paramvalues: " << m_paramValues.join(QChar::fromLatin1(':')) << endl;
        }
        cerr << "  default: " << m_defaultValue << endl;
        cerr << "  hidden: " << m_hidden << endl;
        cerr << "  min: " << m_min << endl;
        cerr << "  max: " << m_max << endl;
        cerr << "</key>" << endl;
#endif
    }

private:
    QString m_group;
    QString m_type;
    QString m_key;
    QString m_name;
    QString m_context;
    QString m_summary;
    QString m_toolTip;
    QString m_description;
    QVariant m_defaultValue;
    QString m_param;
    QString m_paramName;
    QString m_paramType;
    Choices m_choices;
    QStringList m_paramValues;
    QStringList m_paramDefaultValues;
    int m_paramMax;
    bool m_hidden;
    QString m_min;
    QString m_max;

    QStringList splitValueArgs(const QString &s) {
        int l = s.length();
        int idx = 0;

        QStringList result;
        QString item;

        for (++idx; idx < l; ++idx) {
            QChar c = s.at(idx);

            if (c == QLatin1Char(' ')) {
                result.append(item);
                item.clear();
            } else {
                item.append(c);
            }
        }

        return result;
    }

    QVariant stringToVariant(const QString &value, const QString &type) {
        // TODO: Types read from the schema must be converted to integer and here
        // we need to use a switch

        // Font
        if (type == "Rect") {
            QStringList args = splitValueArgs(value);
            if (args.length() == 4)
                return QVariant(QRect(args[0].toInt(), args[1].toInt(), args[2].toInt(), args[3].toInt()));
        } else if (type == "Size") {
            QStringList args = splitValueArgs(value);
            if (args.length() == 2)
                return QVariant(QSize(args[0].toInt(), args[1].toInt()));
            /*
            		} else if (type == "Color") {
            			QStringList args = splitValueArgs(value);
            			if (args.length() >= 3) {
            				int a = args.length() == 4 ? args[3].toInt() : 255;
            				return QVariant(QColor(args[0].toInt(), args[1].toInt(), args[2].toInt(), a));
            			}
            			return QVariant(QColor(value));
            */
        } else if (type == "Point") {
            QStringList args = splitValueArgs(value);
            if (args.length() == 2)
                return QVariant(QPoint(args[0].toInt(), args[1].toInt()));
            // DateTime
        } else if (type == "IntList") {
            QStringList args = splitValueArgs(value);
            QList<int> list;
            foreach(QString val, args)
            list.append(val.toInt());
            return QVariant::fromValue(list);
            // Enum
            // Path
        } else if (type == "PathList") {
            return QVariant::fromValue(splitValueArgs(value));
            // Password
        } else if (type == "Url") {
            return QVariant(QUrl::fromUserInput(value));
        } else if (type == "UrlList") {
            QStringList args = splitValueArgs(value);
            QList<QUrl> list;
            foreach(QString url, args)
            list.append(QUrl::fromUserInput(url));
            return QVariant::fromValue(list);
        }

        return QVariant(value);
    }
};

class VSettingsParam
{
public:
    QString name;
    QString type;
};

class VSettingsSchema
{
public:
    VSettingsSchema(const QString &schema);
    ~VSettingsSchema();

    bool process(const QString &fileName);

    VSettingsKey *parseEntry(const QString &group, const QDomElement &element);

    QString dumpNode(const QDomNode &node);

    VSettingsKey *lookupKey(const QString &keyName);

private:
    QRegExp *m_validNameRegexp;
    QStringList m_allNames;
    QList<VSettingsKey *> m_entries;
};

#endif // VSETTINGSSCHEMA_P_H
