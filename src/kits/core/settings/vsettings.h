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

#ifndef VSETTINGS_H
#define VSETTINGS_H

#include <QObject>

#include <VibeCore/VGlobal>

class VSettingsPrivate;

/*!
    \addtogroup core Core Kit
   @{

    \class VSettings vsetting.h <VSettings>

    \brief Settings management.
*/
class VIBE_EXPORT VSettings : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(VSettings)
public:
    explicit VSettings(const QString &schema, const QString &path = QString());
    ~VSettings();

    QString schema() const;
    void setSchema(const QString &schema);

    QString path() const;
    void setPath(const QString &path);

    /*!
        Returns the value of a key.
        @param key the key, with the complete path.
    */
    QVariant value(const QString &key) const;

    /*!
        Sets the value for the specified key.
        @param key full path of the key.
        @param value the value to set the key to.
    */
    void setValue(const QString &key, const QVariant &value);

signals:
    void changed();

private:
    Q_PRIVATE_SLOT(d_ptr, void _q_dirty(const QString &))
    Q_PRIVATE_SLOT(d_ptr, void _q_deleted(const QString &))

    VSettingsPrivate *const d_ptr;
};

/*!
    @}
*/

#endif // VSETTINGS_H
