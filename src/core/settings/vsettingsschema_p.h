/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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
