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

#ifndef VSETTINGS_H
#define VSETTINGS_H

#include <QObject>

#include <VibeCore/VGlobal>

class VSettingsPrivate;

/**
 * \class VSettings vsetting.h <VSettings>
 *
 * \brief Settings management.
 *
 * The primary interface to VSettings is through setting and getting QObject
 * properties. A QMetaObject based on the contents of the settings schema is
 * dynamically generated and exposed, response to qt_metacall() requests is
 * based on that metadata.
 *
 * QML wants to construct the VSettings object with no arguments and set the
 * properties of it after it's constructed.
 * It needs two things to work: a constructor that takes zero arguments and a
 * staticMetaObject with the properties that it can set at construct time.
 *
 * We export the properties "schema" and "path" so that something like this
 * can be done:
 * @code
 * VSettings {
 *   id: settings
 *   schema: "org.vision.desktop"
 * }
 * @endcode
 *
 * The staticMetaObject is only used at construction time; for future accesses
 * (ie: settings.decorator) the dynamic properties from the schema will be used.
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

    /**
     * Returns the value of a key.
     * @param key the key, with the complete path.
     */
    QVariant value(const QString &key) const;

    /**
     * Sets the value for the specified key.
     * @param key full path of the key.
     * @param value the value to set the key to.
     */
    void setValue(const QString &key, const QVariant &value);

    static const QMetaObject staticMetaObject();
    VSettings();

    virtual int qt_metacall(QMetaObject::Call, int, void **);
    virtual const QMetaObject *metaObject() const;
    virtual void *qt_metacast(const char *);

private:
    VSettingsPrivate *const d_ptr;
};

#endif // VSETTINGS_H
