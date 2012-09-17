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

#ifndef VJOBTRACKERINTERFACE_H
#define VJOBTRACKERINTERFACE_H

#include <QObject>
#include <QPair>

#include <VibeCore/VGlobal>
#include <VibeCore/VJob>

/**
 * The interface to implement to track the progresses of a job.
 */
class VIBE_EXPORT VJobTrackerInterface : public QObject
{
    Q_OBJECT
public:
    /**
     * Creates a new VJobTrackerInterface
     *
     * @param parent the parent object
     */
    VJobTrackerInterface(QObject *parent = 0);

    /**
     * Destroys a VJobTrackerInterface
     */
    virtual ~VJobTrackerInterface();

public Q_SLOTS:
    /**
     * Register a new job in this tracker.
     *
     * @param job the job to register
     */
    virtual void registerJob(VJob *job);

    /**
     * Unregister a job from this tracker.
     *
     * @param job the job to unregister
     */
    virtual void unregisterJob(VJob *job);

protected Q_SLOTS:
    /**
     * Called when a job is finished, in any case. It is used to notify
     * that the job is terminated and that progress UI (if any) can be hidden.
     *
     * @param job the job that emitted this signal
     */
    virtual void finished(VJob *job);

    /**
     * Called when a job is suspended.
     *
     * @param job the job that emitted this signal
     */
    virtual void suspended(VJob *job);

    /**
     * Called when a job is resumed.
     *
     * @param job the job that emitted this signal
     */
    virtual void resumed(VJob *job);

    /**
     * Called to display general description of a job. A description has
     * a title and two optional fields which can be used to complete the
     * description.
     *
     * Examples of titles are "Copying", "Creating resource", etc.
     * The fields of the description can be "Source" with an URL, and,
     * "Destination" with an URL for a "Copying" description.
     * @param job the job that emitted this signal
     * @param title the general description of the job
     * @param field1 first field (localized name and value)
     * @param field2 second field (localized name and value)
     */
    virtual void description(VJob *job, const QString &title,
                             const QPair<QString, QString> &field1,
                             const QPair<QString, QString> &field2);

    /**
     * Called to display state information about a job.
     * Examples of message are "Resolving host", "Connecting to host...", etc.
     *
     * @param job the job that emitted this signal
     * @param plain the info message
     * @param rich the rich text version of the message, or QString() is none is available
     */
    virtual void infoMessage(VJob *job, const QString &plain, const QString &rich);

    /**
     * Emitted to display a warning about a job.
     *
     * @param job the job that emitted this signal
     * @param plain the warning message
     * @param rich the rich text version of the message, or QString() is none is available
     */
    virtual void warning(VJob *job, const QString &plain, const QString &rich);

    /**
     * Called when we know the amount a job will have to process. The unit of this
     * amount is provided too. It can be called several times for a given job if the job
     * manages several different units.
     *
     * @param job the job that emitted this signal
     * @param unit the unit of the total amount
     * @param amount the total amount
     */
    virtual void totalAmount(VJob *job, VJob::Unit unit, qulonglong amount);

    /**
     * Regularly called to show the progress of a job by giving the current amount.
     * The unit of this amount is provided too. It can be called several times for a given
     * job if the job manages several different units.
     *
     * @param job the job that emitted this signal
     * @param unit the unit of the processed amount
     * @param amount the processed amount
     */
    virtual void processedAmount(VJob *job, VJob::Unit unit, qulonglong amount);

    /**
     * Called to show the overall progress of the job.
     * Note that this is not called for finished jobs.
     *
     * @param job the job that emitted this signal
     * @param percent the percentage
     */
    virtual void percent(VJob *job, unsigned long percent);

    /**
     * Called to show the speed of the job.
     *
     * @param job the job that emitted this signal
     * @param value the current speed of the job
     */
    virtual void speed(VJob *job, unsigned long value);

private:
    class Private;
    Private *const d;
};

#endif // VJOBTRACKERINTERFACE_H