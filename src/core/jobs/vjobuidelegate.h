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

#ifndef VJOBUIDELEGATE_H
#define VJOBUIDELEGATE_H

#include <QObject>

#include <VibeCore/VibeCoreExport>

class VJob;

/**
 * The base class for all VJob UI delegate.
 *
 * A UI delegate is responsible for the events of a
 * job and provides a UI for them (an error message
 * box or warning etc.).
 *
 * @see VJob
 */
class VIBECORE_EXPORT VJobUiDelegate : public QObject
{
    Q_OBJECT
public:
    /**
     * Constructs a new VJobUiDelegate.
     */
    VJobUiDelegate();

    /**
     * Destroys a VJobUiDelegate.
     */
    virtual ~VJobUiDelegate();

private:
    /**
     * Attach this UI delegate to a job. Once attached it'll track the job events.
     *
     * @return true if the job we're correctly attached to the job, false otherwise.
     */
    bool setJob(VJob *job);

protected:
    /**
     * Retrieves the current job this UI delegate is attached to.
     *
     * @return current job this UI delegate is attached to, or 0 if
     * this UI delegate is not tracking any job
     */
    VJob *job();

    friend class VJob;

public:
    /**
     * Display a dialog box to inform the user of the error given by
     * this job.
     * Only call if error is not 0, and only in the slot connected
     * to result.
     */
    virtual void showErrorMessage();

    /**
     * Enable or disable the automatic error handling. When automatic
     * error handling is enabled and an error occurs, then showErrorDialog()
     * is called, right before the emission of the result signal.
     *
     * The default is false.
     *
     * See also isAutoErrorHandlingEnabled , showErrorDialog
     *
     * @param enable enable or disable automatic error handling
     * @see isAutoErrorHandlingEnabled()
     */
    void setAutoErrorHandlingEnabled(bool enable);

    /**
     * Returns whether automatic error handling is enabled or disabled.
     * See also setAutoErrorHandlingEnabled .
     * @return true if automatic error handling is enabled
     * @see setAutoErrorHandlingEnabled()
     */
    bool isAutoErrorHandlingEnabled() const;

    /**
     * Enable or disable the automatic warning handling. When automatic
     * warning handling is enabled and an error occurs, then a message box
     * is displayed with the warning message
     *
     * The default is true.
     *
     * See also isAutoWarningHandlingEnabled , showErrorDialog
     *
     * @param enable enable or disable automatic warning handling
     * @see isAutoWarningHandlingEnabled()
     */
    void setAutoWarningHandlingEnabled(bool enable);

    /**
     * Returns whether automatic warning handling is enabled or disabled.
     * See also setAutoWarningHandlingEnabled .
     * @return true if automatic warning handling is enabled
     * @see setAutoWarningHandlingEnabled()
     */
    bool isAutoWarningHandlingEnabled() const;

protected Q_SLOTS:
    virtual void slotWarning(VJob *job, const QString &plain, const QString &rich);

private:
    void connectJob(VJob *job);

    Q_PRIVATE_SLOT(d, void _q_result(VJob *))

    class Private;
    Private *const d;
};

#endif // VJOBUIDELEGATE_H
