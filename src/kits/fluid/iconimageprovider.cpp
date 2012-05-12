/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
 *
 * Author(s):
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

#include <QDebug>
#include <QIcon>

#include "iconimageprovider.h"

IconImageProvider::IconImageProvider() :
    QQuickImageProvider(QQuickImageProvider::Image)
{
}

QImage IconImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    // Special case for images whose full path was specified,
    // we just need to resize to the requested size
    if (id.startsWith("/") && id.length() > 1) {
        QImage icon(id);
        if (icon.isNull()) {
            qWarning() << "Failed to directly load icon" << id;
            return QImage();
        }

        if (requestedSize.isValid())
            icon = icon.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        if (size)
            *size = icon.size();
        return icon;
    }

    // Perform icon lookup in the default theme
    QIcon icon(QIcon::fromTheme(id, QIcon::fromTheme("unknown", QIcon(":/images/unknown.svg"))));
    if (size)
        *size = icon.actualSize(requestedSize);
    return QImage(icon.pixmap(requestedSize).toImage());
}
