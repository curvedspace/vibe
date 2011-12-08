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

#ifndef VSTYLE_H
#define VSTYLE_H

#include <QStyle>

#include <VibeCore/VGlobal>

/*
#define SC_BreadCrumbContainer (uint)(QStyle::SC_CustomBase + 2)
#define SC_BreadCrumbEditField (uint)(QStyle::SC_CustomBase + 3)
#define SC_BreadCrumbIcon (uint)(QStyle::SC_CustomBase + 4)

#define SE_BreadCrumbIndicator (uint)(QStyle::SE_CustomBase + 2)
#define SE_BreadCrumbLabel (uint)(QStyle::SE_CustomBase + 3)
#define SE_BreadCrumbEmptyArea (uint)(QStyle::SE_CustomBase + 4)

#define PE_FrameBreadCrumbBar (uint)(QStyle::PE_CustomBase + 2)
#define PE_BreadCrumbContainerBase (uint)(QStyle::PE_CustomBase + 3)

#define CE_BreadCrumbIndicator (uint)(QStyle::CE_CustomBase + 2)
#define CE_BreadCrumbLabel (uint)(QStyle::CE_CustomBase + 3)
#define CE_BreadCrumbEmptyArea (uint)(QStyle::CE_CustomBase + 4)
*/

class VIBE_EXPORT VStyle : public QStyle
{
public:
    enum AdditionalSubControl {
        SC_BreadCrumbContainer = SC_CustomBase + 1,
        SC_BreadCrumbEditField = SC_CustomBase + 2,
        SC_BreadCrumbIcon = SC_CustomBase + 3
    };
    Q_DECLARE_FLAGS(AdditionalSubControls, AdditionalSubControl)

    enum AdditionalSubElement {
        SE_BreadCrumbIndicator = SE_CustomBase + 1,
        SE_BreadCrumbLabel = SE_CustomBase + 2,
        SE_BreadCrumbEmptyArea = SE_CustomBase + 3
    };

    enum AdditionalPrimitiveElement {
        PE_FrameBreadCrumbBar = PE_CustomBase + 1,
        PE_BreadCrumbContainerBase = PE_CustomBase + 2
    };

    enum AdditionalControlElement {
        CE_BreadCrumbIndicator = CE_CustomBase + 1,
        CE_BreadCrumbLabel = CE_CustomBase + 2,
        CE_BreadCrumbEmptyArea = CE_CustomBase + 3
    };
};

#endif // VSTYLE_H
