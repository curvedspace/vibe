/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * Vibe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Vibe.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef VMAINAPPLICATIONADAPTOR_P_H
#define VMAINAPPLICATIONADAPTOR_P_H

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

class VMainApplicationAdaptorPrivate
{
    Q_DECLARE_PUBLIC(VMainApplicationAdaptor)
public:
    explicit VMainApplicationAdaptorPrivate(VMainApplicationAdaptor *self,
                                            const QString &identifier,
                                            QGuiApplication *app);

    VMainApplicationAdaptor *const q_ptr;

    QGuiApplication *app;
    QString id;
    quint32 internalId;

public slots:
    void _q_lastWindowClosed();
    void _q_aboutToQuit();
};

#endif // VMAINAPPLICATIONADAPTOR_P_H
