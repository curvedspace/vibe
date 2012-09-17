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

class VIBE_EXPORT VJobPrivate
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
