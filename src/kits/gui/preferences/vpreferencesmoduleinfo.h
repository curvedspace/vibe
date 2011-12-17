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

#ifndef VPREFERENCESMODULEINFO_H
#define VPREFERENCESMODULEINFO_H

#include <VibeCore/VGlobal>

class QString;
class QStringList;

class VPreferencesModuleInfoPrivate;

/** \addtogroup gui Gui Kit
 *  @{
 */

/**
 * \class VPreferencesModuleInfo vpreferencesmoduleinfo.h <VPreferencesModuleInfo>
 *
 * \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VIBE_EXPORT VPreferencesModuleInfo
{
    Q_DECLARE_PRIVATE(VPreferencesModuleInfo)
public:
    /**
     * Constructs a VPreferencesModuleInfo object.
     * @note a VPreferencesModuleInfo object will have to be manually deleted, it's not
     * done automatically for you.
     * @param desktopFile The desktop file representing the module,
     * or the name of the module.
     */
    VPreferencesModuleInfo(const QString &desktopFile);

    /**
     * Destructs a VPreferencesModuleInfo object.
     */
    ~VPreferencesModuleInfo();

    /**
     * Assign operator.
     */
    VPreferencesModuleInfo &operator=(const VPreferencesModuleInfo &rhs);

    /**
     * Returns true if @p rhs describes the same Module as this object.
     */
    bool operator==(const VPreferencesModuleInfo &rhs);

    /**
     * Returns true if @p rhs does not describe the same Module as this object.
     */
    bool operator!=(const VPreferencesModuleInfo &rhs);

    /**
     * @returns the filename of the .desktop file that describes the module.
     */
    QString fileName() const;

    /**
     * @returns the keywords associated with this module.
     */
    QStringList keywords() const;

    /**
     * @returns the translated module's name.
     */
    QString name() const;

    /**
     * @returns the translated module's comment.
     */
    QString comment() const;

    /**
     * @returns the module's icon name.
     */
    QString iconName() const;

    /**
     * @returns the module's plugin name.
     */
    QString pluginName() const;

    /**
     * @returns the module's weight.
     */
    int weight() const;

private:
    VPreferencesModuleInfoPrivate *const d_ptr;
};

/** @}*/

#endif // VPREFERENCESMODULEINFO_H
