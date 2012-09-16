/*  This file is part of the KDE project
    Copyright (C) 2006 Kevin Ottens <ervin@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License version 2 as published by the Free Software Foundation.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.

*/

#include "vcompositejob.h"
#include "vcompositejob_p.h"

VCompositeJobPrivate::VCompositeJobPrivate()
{
}

VCompositeJobPrivate::~VCompositeJobPrivate()
{
    qDeleteAll(subjobs);
}

VCompositeJob::VCompositeJob(QObject *parent)
    : VJob(*new VCompositeJobPrivate, parent)
{
}

VCompositeJob::VCompositeJob(VCompositeJobPrivate &dd, QObject *parent)
    : VJob(dd, parent)
{
}

VCompositeJob::~VCompositeJob()
{
}

bool VCompositeJob::addSubjob(VJob *job)
{
    Q_D(VCompositeJob);
    if (job == 0 || d->subjobs.contains(job)) {
        return false;
    }

    d->subjobs.append(job);

    connect(job, SIGNAL(result(VJob *)),
            SLOT(slotResult(VJob *)));

    // Forward information from that subjob.
    connect(job, SIGNAL(infoMessage(VJob *, QString, QString)),
            SLOT(slotInfoMessage(VJob *, QString, QString)));

    return true;
}

bool VCompositeJob::removeSubjob(VJob *job)
{
    Q_D(VCompositeJob);
    if (job == 0) {
        return false;
    }

    d->subjobs.removeAll(job);

    return true;
}

bool VCompositeJob::hasSubjobs() const
{
    return !d_func()->subjobs.isEmpty();
}

const QList<VJob *> &VCompositeJob::subjobs() const
{
    return d_func()->subjobs;
}

void VCompositeJob::clearSubjobs()
{
    Q_D(VCompositeJob);
    d->subjobs.clear();
}

void VCompositeJob::slotResult(VJob *job)
{
    // Did job have an error ?
    if (job->error() && !error()) {
        // Store it in the parent only if first error
        setError(job->error());
        setErrorText(job->errorText());
        emitResult();
    }

    removeSubjob(job);
}

void VCompositeJob::slotInfoMessage(VJob *job, const QString &plain, const QString &rich)
{
    Q_EMIT infoMessage(job, plain, rich);
}

#include "moc_vcompositejob.cpp"
