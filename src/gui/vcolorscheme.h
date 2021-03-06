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

#ifndef VCOLORSCHEME_H
#define VCOLORSCHEME_H

#include <QString>
#include <QPalette>

#include <VibeGui/VibeGuiExport>

class VColorSchemePrivate;

class VIBEGUI_EXPORT VColorScheme
{
    Q_DECLARE_PRIVATE(VColorScheme)
public:
    VColorScheme(const QString &name);

    QString name() const;
    QString comment() const;

    const QPalette *palette() const;

    QColor window(QPalette::ColorGroup group) const;
    QColor windowText(QPalette::ColorGroup group) const;
    QColor base(QPalette::ColorGroup group) const;
    QColor alternateBase(QPalette::ColorGroup group) const;
    QColor toolTipBase(QPalette::ColorGroup group) const;
    QColor toolTipText(QPalette::ColorGroup group) const;
    QColor text(QPalette::ColorGroup group) const;
    QColor button(QPalette::ColorGroup group) const;
    QColor buttonText(QPalette::ColorGroup group) const;
    QColor brightText(QPalette::ColorGroup group) const;
    QColor light(QPalette::ColorGroup group) const;
    QColor midlight(QPalette::ColorGroup group) const;
    QColor dark(QPalette::ColorGroup group) const;
    QColor mid(QPalette::ColorGroup group) const;
    QColor shadow(QPalette::ColorGroup group) const;
    QColor highlight(QPalette::ColorGroup group) const;
    QColor highlightedText(QPalette::ColorGroup group) const;
    QColor link(QPalette::ColorGroup group) const;
    QColor linkVisited(QPalette::ColorGroup group) const;

private:
    VColorSchemePrivate *const d_ptr;
};

#endif // VCOLORSCHEME_H
