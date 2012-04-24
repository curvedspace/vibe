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

#include <QDebug>
#include <QStringList>
#include <QDirIterator>
#include <QDBusConnection>
#include <QDBusInterface>

#include <VStandardDirectories>
#include <VApplicationInfo>

#include "vmainapplicationadaptor.h"
#include "vmainapplicationadaptor_p.h"

using namespace VStandardDirectories;

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

    // Check if we're connected to the session bus
    if (!QDBusConnection::sessionBus().isConnected()) {
        qWarning() << "Cannot connect to the D-Bus session bus.";
        qWarning() << "To start it, run:\n\teval `dbus-launch --auto-syntax`";
        application->exit(127);
    }

    QStringList parts = organizationDomain().split(QLatin1Char('.'), QString::SkipEmptyParts);
    QString reversedDomain;
    if (parts.isEmpty())
        reversedDomain = QStringLiteral("local.");
    else
        foreach(const QString & s, parts) {
        reversedDomain.prepend(QLatin1Char('.'));
        reversedDomain.prepend(s);
    }
    const QString pidSuffix = QString::number(applicationPid()).prepend(QLatin1String("-"));
    const QString serviceName = reversedDomain + applicationName() + pidSuffix;
    if (!QDBusConnection::sessionBus().registerService(serviceName)) {
        qWarning() << "Couldn't register \"" << qPrintable(serviceName)
                   << "\" with D-Bus, another process already owns it!";
        application->exit(127);
    }

    // Register this object on the session bus
    if (!QDBusConnection::sessionBus().registerObject(QStringLiteral("/MainApplication"), application)) {
        qWarning() << "Couldn't register /MainApplication object with D-Bus!";
        application->exit(127);
    }

    // Register this application
    QDBusInterface registrar("org.maui.Mirage", "/Registrar");
    qDebug() << registrar.call("Register", identifier, applicationVersion());
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

qint64 VMainApplicationAdaptor::applicationPid() const
{
    Q_D(const VMainApplicationAdaptor);
    return d->app->applicationPid();
}

QString VMainApplicationAdaptor::desktopFileName() const
{
    Q_D(const VMainApplicationAdaptor);

    // Search for the desktop file
    QStringList paths =
        QStringList() << findDirectory(SystemApplicationsDirectory)
        << findDirectory(CommonApplicationsDirectory)
        << findDirectory(UserApplicationsDirectory);
    foreach(QString path, paths) {
        QDirIterator walker(path,
                            QDir::Files | QDir::NoDotAndDotDot | QDir::Readable,
                            QDirIterator::Subdirectories);
        while (walker.hasNext()) {
            walker.next();

            if (walker.fileInfo().completeSuffix() == "desktop") {
                // Return the desktop file if it matches
                VApplicationInfo info(walker.fileInfo().absoluteFilePath());
                if (info.isValid() && info.identifier() == d->id && info.version() == d->app->applicationVersion())
                    return info.fileName();
            }
        }
    }

    return QString();
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
