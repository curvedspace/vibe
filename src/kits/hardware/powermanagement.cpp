/*
    Copyright 2006-2007 Kevin Ottens <ervin@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "powermanagement.h"
#include "powermanagement_p.h"

#include "soliddefs_p.h"

#include <QtCore/QCoreApplication>

SOLID_GLOBAL_STATIC(QubeHardware::PowerManagementPrivate, globalPowerManager)

QubeHardware::PowerManagementPrivate::PowerManagementPrivate()
    : managerIface("org.freedesktop.PowerManagement",
                   "/org/freedesktop/PowerManagement",
                   QDBusConnection::sessionBus()),
    policyAgentIface("org.vision.Hardware.PowerManagement.PolicyAgent",
                     "/org.vision.Hardware/PowerManagement/PolicyAgent",
                     QDBusConnection::sessionBus()),
    inhibitIface("org.freedesktop.PowerManagement.Inhibit",
                 "/org/freedesktop/PowerManagement/Inhibit",
                 QDBusConnection::sessionBus()),
    serviceWatcher("org.vision.Hardware.PowerManagement",
                   QDBusConnection::sessionBus(),
                   QDBusServiceWatcher::WatchForRegistration)
{
    powerSaveStatus = managerIface.GetPowerSaveStatus();

    if (managerIface.CanSuspend())
        supportedSleepStates+= QubeHardware::PowerManagement::SuspendState;
    if (managerIface.CanHibernate())
        supportedSleepStates+= QubeHardware::PowerManagement::HibernateState;

    connect(&managerIface, SIGNAL(CanSuspendChanged(bool)),
            this, SLOT(slotCanSuspendChanged(bool)));
    connect(&managerIface, SIGNAL(CanHibernateChanged(bool)),
            this, SLOT(slotCanHibernateChanged(bool)));
    connect(&managerIface, SIGNAL(PowerSaveStatusChanged(bool)),
            this, SLOT(slotPowerSaveStatusChanged(bool)));
    connect(&serviceWatcher, SIGNAL(serviceRegistered(QString)),
            this, SLOT(slotServiceRegistered(QString)));

    // If the service is registered, trigger the connection immediately
    if (QDBusConnection::sessionBus().interface()->isServiceRegistered("org.vision.Hardware.PowerManagement")) {
        slotServiceRegistered("org.vision.Hardware.PowerManagement");
    }
}

QubeHardware::PowerManagementPrivate::~PowerManagementPrivate()
{
}

QubeHardware::PowerManagement::Notifier::Notifier()
{
}

bool QubeHardware::PowerManagement::appShouldConserveResources()
{
    return globalPowerManager->powerSaveStatus;
}

QSet<QubeHardware::PowerManagement::SleepState> QubeHardware::PowerManagement::supportedSleepStates()
{
    return globalPowerManager->supportedSleepStates;
}

void QubeHardware::PowerManagement::requestSleep(SleepState state, QObject *receiver, const char *member)
{
    Q_UNUSED(receiver)
    Q_UNUSED(member)

    if (!globalPowerManager->supportedSleepStates.contains(state)) {
        return;
    }

    switch (state) {
    case StandbyState:
        break;
    case SuspendState:
        globalPowerManager->managerIface.Suspend();
        break;
    case HibernateState:
        globalPowerManager->managerIface.Hibernate();
        break;
    }
}

int QubeHardware::PowerManagement::beginSuppressingSleep(const QString &reason)
{
    QDBusReply<uint> reply;
    if (globalPowerManager->policyAgentIface.isValid()) {
        reply = globalPowerManager->policyAgentIface.AddInhibition(
                    (uint)PowerManagementPrivate::InterruptSession,
                    QCoreApplication::applicationName(), reason);
    } else {
        // Fallback to the fd.o Inhibit interface
        reply = globalPowerManager->inhibitIface.Inhibit(QCoreApplication::applicationName(), reason);
    }

    if (reply.isValid())
        return reply;
    else
        return -1;
}

bool QubeHardware::PowerManagement::stopSuppressingSleep(int cookie)
{
    if (globalPowerManager->policyAgentIface.isValid()) {
        return globalPowerManager->policyAgentIface.ReleaseInhibition(cookie).isValid();
    } else {
        // Fallback to the fd.o Inhibit interface
        return globalPowerManager->inhibitIface.UnInhibit(cookie).isValid();
    }
}

int QubeHardware::PowerManagement::beginSuppressingScreenPowerManagement(const QString& reason)
{
    if (globalPowerManager->policyAgentIface.isValid()) {
        QDBusReply<uint> reply = globalPowerManager->policyAgentIface.AddInhibition(
                                     (uint)PowerManagementPrivate::ChangeScreenSettings,
                                     QCoreApplication::applicationName(), reason);

        if (reply.isValid())
            return reply;
        else
            return -1;
    } else {
        // No way to fallback on something, hence return failure
        return -1;
    }
}

bool QubeHardware::PowerManagement::stopSuppressingScreenPowerManagement(int cookie)
{
    if (globalPowerManager->policyAgentIface.isValid()) {
        return globalPowerManager->policyAgentIface.ReleaseInhibition(cookie).isValid();
    } else {
        // No way to fallback on something, hence return failure
        return false;
    }
}

QubeHardware::PowerManagement::Notifier *QubeHardware::PowerManagement::notifier()
{
    return globalPowerManager;
}

void QubeHardware::PowerManagementPrivate::slotCanSuspendChanged(bool newState)
{
    if (newState) {
        supportedSleepStates+= QubeHardware::PowerManagement::SuspendState;
    } else {
        supportedSleepStates-= QubeHardware::PowerManagement::SuspendState;
    }
}

void QubeHardware::PowerManagementPrivate::slotCanHibernateChanged(bool newState)
{
    if (newState) {
        supportedSleepStates+= QubeHardware::PowerManagement::HibernateState;
    } else {
        supportedSleepStates-= QubeHardware::PowerManagement::HibernateState;
    }
}

void QubeHardware::PowerManagementPrivate::slotPowerSaveStatusChanged(bool newState)
{
    powerSaveStatus = newState;
    emit appShouldConserveResourcesChanged(powerSaveStatus);
}

void QubeHardware::PowerManagementPrivate::slotServiceRegistered(const QString &serviceName)
{
    Q_UNUSED(serviceName);

    // Is the resume signal available?
    QDBusMessage call = QDBusMessage::createMethodCall("org.vision.Hardware.PowerManagement",
                        "/org.vision.Hardware/PowerManagement",
                        "org.vision.Hardware.PowerManagement",
                        "backendCapabilities");
    QDBusPendingReply< uint > reply = QDBusConnection::sessionBus().asyncCall(call);
    reply.waitForFinished();

    if (reply.isValid() && reply.value() > 0) {
        // Connect the signal
        QDBusConnection::sessionBus().connect("org.vision.Hardware.PowerManagement",
                                              "/org.vision.Hardware/PowerManagement",
                                              "org.vision.Hardware.PowerManagement",
                                              "resumingFromSuspend",
                                              this,
                                              SIGNAL(resumingFromSuspend()));
    }
}
