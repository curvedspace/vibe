/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
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

#include "vmainapplicationadaptor.h"
#include "vmainapplicationadaptor_p.h"

/*
 * VMainApplicationAdaptorPrivate
 */

VMainApplicationAdaptorPrivate::VMainApplicationAdaptorPrivate(const QString &identifier,
                                                               QGuiApplication *application) :
    id(identifier),
    app(application)
{
}

/*
 * VMainApplicationAdaptor
 */

VMainApplicationAdaptor::VMainApplicationAdaptor(const QString &identifier,
                                                 QGuiApplication *application) :
    QDBusAbstractAdaptor(application),
    d_ptr(new VMainApplicationAdaptorPrivate(identifier, application))
{
    connect(application, SIGNAL(aboutToQuit()), SIGNAL(aboutToQuit()));
}

QString VMainApplicationAdaptor::identifier() const
{
    Q_D(const VMainApplicationAdaptor);
    return d->id;
}

QString VMainApplicationAdaptor::applicationName() const
{
    Q_D(const VMainApplicationAdaptor);
    return d->app->applicationName();
}

QString VMainApplicationAdaptor::applicationVersion() const
{
    Q_D(const VMainApplicationAdaptor);
    return d->app->applicationVersion();
}

QString VMainApplicationAdaptor::organizationName() const
{
    Q_D(const VMainApplicationAdaptor);
    return d->app->organizationName();
}

QString VMainApplicationAdaptor::organizationDomain() const
{
    Q_D(const VMainApplicationAdaptor);
    return d->app->organizationDomain();
}

void VMainApplicationAdaptor::quit()
{
    Q_D(VMainApplicationAdaptor);
    d->app->quit();
}

void VMainApplicationAdaptor::reloadSettings()
{
}

#include "moc_vmainapplicationadaptor.cpp"
