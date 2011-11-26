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

#ifndef VCOLORSCHEME_H
#define VCOLORSCHEME_H

#include <QtGui/QPalette>

#include <VibeCore/VGlobal>

class VColorSchemePrivate;

class VIBE_EXPORT VColorScheme
{
    Q_DECLARE_PRIVATE(VColorScheme)
public:
    VColorScheme(const QString &name);

    QString name() const;
    QString comment() const;

    QPalette palette() const;

    QColor background(QPalette::ColorGroup group) const;
    QColor foreground(QPalette::ColorGroup group) const;
    QColor base(QPalette::ColorGroup group) const;
    QColor text(QPalette::ColorGroup group) const;
    QColor button(QPalette::ColorGroup group) const;
    QColor buttonText(QPalette::ColorGroup group) const;
    QColor link(QPalette::ColorGroup group) const;
    QColor linkVisited(QPalette::ColorGroup group) const;
    QColor highlight(QPalette::ColorGroup group) const;
    QColor highlightedText(QPalette::ColorGroup group) const;

private:
    VColorSchemePrivate *const d_ptr;
};

#endif // VCOLORSCHEME_H
