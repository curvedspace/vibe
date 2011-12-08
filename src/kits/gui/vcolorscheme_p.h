/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2010-2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

class VIBE_EXPORT VColorSchemePrivate
{
    Q_DECLARE_PUBLIC(VColorScheme)
public:
    typedef QPair<QPalette::ColorGroup, QString> Entry;
    typedef QPair<Entry, QColor> Color;

    QSettings *colorScheme;
    QList<Color> colors;
    QPalette palette;
    VColorScheme *const q_ptr;

    VColorSchemePrivate(VColorScheme *q, const QString &name);

    QColor value(QPalette::ColorGroup group, const QString &name) const;
};

#endif // VCOLORSCHEME_P_H
