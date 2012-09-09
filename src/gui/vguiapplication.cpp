/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
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

#include <QEvent>
#include <QWindow>

#include "vguiapplication.h"
#include "vguiapplication_p.h"
#include "vmainapplicationadaptor.h"

/*
 * VGuiApplicationPrivate
 */

VGuiApplicationPrivate::VGuiApplicationPrivate() :
    adaptor(0)
{
}

/*
 * VGuiApplication
 */

VGuiApplication::VGuiApplication(int &argc, char **argv) :
    QGuiApplication(argc, argv),
    d_ptr(new VGuiApplicationPrivate)
{
}

QString VGuiApplication::identifier() const
{
    Q_D(const VGuiApplication);

    return d->id;
}

void VGuiApplication::setIdentifier(const QString &id)
{
    Q_D(VGuiApplication);

    Q_ASSERT(!applicationName().isEmpty());
    Q_ASSERT(!applicationVersion().isEmpty());
    Q_ASSERT(!organizationName().isEmpty());
    Q_ASSERT(!organizationDomain().isEmpty());
    Q_ASSERT(identifier().isEmpty());

    // Create the D-Bus adaptor that wraps this object
    d->id = id;
    d->adaptor = new VMainApplicationAdaptor(d->id, this);
}

void VGuiApplication::demandsAttention()
{
    Q_D(VGuiApplication);

    if (d->adaptor)
        d->adaptor->emitDemandsAttention();
}

void VGuiApplication::reloadSettings()
{
}

void VGuiApplication::closeAllWindows()
{
    QWindowList list = QGuiApplication::allWindows();
    for (int i = 0; i < list.size(); i++)
        list.at(i)->close();
}

#include "moc_vguiapplication.cpp"
