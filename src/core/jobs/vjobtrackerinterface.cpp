/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 * Copyright (c) 2006 Kevin Ottens
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Kevin Ottens <ervin@kde.org>
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

#include "vjobtrackerinterface.h"
#include "vjob.h"

class VJobTrackerInterface::Private
{
public:
    Private(VJobTrackerInterface *interface) : q(interface) {

    }

    VJobTrackerInterface *const q;
};

VJobTrackerInterface::VJobTrackerInterface(QObject *parent)
    : QObject(parent), d(new Private(this))
{

}

VJobTrackerInterface::~VJobTrackerInterface()
{
    delete d;
}

void VJobTrackerInterface::registerJob(VJob *job)
{
    QObject::connect(job, SIGNAL(finished(VJob *)),
                     this, SLOT(unregisterJob(VJob *)));
    QObject::connect(job, SIGNAL(finished(VJob *)),
                     this, SLOT(finished(VJob *)));

    QObject::connect(job, SIGNAL(suspended(VJob *)),
                     this, SLOT(suspended(VJob *)));
    QObject::connect(job, SIGNAL(resumed(VJob *)),
                     this, SLOT(resumed(VJob *)));

    QObject::connect(job, SIGNAL(description(VJob *, const QString &,
                                             const QPair<QString, QString>&,
                                             const QPair<QString, QString>&)),
                     this, SLOT(description(VJob *, const QString &,
                                            const QPair<QString, QString>&,
                                            const QPair<QString, QString>&)));
    QObject::connect(job, SIGNAL(infoMessage(VJob *, QString, QString)),
                     this, SLOT(infoMessage(VJob *, QString, QString)));
    QObject::connect(job, SIGNAL(warning(VJob *, QString, QString)),
                     this, SLOT(warning(VJob *, QString, QString)));

    QObject::connect(job, SIGNAL(totalAmount(VJob *, VJob::Unit, qulonglong)),
                     this, SLOT(totalAmount(VJob *, VJob::Unit, qulonglong)));
    QObject::connect(job, SIGNAL(processedAmount(VJob *, VJob::Unit, qulonglong)),
                     this, SLOT(processedAmount(VJob *, VJob::Unit, qulonglong)));
    QObject::connect(job, SIGNAL(percent(VJob *, ulong)),
                     this, SLOT(percent(VJob *, ulong)));
    QObject::connect(job, SIGNAL(speed(VJob *, ulong)),
                     this, SLOT(speed(VJob *, ulong)));
}

void VJobTrackerInterface::unregisterJob(VJob *job)
{
    job->disconnect(this);
}

void VJobTrackerInterface::finished(VJob *job)
{
    Q_UNUSED(job)
}

void VJobTrackerInterface::suspended(VJob *job)
{
    Q_UNUSED(job)
}

void VJobTrackerInterface::resumed(VJob *job)
{
    Q_UNUSED(job)
}

void VJobTrackerInterface::description(VJob *job, const QString &title,
                                       const QPair<QString, QString> &field1,
                                       const QPair<QString, QString> &field2)
{
    Q_UNUSED(job)
    Q_UNUSED(title)
    Q_UNUSED(field1)
    Q_UNUSED(field2)

}

void VJobTrackerInterface::infoMessage(VJob *job, const QString &plain, const QString &rich)
{
    Q_UNUSED(job)
    Q_UNUSED(plain)
    Q_UNUSED(rich)
}

void VJobTrackerInterface::warning(VJob *job, const QString &plain, const QString &rich)
{
    Q_UNUSED(job)
    Q_UNUSED(plain)
    Q_UNUSED(rich)
}

void VJobTrackerInterface::totalAmount(VJob *job, VJob::Unit unit, qulonglong amount)
{
    Q_UNUSED(job)
    Q_UNUSED(unit)
    Q_UNUSED(amount)
}

void VJobTrackerInterface::processedAmount(VJob *job, VJob::Unit unit, qulonglong amount)
{
    Q_UNUSED(job)
    Q_UNUSED(unit)
    Q_UNUSED(amount)
}

void VJobTrackerInterface::percent(VJob *job, unsigned long percent)
{
    Q_UNUSED(job)
    Q_UNUSED(percent)
}

void VJobTrackerInterface::speed(VJob *job, unsigned long value)
{
    Q_UNUSED(job)
    Q_UNUSED(value)
}

#include "moc_vjobtrackerinterface.cpp"
