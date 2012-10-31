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

#include <QEvent>
#include <QWindow>

#include "vapplication.h"
#include "vapplication_p.h"
#include "vmainapplicationadaptor.h"

/*
 * VApplicationPrivate
 */

VApplicationPrivate::VApplicationPrivate() :
    adaptor(0)
{
}

/*
 * VApplication
 */

VApplication::VApplication(int &argc, char **argv) :
    QApplication(argc, argv),
    d_ptr(new VApplicationPrivate)
{
}

QString VApplication::identifier() const
{
    Q_D(const VApplication);

    return d->id;
}

void VApplication::setIdentifier(const QString &id)
{
    Q_D(VApplication);

    Q_ASSERT(!applicationName().isEmpty());
    Q_ASSERT(!applicationVersion().isEmpty());
    Q_ASSERT(!organizationName().isEmpty());
    Q_ASSERT(!organizationDomain().isEmpty());
    Q_ASSERT(identifier().isEmpty());

    // Create the D-Bus adaptor that wraps this object
    d->id = id;
    d->adaptor = new VMainApplicationAdaptor(d->id, this);
}

void VApplication::demandsAttention()
{
    Q_D(VApplication);

    if (d->adaptor)
        d->adaptor->emitDemandsAttention();
}

void VApplication::reloadSettings()
{
}

void VApplication::closeAllWindows()
{
    QWindowList list = QGuiApplication::allWindows();
    for (int i = 0; i < list.size(); i++)
        list.at(i)->close();
}

#include "moc_vapplication.cpp"
