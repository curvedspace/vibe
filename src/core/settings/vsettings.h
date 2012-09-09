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

#ifndef VSETTINGS_H
#define VSETTINGS_H

#include <QObject>

#include <VibeCore/VGlobal>

class VSettingsPrivate;

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
        \param key the key, with the complete path.
    */
    QVariant value(const QString &key) const;

    /*!
        Sets the value for the specified key.
        \param key full path of the key.
        \param value the value to set the key to.
    */
    void setValue(const QString &key, const QVariant &value);

signals:
    void changed(const QString &key, const QVariant &value);

private:
    VSettingsPrivate *const d_ptr;
};

#endif // VSETTINGS_H
