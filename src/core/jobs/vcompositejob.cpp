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
    if (job == 0 || d->subjobs.contains(job))
        return false;

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
    if (job == 0)
        return false;

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
