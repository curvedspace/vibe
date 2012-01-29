/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
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

#ifndef VDECLARATIVEICONIMAGEPROVIDER_H
#define VDECLARATIVEICONIMAGEPROVIDER_H

#include <QDeclarativeImageProvider>

#include <VibeCore/VGlobal>

class VIBE_EXPORT VDeclarativeIconImageProvider : public QDeclarativeImageProvider
{
public:
    explicit VDeclarativeIconImageProvider();

    virtual QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
};

#endif // VDECLARATIVEICONIMAGEPROVIDER_H
