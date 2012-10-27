/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
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

#ifndef VPREFERENCESMODULEPLUGIN_H
#define VPREFERENCESMODULEPLUGIN_H

#include <QObject>
#include <QtCore/qplugin.h>

#include <VibeWidgets/VibeWidgetsExport>

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
class VIBEWIDGETS_EXPORT VPreferencesModulePlugin : public QObject
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
