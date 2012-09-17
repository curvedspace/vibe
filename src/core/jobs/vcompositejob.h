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

#ifndef VCOMPOSITEJOB_H
#define VCOMPOSITEJOB_H

#include <QList>

#include <VibeCore/VGlobal>
#include <VibeCore/VJob>

class VCompositeJobPrivate;

/**
 * The base class for all jobs able to be composed of one
 * or more subjobs.
 */
class VIBE_EXPORT VCompositeJob : public VJob
{
    Q_OBJECT
public:
    /**
     * Creates a new VCompositeJob object.
     *
     * @param parent the parent QObject
     */
    explicit VCompositeJob(QObject *parent = 0);

    /**
     * Destroys a VCompositeJob object.
     */
    virtual ~VCompositeJob();

protected:
    /**
     * Add a job that has to be finished before a result
     * is emitted. This has obviously to be called before
     * the result has been emitted by the job.
     *
     * @param job the subjob to add
     * @return true if the job has been added correctly, false otherwise
     */
    virtual bool addSubjob(VJob *job);

    /**
     * Mark a sub job as being done.
     *
     * @param job the subjob to remove
     * @return true if the job has been removed correctly, false otherwise
     */
    virtual bool removeSubjob(VJob *job);

    /**
     * Checks if this job has subjobs running.
     *
     * @return true if we still have subjobs running, false otherwise
     */
    bool hasSubjobs() const;

    /**
     * Retrieves the list of the subjobs.
     *
     * @return the full list of sub jobs
     */
    const QList<VJob *> &subjobs() const;

    /**
     * Clears the list of subjobs.
     */
    void clearSubjobs();

protected Q_SLOTS:
    /**
     * Called whenever a subjob finishes.
     * Default implementation checks for errors and propagates
     * to parent job, and in all cases it calls removeSubjob.
     *
     * @param job the subjob
     */
    virtual void slotResult(VJob *job);

    /**
     * Forward signal from subjob.
     *
     * @param job the subjob
     * @param plain the info message in plain text version
     * @param rich the info message in rich text version
     * @see infoMessage()
     */
    virtual void slotInfoMessage(VJob *job, const QString &plain, const QString &rich);

protected:
    VCompositeJob(VCompositeJobPrivate &dd, QObject *parent);
private:
    Q_DECLARE_PRIVATE(VCompositeJob)
};

#endif // VCOMPOSITEJOB_H
