/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2010-2011 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef VCOLORSCHEME_H
#define VCOLORSCHEME_H

#include <QString>
#include <QPalette>

#include <VibeCore/VGlobal>

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
