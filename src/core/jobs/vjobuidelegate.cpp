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

#include "vjobuidelegate.h"
#include "vjob.h"

class VJobUiDelegate::Private
{
public:
    Private(VJobUiDelegate *delegate)
        : q(delegate), job(0),
          autoErrorHandling(false),
          autoWarningHandling(true) { }

    VJobUiDelegate *const q;

    VJob *job;
    bool autoErrorHandling : 1;
    bool autoWarningHandling : 1;

    void connectJob(VJob *job);
    void _q_result(VJob *job);
};

VJobUiDelegate::VJobUiDelegate()
    : QObject(), d(new Private(this))
{

}

VJobUiDelegate::~VJobUiDelegate()
{
    delete d;
}

bool VJobUiDelegate::setJob(VJob *job)
{
    if (d->job != 0) {
        return false;
    }

    d->job = job;
    setParent(job);

    return true;
}

VJob *VJobUiDelegate::job()
{
    return d->job;
}

void VJobUiDelegate::showErrorMessage()
{
}

void VJobUiDelegate::setAutoErrorHandlingEnabled(bool enable)
{
    d->autoErrorHandling = enable;
}

bool VJobUiDelegate::isAutoErrorHandlingEnabled() const
{
    return d->autoErrorHandling;
}

void VJobUiDelegate::setAutoWarningHandlingEnabled(bool enable)
{
    d->autoWarningHandling = enable;
}

bool VJobUiDelegate::isAutoWarningHandlingEnabled() const
{
    return d->autoWarningHandling;
}

void VJobUiDelegate::slotWarning(VJob *job, const QString &plain,
                                 const QString &rich)
{
    Q_UNUSED(job)
    Q_UNUSED(plain)
    Q_UNUSED(rich)
}

void VJobUiDelegate::connectJob(VJob *job)
{
    connect(job, SIGNAL(result(VJob *)),
            this, SLOT(_q_result(VJob *)));

    connect(job, SIGNAL(warning(VJob *, QString, QString)),
            this, SLOT(slotWarning(VJob *, QString, QString)));
}

void VJobUiDelegate::Private::_q_result(VJob *job2)
{
    Q_UNUSED(job2)
    if (job->error() && autoErrorHandling)
        q->showErrorMessage();
}

#include "moc_vjobuidelegate.cpp"
