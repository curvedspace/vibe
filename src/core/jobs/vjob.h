/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 * Copyright (c) 2006 Kevin Ottens
 * Copyright (c) 2000 Stephan Kulow
 * Copyright (c) 2000 David Faure
 *
 * Author(s):
 *    David Faure <faure@kde.org>
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Stephan Kulow <coolo@kde.org>
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

#ifndef VJOB_H
#define VJOB_H

#include <VibeCore/VibeCoreExport>

#include <QObject>
#include <QPair>

class VJobUiDelegate;
class VJobPrivate;

/**
 * The base class for all jobs.
 * For all jobs created in an application, the code looks like
 *
 * \code
 * void SomeClass::methodWithAsynchronousJobCall()
 * {
 *   VJob * job = someoperation( some parameters );
 *   connect( job, SIGNAL( result( VJob * ) ),
 *            this, SLOT( handleResult( VJob * ) ) );
 *   job->start();
 * }
 * \endcode
 *   (other connects, specific to the job)
 *
 * And handleResult is usually at least:
 *
 * \code
 * void SomeClass::handleResult( VJob *job )
 * {
 *   if ( job->error() )
 *       doSomething();
 * }
 * \endcode
 *
 * With the synchronous interface the code looks like
 *
 * \code
 * void SomeClass::methodWithSynchronousJobCall()
 * {
 *   VJob *job = someoperation( some parameters );
 *   if ( !job->exec() )
 *   {
 *       // An error occurred
 *   }
 *   else
 *   {
 *       // Do something
 *   }
 * }
 * \endcode
 *
 * Subclasses must implement start(), which should trigger
 * the execution of the job (although the work should be
 * done asynchronously).  errorString() should also be
 * reimplemented by any subclasses that introduce new
 * error codes.
 *
 * @note: VJob and its subclasses are meant to be used
 * in a fire-and-forget way. Jobs will delete themselves
 * when they finish using deleteLater() (although this
 * behaviour can be changed), so a job instance will
 * disappear after the next event loop run.
 */
class VIBECORE_EXPORT VJob : public QObject
{
    Q_OBJECT
    Q_ENUMS(KillVerbosity Capability Unit)
    Q_FLAGS(Capabilities)
public:
    enum Unit { Bytes, Files, Directories };

    enum Capability { NoCapabilities = 0x0000,
                      Killable       = 0x0001,
                      Suspendable    = 0x0002
                    };

    Q_DECLARE_FLAGS(Capabilities, Capability)

    /**
     * Creates a new VJob object.
     *
     * @param parent the parent QObject
     */
    explicit VJob(QObject *parent = 0);

    /**
     * Destroys a VJob object.
     */
    virtual ~VJob();

    /**
     * Attach a UI delegate to this job.
     *
     * If the job had another UI delegate, it's automatically deleted. Once
     * attached to the job, the UI delegate will be deleted with the job.
     *
     * @param delegate the new UI delegate to use
     * @see VJobUiDelegate
     */
    void setUiDelegate(VJobUiDelegate *delegate);

    /**
     * Retrieves the delegate attached to this job.
     *
     * @return the delegate attached to this job, or 0 if there's no such delegate
     */
    VJobUiDelegate *uiDelegate() const;

    /**
     * Returns the capabilities of this job.
     *
     * @return the capabilities that this job supports
     * @see setCapabilities()
     */
    Capabilities capabilities() const;

    /**
     * Returns if the job was suspended with the suspend() call.
     *
     * @return if the job was suspended
     * @see suspend() resume()
     */
    bool isSuspended() const;

    /**
     * Starts the job asynchronously.
     *
     * When the job is finished, result() is emitted.
     *
     * Warning: Never implement any synchronous workload in this method. This method
     * should just trigger the job startup, not do any work itself. It is expected to
     * be non-blocking.
     *
     * This is the method all subclasses need to implement.
     * It should setup and trigger the workload of the job. It should not do any
     * work itself. This includes all signals and terminating the job, e.g. by
     * emitResult(). The workload, which could be another method of the
     * subclass, is to be triggered using the event loop, e.g. by code like:
     * \code
     * void ExampleJob::start()
     * {
     *  QTimer::singleShot( 0, this, SLOT( doWork() ) );
     * }
     * \endcode
     */
    virtual void start() = 0;

    enum KillVerbosity { Quietly, EmitResult };

public Q_SLOTS:
    /**
     * Aborts this job.
     *
     * This kills and deletes the job.
     *
     * @param verbosity if equals to EmitResult, Job will emit signal result
     * and ask uiserver to close the progress window.
     * @p verbosity is set to EmitResult for subjobs. Whether applications
     * should call with Quietly or EmitResult depends on whether they rely
     * on result being emitted or not. Please notice that if @p verbosity is
     * set to Quietly, signal result will NOT be emitted.
     * @return true if the operation is supported and succeeded, false otherwise
     */
    bool kill(KillVerbosity verbosity = Quietly);

    /**
     * Suspends this job.
     * The job should be kept in a state in which it is possible to resume it.
     *
     * @return true if the operation is supported and succeeded, false otherwise
     */
    bool suspend();

    /**
     * Resumes this job.
     *
     * @return true if the operation is supported and succeeded, false otherwise
     */
    bool resume();

protected:
    /**
     * Aborts this job quietly.
     *
     * This simply kills the job, no error reporting or job deletion should be involved.
     *
     * @return true if the operation is supported and succeeded, false otherwise
     */
    virtual bool doKill();

    /**
     * Suspends this job.
     *
     * @return true if the operation is supported and succeeded, false otherwise
     */
    virtual bool doSuspend();

    /**
     * Resumes this job.
     *
     * @return true if the operation is supported and succeeded, false otherwise
     */
    virtual bool doResume();

    /**
     * Sets the capabilities for this job.
     *
     * @param capabilities are the capabilities supported by this job
     * @see capabilities()
     */
    void setCapabilities(Capabilities capabilities);

public:
    /**
     * Executes the job synchronously.
     *
     * This will start a nested QEventLoop internally. Nested event loop can be dangerous and
     * can have unintended side effects, you should avoid calling exec() whenever you can and use the
     * asynchronous interface of VJob instead.
     *
     * Should you indeed call this method, you need to make sure that all callers are reentrant,
     * so that events delivered by the inner event loop don't cause non-reentrant functions to be
     * called, which usually wreaks havoc.
     *
     * Note that the event loop started by this method does not process user input events, which means
     * your user interface will effectivly be blocked. Other events like paint or network events are
     * still being processed. The advantage of not processing user input events is that the chance of
     * accidental reentrancy is greatly reduced. Still you should avoid calling this function.
     *
     * @return true if the job has been executed without error, false otherwise
     */
    bool exec();

    enum {
        /*** Indicates there is no error */
        NoError = 0,
        /*** Indicates the job was killed */
        KilledJobError = 1,
        /*** Subclasses should define error codes starting at this value */
        UserDefinedError = 100
    };

    /**
     * Returns the error code, if there has been an error.
     *
     * Only call this method from the slot connected to result().
     *
     * @return the error code for this job, 0 if no error.
     */
    int error() const;

    /**
     * Returns the error text if there has been an error.
     *
     * Only call if error is not 0.
     *
     * This is usually some extra data associated with the error,
     * such as a URL.  Use errorString() to get a human-readable,
     * translated message.
     *
     * @return a string to help understand the error
     */
    QString errorText() const;

    /**
     * A human-readable error message.
     *
     * This provides a translated, human-readable description of the
     * error.  Only call if error is not 0.
     *
     * Subclasses should implement this to create a translated
     * error message from the error code and error text.
     * For example:
     * \code
     * if (error() == ReadFailed)
     *   i18n( "Could not read \"%1\"", errorText() );
     * \endcode
     *
     * @return a translated error message, providing error() is 0
     */
    virtual QString errorString() const;


    /**
     * Returns the processed amount of a given unit for this job.
     *
     * @param unit the unit of the requested amount
     * @return the processed size
     */
    qulonglong processedAmount(Unit unit) const;

    /**
     * Returns the total amount of a given unit for this job.
     *
     * @param unit the unit of the requested amount
     * @return the total size
     */
    qulonglong totalAmount(Unit unit) const;

    /**
     * Returns the overall progress of this job.
     *
     * @return the overall progress of this job
     */
    unsigned long percent() const;

    /**
     * set the auto-delete property of the job. If @p autodelete is
     * set to false the job will not delete itself once it is finished.
     *
     * The default for any VJob is to automatically delete itself.
     *
     * @param autodelete set to false to disable automatic deletion
     * of the job.
     */
    void setAutoDelete(bool autodelete);

    /**
     * Returns whether this job automatically deletes itself once
     * the job is finished.
     *
     * @return whether the job is deleted automatically after
     * finishing.
     */
    bool isAutoDelete() const;

Q_SIGNALS:
#if !defined(Q_MOC_RUN)
private: // don't tell moc, but those signals are in fact private
#endif
    /**
     * Emitted when the job is finished, in any case. It is used to notify
     * observers that the job is terminated and that progress can be hidden.
     *
     * This is a private signal, it can't be emitted directly by subclasses of
     * VJob, use emitResult() instead.
     *
     * Client code is not supposed to connect to this signal, signal result should
     * be used instead.
     *
     * If you store a list of jobs and they might get killed silently,
     * then you must connect to this instead of result().
     *
     * @param job the job that emitted this signal
     * @internal
     *
     * @see result
     */
    void finished(VJob *job);

    /**
     * Emitted when the job is suspended.
     *
     * This is a private signal, it can't be emitted directly by subclasses of
     * VJob.
     *
     * @param job the job that emitted this signal
     */
    void suspended(VJob *job);

    /**
     * Emitted when the job is resumed.
     *
     * This is a private signal, it can't be emitted directly by subclasses of
     * VJob.
     *
     * @param job the job that emitted this signal
     */
    void resumed(VJob *job);

    /**
     * Emitted when the job is finished (except when killed with VJob::Quietly).
     *
     * Use error to know if the job was finished with error.
     *
     * This is a private signal, it can't be emitted directly by subclasses of
     * VJob, use emitResult() instead.
     *
     * Please connect to this signal instead of finished.
     *
     * @param job the job that emitted this signal
     *
     * @see kill
     */
    void result(VJob *job);

Q_SIGNALS:
    /**
     * Emitted to display general description of this job. A description has
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
    void description(VJob *job, const QString &title,
                     const QPair<QString, QString> &field1 = qMakePair(QString(), QString()),
                     const QPair<QString, QString> &field2 = qMakePair(QString(), QString()));

    /**
     * Emitted to display state information about this job.
     * Examples of message are "Resolving host", "Connecting to host...", etc.
     *
     * @param job the job that emitted this signal
     * @param plain the info message
     * @param rich the rich text version of the message, or QString() is none is available
     */
    void infoMessage(VJob *job, const QString &plain, const QString &rich = QString());

    /**
     * Emitted to display a warning about this job.
     *
     * @param job the job that emitted this signal
     * @param plain the warning message
     * @param rich the rich text version of the message, or QString() is none is available
     */
    void warning(VJob *job, const QString &plain, const QString &rich = QString());


Q_SIGNALS:
#if !defined(Q_MOC_RUN) && !defined(DOXYGEN_SHOULD_SKIP_THIS) && !defined(IN_IDE_PARSER)
private: // don't tell moc, doxygen or kdevelop, but those signals are in fact private
#endif
    /**
     * Emitted when we know the amount the job will have to process. The unit of this
     * amount is sent too. It can be emitted several times if the job manages several
     * different units.
     *
     * This is a private signal, it can't be emitted directly by subclasses of
     * VJob, use setTotalAmount() instead.
     *
     * @param job the job that emitted this signal
     * @param unit the unit of the total amount
     * @param amount the total amount
     */
    void totalAmount(VJob *job, VJob::Unit unit, qulonglong amount);

    /**
     * Regularly emitted to show the progress of this job by giving the current amount.
     * The unit of this amount is sent too. It can be emitted several times if the job
     * manages several different units.
     *
     * This is a private signal, it can't be emitted directly by subclasses of
     * VJob, use setProcessedAmount() instead.
     *
     * @param job the job that emitted this signal
     * @param unit the unit of the processed amount
     * @param amount the processed amount
     */
    void processedAmount(VJob *job, VJob::Unit unit, qulonglong amount);

    /**
     * Emitted when we know the size of this job (data size in bytes for transfers,
     * number of entries for listings, etc).
     *
     * This is a private signal, it can't be emitted directly by subclasses of
     * VJob, use setTotalAmount() instead.
     *
     * @param job the job that emitted this signal
     * @param size the total size
     */
    void totalSize(VJob *job, qulonglong size);

    /**
     * Regularly emitted to show the progress of this job
     * (current data size in bytes for transfers, entries listed, etc.).
     *
     * This is a private signal, it can't be emitted directly by subclasses of
     * VJob, use setProcessedAmount() instead.
     *
     * @param job the job that emitted this signal
     * @param size the processed size
     */
    void processedSize(VJob *job, qulonglong size);

    /**
     * Progress signal showing the overall progress of the job
     * This is valid for any kind of job, and allows using a
     * a progress bar very easily. (see KProgressBar).
     * Note that this signal is not emitted for finished jobs.
     *
     * This is a private signal, it can't be emitted directly by subclasses of
     * VJob, use emitPercent(), setPercent() setTotalAmount() or
     * setProcessedAmount() instead.
     *
     * @param job the job that emitted this signal
     * @param percent the percentage
     */
    void percent(VJob *job, unsigned long percent);

    /**
     * Emitted to display information about the speed of this job.
     *
     * This is a private signal, it can't be emitted directly by subclasses of
     * VJob, use emitSpeed() instead.
     *
     * @param job the job that emitted this signal
     * @param speed the speed in bytes/s
     */
    void speed(VJob *job, unsigned long speed);

protected:
    /**
     * Sets the error code.
     *
     * It should be called when an error
     * is encountered in the job, just before calling emitResult().
     *
     * You should define an (anonymous) enum of error codes,
     * with values starting at VJob::UserDefinedError, and use
     * those.  For example,
     * @code
     * enum {
     *   InvalidFoo = UserDefinedError,
     *   BarNotFound
     * };
     * @endcode
     *
     * @param errorCode the error code
     * @see emitResult()
     */
    void setError(int errorCode);

    /**
     * Sets the error text.
     *
     * It should be called when an error
     * is encountered in the job, just before calling emitResult().
     *
     * Provides extra information about the error that cannot be
     * determined directly from the error code.  For example, a
     * URL or filename.  This string is not normally translatable.
     *
     * @param errorText the error text
     * @see emitResult(), errorString(), setError()
     */
    void setErrorText(const QString &errorText);


    /**
     * Sets the processed size. The processedAmount() and percent() signals
     * are emitted if the values changed. The percent() signal is emitted
     * only for the progress unit.
     *
     * @param unit the unit of the new processed amount
     * @param amount the new processed amount
     */
    void setProcessedAmount(Unit unit, qulonglong amount);

    /**
     * Sets the total size. The totalSize() and percent() signals
     * are emitted if the values changed. The percent() signal is emitted
     * only for the progress unit.
     *
     * @param unit the unit of the new total amount
     * @param amount the new total amount
     */
    void setTotalAmount(Unit unit, qulonglong amount);

    /**
     * Sets the overall progress of the job. The percent() signal
     * is emitted if the value changed.
     *
     * @param percentage the new overall progress
     */
    void setPercent(unsigned long percentage);


    /**
     * Utility function to emit the result signal, and suicide this job.
     * It first notifies the observers to hide the progress for this job using
     * the finished() signal.
     *
     * @note: Deletes this job using deleteLater().
     *
     * @see result()
     * @see finished()
     */
    void emitResult();

    /**
     * Utility function for inherited jobs.
     * Emits the percent signal if bigger than previous value,
     * after calculating it from the parameters.
     *
     * @param processedAmount the processed amount
     * @param totalAmount the total amount
     * @see percent()
     */
    void emitPercent(qulonglong processedAmount, qulonglong totalAmount);

    /**
     * Utility function for inherited jobs.
     * Emits the speed signal and starts the timer for removing that info
     *
     * @param speed the speed in bytes/s
     */
    void emitSpeed(unsigned long speed);

protected:
    VJobPrivate *const d_ptr;
    VJob(VJobPrivate &dd, QObject *parent);

private:
    Q_PRIVATE_SLOT(d_func(), void _q_speedTimeout())
    Q_DECLARE_PRIVATE(VJob)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(VJob::Capabilities)

#endif // VJOB_H
