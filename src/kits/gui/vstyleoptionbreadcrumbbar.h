/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (C) 2009 Dzimi Mve Alexandre
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *    Dzimi Mve Alexandre <dzimiwine@gmail.com>
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

#ifndef VSTYLEOPTIONBREADCRUMBBAR_H
#define VSTYLEOPTIONBREADCRUMBBAR_H

#include <QtGui/qstyleoption.h>
#include <QtGui/qstyle.h>

#include <VibeCore/VGlobal>

class VIBE_EXPORT VStyleOptionBreadCrumbIndicator : public QStyleOption
{
public:
    enum StyleOptionType { Type = SO_CustomBase + 1 };
    enum StyleOptionVersion { Version = 1 };

    VStyleOptionBreadCrumbIndicator();
    VStyleOptionBreadCrumbIndicator(const VStyleOptionBreadCrumbIndicator &other) : QStyleOption(Version, Type) {
        *this = other;
    }

    bool isTruncated;
    bool hasLabel;
    bool usePseudoState;
    bool isValid;
    bool isFlat;
};

class VIBE_EXPORT VStyleOptionBreadCrumbLabel : public QStyleOption
{
public:
    enum StyleOptionType { Type = SO_CustomBase + 2 };
    enum StyleOptionVersion { Version = 1 };

    VStyleOptionBreadCrumbLabel();
    VStyleOptionBreadCrumbLabel(const VStyleOptionBreadCrumbLabel &other) : QStyleOption(Version, Type) {
        *this = other;
    }

    QString text;
    bool hasIndicator;
    bool usePseudoState;
    bool isValid;
    bool isFlat;
};

#endif // VSTYLEOPTIONBREADCRUMBBAR_H
