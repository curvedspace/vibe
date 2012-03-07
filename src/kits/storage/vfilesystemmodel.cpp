/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
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

#include <QtWidgets/QApplication>
#include <QtWidgets/QStyle>

#include "vfilesystemmodel.h"

VFileSystemModel::VFileSystemModel(QObject *parent) :
    QFileSystemModel(parent)
{
}

QVariant VFileSystemModel::data(const QModelIndex &index, int role) const
{
    QFileInfo fileInfo(filePath(index));

    if (role == Qt::DisplayRole) {
        if (index.column() == 2 && !fileInfo.isDir()) {
            QMimeType type = m_mimeDatabase.mimeTypeForFile(fileInfo);
            return type.comment();
        } else
            return QFileSystemModel::data(index, role);
    } else if (role == Qt::DecorationRole) {
        if (index.column() != 0)
            return QVariant();

        if (fileInfo.isDir()) {
            return QFileSystemModel::data(index, role);
        } else {
            // Load an icon by looking at the MIME database, if the icon is not available
            // try with a generic name
            QMimeType type = m_mimeDatabase.mimeTypeForFile(fileInfo);
            QString iconName = type.iconName();
            if (!QIcon::hasThemeIcon(iconName)) {
                if (fileInfo.isExecutable())
                    iconName = QStringLiteral("application-x-executable");
                else {
                    QString mimeType = type.name();
                    if (QIcon::hasThemeIcon(mimeType.split("-").at(0) + "-x-generic"))
                        iconName = mimeType.split("-").at(0) + "-x-generic";
                    else
                        iconName = QString();
                }
            }

            // Return the icon we found above, if nothing was found return
            // the standard icon
            if (iconName.isEmpty())
                return QIcon(qApp->style()->standardIcon(QStyle::SP_FileIcon));
            return QIcon::fromTheme(type.iconName());
        }
    }

    return QFileSystemModel::data(index, role);
}

#include "moc_vfilesystemmodel.cpp"
