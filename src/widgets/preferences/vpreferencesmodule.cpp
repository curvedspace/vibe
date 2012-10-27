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

#include "vpreferencesmodule.h"
#include "vpreferencesmodule_p.h"

/*
 * VPreferencesModulePrivate
 */

VPreferencesModulePrivate::VPreferencesModulePrivate() :
    firstShow(true)
{
}

/*
 * VPreferencesModule
 */

VPreferencesModule::VPreferencesModule(QWidget *parent) :
    QWidget(parent),
    d_ptr(new VPreferencesModulePrivate)
{
}

bool VPreferencesModule::requireAdministrativePrivileges() const
{
    return false;
}

QString VPreferencesModule::useAdministratorOnlyMessage() const
{
    return QString("<b>Changes are locked!</b>");
}

void VPreferencesModule::load()
{
}

void VPreferencesModule::save()
{
}

void VPreferencesModule::defaults()
{
}

void VPreferencesModule::showEvent(QShowEvent *event)
{
    Q_D(VPreferencesModule);

    // When this module is first shown, load settings and emit changed(bool) signal
    if (d->firstShow) {
        d->firstShow = false;
        QMetaObject::invokeMethod(this, "load", Qt::QueuedConnection);
        QMetaObject::invokeMethod(this, "changed", Qt::QueuedConnection, Q_ARG(bool, false));
    }

    QWidget::showEvent(event);
}

#include "moc_vpreferencesmodule.cpp"
