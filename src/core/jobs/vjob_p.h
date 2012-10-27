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

#ifndef VJOB_P_H
#define VJOB_P_H

#include <QMap>

#include "vjob.h"

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Vibe API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

class VJobUiDelegate;
class QTimer;
class QEventLoop;

class VIBECORE_EXPORT VJobPrivate
{
public:
    VJobPrivate();
    virtual ~VJobPrivate();

    VJob *q_ptr;

    VJobUiDelegate *uiDelegate;
    int error;
    QString errorText;
    VJob::Unit progressUnit;
    QMap<VJob::Unit, qulonglong> processedAmount;
    QMap<VJob::Unit, qulonglong> totalAmount;
    unsigned long percentage;
    bool suspended;
    VJob::Capabilities capabilities;
    QTimer *speedTimer;
    bool isAutoDelete;
    QEventLoop *eventLoop;

    void _q_speedTimeout();

    static bool _q_kjobUnitEnumRegistered;

    bool isFinished;

    Q_DECLARE_PUBLIC(VJob)
};

#endif // VJOB_P_H
