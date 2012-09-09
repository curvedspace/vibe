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
