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

#ifndef VPREFERENCESMODULEPLUGIN_H
#define VPREFERENCESMODULEPLUGIN_H

#include <QObject>
#include <QtCore/qplugin.h>

#include <VibeCore/VGlobal>

class QString;
class QStringList;

class VPreferencesModule;

/** \addtogroup gui Gui Kit
 *  @{
 */

/**
 * \class VPreferencesModulePlugin vpreferencesmoduleplugin.h <VPreferencesModulePlugin>
 *
 * \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VIBE_EXPORT VPreferencesModulePlugin
{
public:
    /**
     * Destructs the VPreferencesModulePlugin object.
     */
    virtual ~VPreferencesModulePlugin() {}

    /**
     * @returns the module's name.
     */
    virtual QString name() const = 0;

    /**
     * @returns the module's comment.
     */
    virtual QString comment() const = 0;

    /**
     * @returns the module's icon name.
     */
    virtual QString iconName() const = 0;

    /**
     * @returns the keywords associated with this module.
     */
    virtual QStringList keywords() const = 0;

    /**
     * @returns the category for this module.
     */
    virtual QString category() const = 0;

    /**
     * @returns the module's weight.
     */
    virtual int weight() const = 0;

    /**
     * @returns the actual preferences module object.
     */
    virtual VPreferencesModule *module() const = 0;
};

Q_DECLARE_INTERFACE(VPreferencesModulePlugin, "org.vision.Preferences.Module/1.0")

/** @}*/

#endif // VPREFERENCESMODULEPLUGIN_H
