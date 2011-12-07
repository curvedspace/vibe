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

#include <QtCore/QCoreApplication>

#include <VibeCore/VGlobal>

#include "vpowermanagement.h"
#include "vpowermanagement_p.h"

VIBE_GLOBAL_STATIC(VPowerManagementPrivate, globalPowerManager)

VPowerManagementPrivate::VPowerManagementPrivate()
    : managerIface("org.freedesktop.PowerManagement",
                   "/org/freedesktop/PowerManagement",
                   QDBusConnection::sessionBus()),
    policyAgentIface("org.vision.Vibe.Hardware.PowerManagement.PolicyAgent",
                     "/org/vision/Vibe/Hardware/PowerManagement/PolicyAgent",
                     QDBusConnection::sessionBus()),
    inhibitIface("org.freedesktop.PowerManagement.Inhibit",
                 "/org/freedesktop/PowerManagement/Inhibit",
                 QDBusConnection::sessionBus()),
    serviceWatcher("org.vision.Vibe.Hardware.PowerManagement",
                   QDBusConnection::sessionBus(),
                   QDBusServiceWatcher::WatchForRegistration)
{
    powerSaveStatus = managerIface.GetPowerSaveStatus();

    if (managerIface.CanSuspend())
        supportedSleepStates += VHardware::PowerManagement::SuspendState;
    if (managerIface.CanHibernate())
        supportedSleepStates += VHardware::PowerManagement::HibernateState;

    connect(&managerIface, SIGNAL(CanSuspendChanged(bool)),
            this, SLOT(slotCanSuspendChanged(bool)));
    connect(&managerIface, SIGNAL(CanHibernateChanged(bool)),
            this, SLOT(slotCanHibernateChanged(bool)));
    connect(&managerIface, SIGNAL(PowerSaveStatusChanged(bool)),
            this, SLOT(slotPowerSaveStatusChanged(bool)));
    connect(&serviceWatcher, SIGNAL(serviceRegistered(QString)),
            this, SLOT(slotServiceRegistered(QString)));

    // If the service is registered, trigger the connection immediately
    if (QDBusConnection::sessionBus().interface()->isServiceRegistered("org.vision.Vibe.Hardware.PowerManagement")) {
        slotServiceRegistered("org.vision.Vibe.Hardware.PowerManagement");
    }
}

VPowerManagementPrivate::~VPowerManagementPrivate()
{
}

VHardware::PowerManagement::Notifier::Notifier()
{
}

bool VHardware::PowerManagement::appShouldConserveResources()
{
    return globalPowerManager->powerSaveStatus;
}

QSet<VHardware::PowerManagement::SleepState> VHardware::PowerManagement::supportedSleepStates()
{
    return globalPowerManager->supportedSleepStates;
}

void VHardware::PowerManagement::requestSleep(SleepState state, QObject *receiver, const char *member)
{
    Q_UNUSED(receiver)
    Q_UNUSED(member)

    if (!globalPowerManager->supportedSleepStates.contains(state))
        return;

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

int VHardware::PowerManagement::beginSuppressingSleep(const QString &reason)
{
    QDBusReply<uint> reply;
    if (globalPowerManager->policyAgentIface.isValid()) {
        reply = globalPowerManager->policyAgentIface.AddInhibition(
                    (uint)VPowerManagementPrivate::InterruptSession,
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

bool VHardware::PowerManagement::stopSuppressingSleep(int cookie)
{
    if (globalPowerManager->policyAgentIface.isValid()) {
        return globalPowerManager->policyAgentIface.ReleaseInhibition(cookie).isValid();
    } else {
        // Fallback to the fd.o Inhibit interface
        return globalPowerManager->inhibitIface.UnInhibit(cookie).isValid();
    }
}

int VHardware::PowerManagement::beginSuppressingScreenPowerManagement(const QString &reason)
{
    if (globalPowerManager->policyAgentIface.isValid()) {
        QDBusReply<uint> reply = globalPowerManager->policyAgentIface.AddInhibition(
                                     (uint)VPowerManagementPrivate::ChangeScreenSettings,
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

bool VHardware::PowerManagement::stopSuppressingScreenPowerManagement(int cookie)
{
    if (globalPowerManager->policyAgentIface.isValid()) {
        return globalPowerManager->policyAgentIface.ReleaseInhibition(cookie).isValid();
    } else {
        // No way to fallback on something, hence return failure
        return false;
    }
}

VHardware::PowerManagement::Notifier *VHardware::PowerManagement::notifier()
{
    return globalPowerManager;
}

void VPowerManagementPrivate::slotCanSuspendChanged(bool newState)
{
    if (newState)
        supportedSleepStates += VHardware::PowerManagement::SuspendState;
    else
        supportedSleepStates -= VHardware::PowerManagement::SuspendState;
}

void VPowerManagementPrivate::slotCanHibernateChanged(bool newState)
{
    if (newState)
        supportedSleepStates += VHardware::PowerManagement::HibernateState;
    else
        supportedSleepStates -= VHardware::PowerManagement::HibernateState;
}

void VPowerManagementPrivate::slotPowerSaveStatusChanged(bool newState)
{
    powerSaveStatus = newState;
    emit appShouldConserveResourcesChanged(powerSaveStatus);
}

void VPowerManagementPrivate::slotServiceRegistered(const QString &serviceName)
{
    Q_UNUSED(serviceName);

    // Is the resume signal available?
    QDBusMessage call = QDBusMessage::createMethodCall("org.vision.Vibe.Hardware.PowerManagement",
                        "/org/vision/Vibe/Hardware/PowerManagement",
                        "org.vision.Vibe.Hardware.PowerManagement",
                        "backendCapabilities");
    QDBusPendingReply< uint > reply = QDBusConnection::sessionBus().asyncCall(call);
    reply.waitForFinished();

    if (reply.isValid() && reply.value() > 0) {
        // Connect the signal
        QDBusConnection::sessionBus().connect("org.vision.Vibe.Hardware.PowerManagement",
                                              "/org/vision/Vibe/Hardware/PowerManagement",
                                              "org.vision.Vibe.Hardware.PowerManagement",
                                              "resumingFromSuspend",
                                              this,
                                              SIGNAL(resumingFromSuspend()));
    }
}

#include "vpowermanagement_p.moc"
#include "vpowermanagement.moc"
