/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2010-2011 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef VCOLORSCHEME_P_H
#define VCOLORSCHEME_P_H

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

class VColorSchemePrivate
{
    Q_DECLARE_PUBLIC(VColorScheme)
public:
    typedef QPair<QPalette::ColorGroup, QString> Entry;
    typedef QPair<Entry, QColor> Color;

    QSettings *colorScheme;
    QList<Color> colors;
    QPalette *palette;

    VColorSchemePrivate(VColorScheme *q, const QString &name);

    QColor value(QPalette::ColorGroup group, const QString &name) const;

protected:
    VColorScheme *const q_ptr;
};

#endif // VCOLORSCHEME_P_H
