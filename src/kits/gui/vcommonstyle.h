/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef VCOMMONSTYLE_H
#define VCOMMONSTYLE_H

#include <QCommonStyle>

#include <VibeGui/VStyle>

class VIBE_EXPORT VCommonStyle : public VStyle, public QCommonStyle
{
    Q_OBJECT
public:
    VCommonStyle();

    virtual QRect subControlRect(QStyle::ComplexControl control,
                                 const QStyleOptionComplex *option,
                                 QStyle::SubControl subControl, const QWidget *widget = 0) const;
    virtual QRect subElementRect(QStyle::SubElement subElement,
                                 const QStyleOption *option,
                                 const QWidget *widget = 0) const;
    virtual void drawPrimitive(QStyle::PrimitiveElement element,
                               const QStyleOption *option, QPainter *painter,
                               const QWidget *widget = 0) const;
    virtual void drawControl(QStyle::ControlElement element,
                             const QStyleOption *option, QPainter *painter,
                             const QWidget *widget = 0) const;

protected:
    QRect lineEditRect(const VBreadCrumbBar *bar) const;
};

#endif // VCOMMONSTYLE_H
