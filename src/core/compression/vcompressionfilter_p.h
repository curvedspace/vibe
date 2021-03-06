/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2000-2006 David Faure
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    David Faure <faure@kde.org>
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

#ifndef VCOMPRESSIONFILTER_P_H
#define VCOMPRESSIONFILTER_P_H

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

class VCompressionFilterPrivate
{
public:
    VCompressionFilterPrivate();

    bool bNeedHeader;
    bool bSkipHeaders;
    bool autoDeleteFilterBase;
    bool bOpenedUnderlyingDevice;
    bool bIgnoreData;
    QByteArray buffer; // Used as 'input buffer' when reading, as 'output buffer' when writing
    QByteArray origFileName;
    VAbstractCompressionFilter::Result result;
    VAbstractCompressionFilter *filter;
};

#endif // VCOMPRESSIONFILTER_P_H
