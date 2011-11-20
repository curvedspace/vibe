/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** Copyright (C) 2009 Dzimi Mve Alexandre <dzimiwine@gmail.com>
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QSTYLEOPTIONBREADCRUMBBAR_H
#define QSTYLEOPTIONBREADCRUMBBAR_H

#include <QtGui/qstyleoption.h>
#include <QtGui/qstyle.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Gui)

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

class Q_GUI_EXPORT QStyleOptionBreadCrumbIndicator : public QStyleOption
{
public:
    enum StyleOptionType { Type = SO_CustomBase + 1 };
    enum StyleOptionVersion { Version = 1 };

    QStyleOptionBreadCrumbIndicator();
    QStyleOptionBreadCrumbIndicator(const QStyleOptionBreadCrumbIndicator &other) : QStyleOption(Version, Type) {
        *this = other;
    }

    bool isTruncated;
    bool hasLabel;
    bool usePseudoState;
    bool isValid;
    bool isFlat;
};

class Q_GUI_EXPORT QStyleOptionBreadCrumbLabel : public QStyleOption
{
public:
    enum StyleOptionType { Type = SO_CustomBase + 2 };
    enum StyleOptionVersion { Version = 1 };

    QStyleOptionBreadCrumbLabel();
    QStyleOptionBreadCrumbLabel(const QStyleOptionBreadCrumbLabel &other) : QStyleOption(Version, Type) {
        *this = other;
    }

    QString text;
    bool hasIndicator;
    bool usePseudoState;
    bool isValid;
    bool isFlat;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // QSTYLEOPTIONBREADCRUMBBAR_H
