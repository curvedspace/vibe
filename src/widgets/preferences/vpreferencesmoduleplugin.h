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

#ifndef VPREFERENCESMODULEPLUGIN_H
#define VPREFERENCESMODULEPLUGIN_H

#include <QObject>
#include <QtCore/qplugin.h>

#include <VibeCore/VGlobal>

class VPreferencesModule;

#define VPreferencesModuleFactoryInterface_iid "org.hawaii.Vibe.VPreferencesModuleFactoryInterface"

/** \addtogroup gui Gui Kit
 *  @{
 */

/**
 * \class VPreferencesModulePlugin vpreferencesmoduleplugin.h <VPreferencesModulePlugin>
 *
 * \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VIBE_EXPORT VPreferencesModulePlugin : public QObject
{
    Q_OBJECT
public:
    /**
     * Contructs the VPreferencesModulePlugin object.
     */
    explicit VPreferencesModulePlugin(QObject *parent = 0);

    /**
     * Destructs the VPreferencesModulePlugin object.
     */
    ~VPreferencesModulePlugin();

    /**
     * Return the keys corresponding to the module objects that can be created.
     */
    virtual QStringList keys() const = 0;

    /**
     * Creates the preferences module object that corresponds to \a key.
     */
    virtual VPreferencesModule *create(const QString &key) const = 0;
};

/** @}*/

#endif // VPREFERENCESMODULEPLUGIN_H
