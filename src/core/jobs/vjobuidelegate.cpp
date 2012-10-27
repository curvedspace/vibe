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
