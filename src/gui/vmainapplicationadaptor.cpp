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

#include <QCoreApplication>
#include <QDebug>
#include <QStandardPaths>
#include <QStringList>
#include <QDirIterator>
#include <QDBusConnection>
#include <QDBusInterface>

#include <VApplicationInfo>

#include "vmainapplicationadaptor.h"
#include "vmainapplicationadaptor_p.h"
#include "vguiapplication.h"

/*
 * VMainApplicationAdaptorPrivate
 */

VMainApplicationAdaptorPrivate::VMainApplicationAdaptorPrivate(
    VMainApplicationAdaptor *self, const QString &identifier,
    QGuiApplication *vapp) :
    q_ptr(self),
    app(vapp),
    id(identifier),
    internalId(0)
{
    // Connect signals defined on QGuiApplication and derivates (such as QApplication)
    QObject::connect(app, SIGNAL(lastWindowClosed()), q_ptr, SLOT(_q_lastWindowClosed()));
    QObject::connect(app, SIGNAL(aboutToQuit()), q_ptr, SLOT(_q_aboutToQuit()));
}

void VMainApplicationAdaptorPrivate::_q_lastWindowClosed()
{
    Q_Q(VMainApplicationAdaptor);
    emit q->LastWindowClosed(internalId);
}

void VMainApplicationAdaptorPrivate::_q_aboutToQuit()
{
    Q_Q(VMainApplicationAdaptor);
    emit q->Quit(internalId);
}

/*
 * VMainApplicationAdaptor
 */

VMainApplicationAdaptor::VMainApplicationAdaptor(const QString &identifier, QGuiApplication *app) :
    QDBusAbstractAdaptor(app),
    d_ptr(new VMainApplicationAdaptorPrivate(this, identifier, app))
{
    // Install the event filter
    app->installEventFilter(this);

    // Check if we're connected to the session bus
    if (!QDBusConnection::sessionBus().isConnected()) {
        qWarning() << "Cannot connect to the D-Bus session bus.";
        qWarning() << "To start it, run:\n\teval `dbus-launch --auto-syntax`";
        QCoreApplication::exit(127);
    }

    // Register the service on the session bus
    QStringList parts = QCoreApplication::organizationDomain()
                        .split(QLatin1Char('.'), QString::SkipEmptyParts);
    QString reversedDomain;
    if (parts.isEmpty())
        reversedDomain = QStringLiteral("local.");
    else
        foreach (const QString & s, parts) {
            reversedDomain.prepend(QLatin1Char('.'));
            reversedDomain.prepend(s);
        }
    const QString pidSuffix = QString::number(QCoreApplication::applicationPid())
                              .prepend(QLatin1String("-"));
    const QString serviceName = reversedDomain + QCoreApplication::applicationName()
                                + pidSuffix;
    if (!QDBusConnection::sessionBus().registerService(serviceName)) {
        qWarning() << "Couldn't register \"" << qPrintable(serviceName)
                   << "\" with D-Bus, another process already owns it!";
        QCoreApplication::exit(127);
    }

    // Register this object on the session bus
    if (!QDBusConnection::sessionBus().registerObject("/MainApplication", app)) {
        qWarning() << "Couldn't register /MainApplication object with D-Bus!";
        QCoreApplication::exit(127);
    }

    // Call registrar in order to register this application
    QDBusInterface registrar("org.hawaii.Hawaii", "/Registrar");
    QDBusMessage reply = registrar.call("Register", d_ptr->id,
                                        QCoreApplication::applicationVersion());
    if (reply.type() == QDBusMessage::ErrorMessage) {
        qWarning() << qPrintable(reply.errorMessage());
        QCoreApplication::exit(127);
    }

    // Get the internal serial number
    d_ptr->internalId = reply.arguments()[0].toInt();
    if (d_ptr->internalId == 0) {
        qWarning() << "Registrar gave us an invalid serial number for"
                   << qPrintable(d_ptr->id)
                   << qPrintable(QCoreApplication::applicationVersion());
        QCoreApplication::exit(127);
    }
}

QString VMainApplicationAdaptor::identifier() const
{
    Q_D(const VMainApplicationAdaptor);
    return d->id;
}

QString VMainApplicationAdaptor::applicationName() const
{
    return QCoreApplication::applicationName();
}

QString VMainApplicationAdaptor::applicationVersion() const
{
    return QCoreApplication::applicationVersion();
}

qint64 VMainApplicationAdaptor::applicationPid() const
{
    return QCoreApplication::applicationPid();
}

QString VMainApplicationAdaptor::applicationDirPath() const
{
    return QCoreApplication::applicationFilePath();
}

QString VMainApplicationAdaptor::applicationFilePath() const
{
    return QCoreApplication::applicationFilePath();
}

QString VMainApplicationAdaptor::organizationName() const
{
    return QCoreApplication::organizationName();
}

QString VMainApplicationAdaptor::organizationDomain() const
{
    return QCoreApplication::organizationDomain();
}

void VMainApplicationAdaptor::emitDemandsAttention()
{
    Q_D(VMainApplicationAdaptor);

    emit DemandsAttention(d->internalId);
}

QString VMainApplicationAdaptor::GetDesktopFileName()
{
    Q_D(VMainApplicationAdaptor);

    // Search for the desktop file
    QString path = QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation);
    QDirIterator walker(path,
                        QDir::Files | QDir::NoDotAndDotDot | QDir::Readable,
                        QDirIterator::Subdirectories);
    while (walker.hasNext()) {
        walker.next();

        if (walker.fileInfo().completeSuffix() == "desktop") {
            // Return the desktop file if it matches
            VApplicationInfo info(walker.fileInfo().absoluteFilePath());
            if (info.isValid() && info.identifier() == identifier() && info.version() == applicationVersion())
                return info.fileName();
        }
    }

    return QString();
}

void VMainApplicationAdaptor::ReloadSettings()
{
    Q_D(VMainApplicationAdaptor);
    QMetaObject::invokeMethod(d->app, "reloadSettings", Qt::QueuedConnection);
}

void VMainApplicationAdaptor::CloseAllWindows()
{
    Q_D(VMainApplicationAdaptor);
    QMetaObject::invokeMethod(d->app, "closeAllWindows", Qt::QueuedConnection);
}

void VMainApplicationAdaptor::Quit()
{
    QCoreApplication::quit();
}

bool VMainApplicationAdaptor::eventFilter(QObject *obj, QEvent *e)
{
    Q_D(VMainApplicationAdaptor);

    if (obj == d->app) {
        // When the application is ready, emit the event
        // When the application is activate, emit the event
        if (e->type() == QEvent::ApplicationActivate)
            emit Activate(d->internalId);

        // When the application is deactivate, emit the event
        if (e->type() == QEvent::ApplicationDeactivate)
            emit Deactivate(d->internalId);
    }

    return QObject::eventFilter(obj, e);
}

#include "moc_vmainapplicationadaptor.cpp"
