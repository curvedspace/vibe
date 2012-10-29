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

#ifndef VPREFERENCESMODULE_H
#define VPREFERENCESMODULE_H

#include <QWidget>

#include <VibeWidgets/VibeWidgetsExport>

/**
 * \brief Base class for all preferences modules.
 *
 * Preferences modules are realized as plugins that are loaded by the
 * the System Preferences application.
 *
 * To write a preferences module you have to create two classes, one that
 * inherits from VPreferencesPlugin and another that implements the
 * module user interface and inherits from VPreferencesModule.
 *
 * Plugin header file:
 *
 * @code
 * #include <VPreferencesPlugin>
 *
 * class MyPreferencesPlugin : public VPreferencesModulePlugin
 * {
 *     Q_OBJECT
 *     Q_PLUGIN_METADATA(IID "org.hawaii.Vibe.VPreferencesModuleFactoryInterface" FILE "mypreferences.json")
 * public:
 *     explicit MyPreferencesPlugin(QObject *parent = 0);
 *
 *     QStringList keys() const;
 *     VPreferencesModule *create(const QString &key) const;
 * };
 * @endcode
 *
 * Module header file:
 *
 * @code
 * #include <VPreferencesModule>
 *
 * class MyPreferences : public VPreferencesModule
 * {
 *     Q_OBJECT
 *     Q_INTERFACES(VPreferencesModule)
 * public:
 *     explicit MyPreferences();
 *
 *     QString name() const;
 *     QString comment() const;
 *     QString iconName() const;
 *     QStringList keywords() const;
 *     VPreferencesModule::Category category() const;
 * };
 * @endcode
 *
 * Plugin C++ source code file:
 *
 * @code
 * MyPreferencesPlugin::MyPreferencesPlugin(QObject *parent)
 *     : VPreferencesModulePlugin(parent)
 * {
 * }
 *
 * QStringList MyPreferencesPlugin::keys() const
 * {
 *     return QStringList() << "mypreferences";
 * }
 *
 * VPreferencesModule *MyPreferencesPlugin::create(const QString &key) const
 * {
 *     if (key.toLower() == "mypreferences")
 *         return new MyPreferences();
 *     return 0;
 * }
 * @endcode
 *
 * Module C++ source code file:
 *
 * @code
 * #include "mypreferences.h"
 *
 * MyPreferences::MyPreferences()
 *     : VPreferencesModule(parent)
 * {
 *     // ... Create the UI here ...
 * }
 *
 * QString MyPreferences::name() const
 * {
 *     return tr("Name");
 * }
 *
 * QString MyPreferences::comment() const
 * {
 *     return tr("Configure something.");
 * }
 *
 * QString MyPreferences::iconName() const
 * {
 *     return QLatin1String("preferences-something");
 * }
 *
 * QStringList MyPreferences::keywords() const
 * {
 *     return tr("something;something else").split(QLatin1Char(';'));
 * }
 *
 * VPreferencesModule::Category category() const
 * {
 *     return VPreferencesModule::PersonalCategory;
 * }
 * @endcode
 *
 * \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VIBEWIDGETS_EXPORT VPreferencesModule : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool requiresAdministrativePrivileges READ requiresAdministrativePrivileges)
    Q_ENUMS(Category)
public:
    enum Category {
        //! Personal preferences.
        PersonalCategory,
        //! Hardware preferences.
        HardwareCategory,
        //! System preferences.
        SystemCategory,
        //! Other preferences.
        OtherCategory,
    };

    /**
     * Constructs a VPreferencesModule object.
     */
    explicit VPreferencesModule();

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
    virtual Category category() const = 0;

    /**
     * Some preference modules requires administrative privileges
     * to configure their settings.  If your preferences modules
     * requires administrative privileges return true.
     * This method returns false by default.
     */
    virtual bool requiresAdministrativePrivileges() const;
};

#endif // VPREFERENCESMODULE_H
