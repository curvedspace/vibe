/*  This file is part of the KDE project
    Copyright (C) 2000 Stephan Kulow <coolo@kde.org>
                       David Faure <faure@kde.org>
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

#include <QEventLoop>
#include <QMap>
#include <QMetaType>
#include <QTimer>

#include "vjob.h"
#include "vjob_p.h"
#include "vjobuidelegate.h"

bool VJobPrivate::_q_kjobUnitEnumRegistered = false;
VJobPrivate::VJobPrivate()
    : q_ptr(0), uiDelegate(0), error(VJob::NoError),
      progressUnit(VJob::Bytes), percentage(0),
      suspended(false), capabilities(VJob::NoCapabilities),
      speedTimer(0), isAutoDelete(true), eventLoop(0), isFinished(false)
{
    if (!_q_kjobUnitEnumRegistered)
        _q_kjobUnitEnumRegistered = qRegisterMetaType<VJob::Unit>("VJob::Unit");
}

VJobPrivate::~VJobPrivate()
{
}

VJob::VJob(QObject *parent)
    : QObject(parent), d_ptr(new VJobPrivate)
{
    d_ptr->q_ptr = this;
    // Don't exit while this job is running
#pragma message ("KDE5 TODO: Revert ref-counting when it is in Qt5")
    //KGlobal::ref();
}

VJob::VJob(VJobPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
    d_ptr->q_ptr = this;
    // Don't exit while this job is running
    //KGlobal::ref();
}

VJob::~VJob()
{
    if (!d_ptr->isFinished) {
        Q_EMIT finished(this);
    }

    delete d_ptr->speedTimer;
    delete d_ptr->uiDelegate;
    delete d_ptr;

    //KGlobal::deref();
}

void VJob::setUiDelegate(VJobUiDelegate *delegate)
{
    Q_D(VJob);
    if (delegate == 0 || delegate->setJob(this)) {
        delete d->uiDelegate;
        d->uiDelegate = delegate;

        if (d->uiDelegate) {
            d->uiDelegate->connectJob(this);
        }
    }
}

VJobUiDelegate *VJob::uiDelegate() const
{
    return d_func()->uiDelegate;
}

VJob::Capabilities VJob::capabilities() const
{
    return d_func()->capabilities;
}

bool VJob::isSuspended() const
{
    return d_func()->suspended;
}

bool VJob::kill(KillVerbosity verbosity)
{
    Q_D(VJob);
    if (doKill()) {
        setError(KilledJobError);

        if (verbosity != Quietly) {
            emitResult();
        } else {
            // If we are displaying a progress dialog, remove it first.
            d->isFinished = true;
            Q_EMIT finished(this);

            if (isAutoDelete())
                deleteLater();
        }

        return true;
    } else {
        return false;
    }
}

bool VJob::suspend()
{
    Q_D(VJob);
    if (!d->suspended) {
        if (doSuspend()) {
            d->suspended = true;
            Q_EMIT suspended(this);

            return true;
        }
    }

    return false;
}

bool VJob::resume()
{
    Q_D(VJob);
    if (d->suspended) {
        if (doResume()) {
            d->suspended = false;
            Q_EMIT resumed(this);

            return true;
        }
    }

    return false;
}

bool VJob::doKill()
{
    return false;
}

bool VJob::doSuspend()
{
    return false;
}

bool VJob::doResume()
{
    return false;
}

void VJob::setCapabilities(VJob::Capabilities capabilities)
{
    Q_D(VJob);
    d->capabilities = capabilities;
}

bool VJob::exec()
{
    Q_D(VJob);
    // Usually this job would delete itself, via deleteLater() just after
    // emitting result() (unless configured otherwise). Since we use an event
    // loop below, that event loop will process the deletion event and we'll
    // have been deleted when exec() returns. This crashes, so temporarily
    // suspend autodeletion and manually do it afterwards.
    const bool wasAutoDelete = isAutoDelete();
    setAutoDelete(false);

    Q_ASSERT(! d->eventLoop);

    QEventLoop loop(this);
    d->eventLoop = &loop;

    start();
    if (!d->isFinished) {
        d->eventLoop->exec(QEventLoop::ExcludeUserInputEvents);
    }
    d->eventLoop = 0;

    if (wasAutoDelete) {
        deleteLater();
    }
    return (d->error == NoError);
}

int VJob::error() const
{
    return d_func()->error;
}

QString VJob::errorText() const
{
    return d_func()->errorText;
}

QString VJob::errorString() const
{
    return d_func()->errorText;
}

qulonglong VJob::processedAmount(Unit unit) const
{
    return d_func()->processedAmount[unit];
}

qulonglong VJob::totalAmount(Unit unit) const
{
    return d_func()->totalAmount[unit];
}

unsigned long VJob::percent() const
{
    return d_func()->percentage;
}

void VJob::setError(int errorCode)
{
    Q_D(VJob);
    d->error = errorCode;
}

void VJob::setErrorText(const QString &errorText)
{
    Q_D(VJob);
    d->errorText = errorText;
}

void VJob::setProcessedAmount(Unit unit, qulonglong amount)
{
    Q_D(VJob);
    bool should_emit = (d->processedAmount[unit] != amount);

    d->processedAmount[unit] = amount;

    if (should_emit) {
        Q_EMIT processedAmount(this, unit, amount);
        if (unit == d->progressUnit) {
            Q_EMIT processedSize(this, amount);
            emitPercent(d->processedAmount[unit], d->totalAmount[unit]);
        }
    }
}

void VJob::setTotalAmount(Unit unit, qulonglong amount)
{
    Q_D(VJob);
    bool should_emit = (d->totalAmount[unit] != amount);

    d->totalAmount[unit] = amount;

    if (should_emit) {
        Q_EMIT totalAmount(this, unit, amount);
        if (unit == d->progressUnit) {
            Q_EMIT totalSize(this, amount);
            emitPercent(d->processedAmount[unit], d->totalAmount[unit]);
        }
    }
}

void VJob::setPercent(unsigned long percentage)
{
    Q_D(VJob);
    if (d->percentage != percentage) {
        d->percentage = percentage;
        Q_EMIT percent(this, percentage);
    }
}

void VJob::emitResult()
{
    Q_D(VJob);
    d->isFinished = true;

    if (d->eventLoop) {
        d->eventLoop->quit();
    }

    // If we are displaying a progress dialog, remove it first.
    Q_EMIT finished(this);

    Q_EMIT result(this);

    if (isAutoDelete())
        deleteLater();
}

void VJob::emitPercent(qulonglong processedAmount, qulonglong totalAmount)
{
    Q_D(VJob);
    // calculate percents
    if (totalAmount) {
        unsigned long oldPercentage = d->percentage;
        d->percentage = (unsigned long)(((float)(processedAmount) / (float)(totalAmount)) * 100.0);
        if (d->percentage != oldPercentage) {
            Q_EMIT percent(this, d->percentage);
        }
    }
}

void VJob::emitSpeed(unsigned long value)
{
    Q_D(VJob);
    if (!d->speedTimer) {
        d->speedTimer = new QTimer(this);
        connect(d->speedTimer, SIGNAL(timeout()), SLOT(_q_speedTimeout()));
    }

    Q_EMIT speed(this, value);
    d->speedTimer->start(5000);   // 5 seconds interval should be enough
}

void VJobPrivate::_q_speedTimeout()
{
    Q_Q(VJob);
    // send 0 and stop the timer
    // timer will be restarted only when we receive another speed event
    Q_EMIT q->speed(q, 0);
    speedTimer->stop();
}

bool VJob::isAutoDelete() const
{
    Q_D(const VJob);
    return d->isAutoDelete;
}

void VJob::setAutoDelete(bool autodelete)
{
    Q_D(VJob);
    d->isAutoDelete = autodelete;
}

#include "moc_vjob.cpp"
