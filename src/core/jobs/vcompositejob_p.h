/*  This file is part of the KDE project
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

#ifndef VCOMPOSITEJOB_P_H
#define VCOMPOSITEJOB_P_H

#include "vcompositejob.h"
#include "vjob_p.h"

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

class VIBE_EXPORT VCompositeJobPrivate: public VJobPrivate
{
public:
    VCompositeJobPrivate();
    ~VCompositeJobPrivate();

    QList<VJob *> subjobs;

    Q_DECLARE_PUBLIC(VCompositeJob)
};

#endif // VCOMPOSITEJOB_P_H
