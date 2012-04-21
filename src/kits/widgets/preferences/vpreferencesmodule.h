/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef VPREFERENCESMODULE_H
#define VPREFERENCESMODULE_H

#include <QWidget>

#include <VibeCore/VGlobal>

class VPreferencesModulePrivate;

/** \addtogroup gui Gui Kit
 *  @{
 */

/**
 * \class VPreferencesModule vpreferencesmodule.h <VPreferencesModule>
 *
 * The base class for all preferences modules.
 *
 * Preferences modules are realized as plugins that are loaded only
 * when needed.
 *
 * All the necessary glue logic and layout management are handled by
 * the Preferences application and must not concern the module author.
 *
 * To write a preferences module you have to create a plugin that
 * contains a class that derives from VPreferencesPlugin like in the following
 * example.
 *
 * Header file:
 *
 * @code
 * #include <VPreferencesModule>
 * #include <VPreferencesPlugin>
 *
 * class MyPreferences : public VPreferencesModule
 * {
 *     Q_OBJECT
 *     Q_INTERFACES(VPreferencesModule)
 * public:
 *     explicit MyPreferences(QWidget *parent = 0);
 *
 *     virtual void load();
 *     virtual void save();
 * };
 * @endcode
 *
 * C++ source code file:
 *
 * @code
 * #include "mypreferences.h"
 *
 * MyPreferences::MyPreferences(QWidget *parent) :
 *     VPreferencesModule(parent)
 * {
 *     // ... GUI creation code goes here ...
 * }
 *
 * void MyPreferences::load()
 * {
 *     // ... preferences loading code here ...
 * }
 *
 * void MyPreferences::save()
 * {
 *     // ... preferences saving code here ...
 * }
 *
 * Q_EXPORT_PLUGIN2(my_preferences, MyPreferences)
 * @endcode
 *
 * \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VIBE_EXPORT VPreferencesModule : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool requireAdministrativePrivileges READ requireAdministrativePrivileges)
    Q_DECLARE_PRIVATE(VPreferencesModule)
public:
    /**
     * Constructs a VPreferencesModule object.
     */
    explicit VPreferencesModule(QWidget *parent = 0);

    /**
     * When a module must be run with administrative privileges, or acts
     * differently for administrators and normal users, this method
     * must return true.
     */
    bool requireAdministrativePrivileges() const;

    /**
     * When a module must be run with administrative privileges, or acts
     * differently for administrators and normal users, it's sometimes useful
     * to customize the message that appears at the top of the module when
     * used as a normal user. This method returns the customized message.
     * If none has been set, a default message will be used.
     */
    QString useAdministratorOnlyMessage() const;

public slots:
    /**
     * Loads the configuration data into the module.
     *
     * This method sets the user interface elements of the module to reflect
     * the current settings stored in the configuration file(s).
     *
     * This method is invoked whenever the module should read its configuration
     * and update the user interface. This happens right after construction.
     *
     * Module authors may also call this method when the user clicks the
     * "Reset" button, in order to restore settings the way they were before
     * changes in the user interface occurred.
     *
     * When this method is called, the changed() signal is emitted.
     *
     * \see changed(bool)
     */
    virtual void load();

    /**
     * Saves the configuration data.
     *
     * This method stores the configuration information as shown in the user
     * interface to the configuration file(s).
     *
     * Module authors must call this method when the user clicks the "Apply"
     * button.
     */
    virtual void save();

    /**
     * Sets the configuration to sensible default values.
     *
     * Module authors must call this method when the user clicks the "Default"
     * button.
     *
     * When this method is called, the changed() signal is emitted.
     *
     * \see changed(bool)
     */
    virtual void defaults();

signals:
    /**
     * Indicates that the state of the module's contents has changed.
     *
     * This signal is emitted whenever the state of the configuration shown
     * in the module changes. It allows the module container to keep track
     * of unsaved changes.
     */
    void changed(bool state);

protected:
    /**
     * Show event override.
     * If it's the first time that the Preferences application loads the
     * the preferences module, it calls load() and emits the changed(bool) signal.
     *
     * \see load()
     * \see changed(bool)
     */
    virtual void showEvent(QShowEvent *event);

private:
    VPreferencesModulePrivate *const d_ptr;
};

/** @}*/

#endif // VPREFERENCESMODULE_H