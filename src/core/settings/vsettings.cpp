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

#include <QSettings>
#include <QStandardPaths>

#include "vsettings.h"
#include "vsettings_p.h"

/*
 * VSettingsPrivate
 */

VSettingsPrivate::VSettingsPrivate(VSettings *parent, const QString &schema)
    : schemaName(schema)
    , q_ptr(parent)
{
    // Determine the file path
    fileName = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) +
               QLatin1String("/hawaii/") + schemaName;

    // Create the storage
    storage = new QSettings(fileName, QSettings::IniFormat);
}

VSettingsPrivate::~VSettingsPrivate()
{
    storage->sync();
    delete storage;
}

/*
 * VSettings
 */

VSettings::VSettings(const QString &schemaName)
    : QObject()
    , d_ptr(new VSettingsPrivate(this, schemaName))
{
}

VSettings::~VSettings()
{
    delete d_ptr;
}

/*!
    Returns the schema name.
*/
QString VSettings::schemaName() const
{
    Q_D(const VSettings);

    return d->schemaName;
}

/*!
    Returns the value of a key.
    \param key the key, with the complete path.
*/
QVariant VSettings::value(const QString &key) const
{
    Q_D(const VSettings);

    return d->storage->value(key);
}

/*!
    Sets the value for the specified key.
    \param key full path of the key.
    \param value the value to set the key to.
*/
void VSettings::setValue(const QString &key, const QVariant &value)
{
    Q_D(VSettings);

    d->storage->setValue(key, value);
}

#include "moc_vsettings.cpp"
