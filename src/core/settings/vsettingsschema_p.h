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

#include <QRect>
#include <QSize>
#include <QPoint>
#include <QUrl>
#include <QList>
#include <QVariant>

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
    QString group;
    QString type;
    QString name;
    QString summary;
    QString description;
    QVariant defaultValue;
    int minValue;
    int maxValue;
    QVariantList choices;

    VSettingsKey() {
    }

    QVariant stringToVariant(const QString &value, const QString &type) {
#if 0
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
        } else if (type == "StringList") {
            return QVariant::fromValue(splitValueArgs(value));
        }
#endif

        return QVariant(value);
    }
};

class VSettingsSchema
{
public:
    VSettingsSchema(const QString &schema);

    bool process(const QString &fileName);

    VSettingsKey *lookupKey(const QString &keyName);

private:
    QString m_schema;
    QStringList m_allNames;
    QList<VSettingsKey *> m_entries;
};

#endif // VSETTINGSSCHEMA_P_H
