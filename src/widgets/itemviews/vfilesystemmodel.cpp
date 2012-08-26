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

#include <QApplication>
#include <QStyle>
#include <QStandardPaths>

#include <VibeCore/VDesktopFile>

#include "vfilesystemmodel.h"

VFileSystemModel::VFileSystemModel(QObject *parent)
    : QFileSystemModel(parent)
{
}

QVariant VFileSystemModel::data(const QModelIndex &index, int role) const
{
    QFileInfo fileInfo(filePath(index));

    static QMap<QStandardPaths::StandardLocation, QString> iconsMap;
    iconsMap[QStandardPaths::DesktopLocation] = "user-desktop";
    iconsMap[QStandardPaths::DocumentsLocation] = "folder-documents";
    iconsMap[QStandardPaths::FontsLocation] = "folder-fonts";
    iconsMap[QStandardPaths::ApplicationsLocation] = "folder-applications";
    iconsMap[QStandardPaths::MusicLocation] = "folder-music";
    iconsMap[QStandardPaths::MoviesLocation] = "folder-video";
    iconsMap[QStandardPaths::PicturesLocation] = "folder-pictures";
    iconsMap[QStandardPaths::HomeLocation] = "user-home";
    iconsMap[QStandardPaths::DownloadLocation] = "folder-download";

    switch (role) {
        case Qt::EditRole:
        case Qt::DisplayRole:
            switch (index.column()) {
                case 0: {
                    // Read .directory desktop file
                    QString desktopFileName = fileInfo.dir().absoluteFilePath(".directory");
                    if (fileInfo.isDir() && QFile::exists(desktopFileName)) {
                        VDesktopFile desktopFile(desktopFileName);
                        if (desktopFile.isValid())
                            return desktopFile.name();
                    }

                    // Standard paths names
                    foreach(QStandardPaths::StandardLocation key, iconsMap.keys()) {
                        if (fileInfo.absoluteFilePath() == QStandardPaths::writableLocation(key))
                            return QStandardPaths::displayName(key);
                    }
                }
                break;
                case 2:
                    // MIME type description for files
                    if (!fileInfo.isDir()) {
                        QMimeType type = m_mimeDatabase.mimeTypeForFile(fileInfo);
                        return type.comment();
                    }
                    break;
                default:
                    break;
            }
            break;
        case Qt::DecorationRole:
            if (index.column() == 0) {
                if (fileInfo.isDir()) {
                    // Read .directory desktop file
                    QString desktopFileName = fileInfo.dir().absoluteFilePath(".directory");
                    if (fileInfo.isDir() && QFile::exists(desktopFileName)) {
                        VDesktopFile desktopFile(desktopFileName);
                        if (QIcon::hasThemeIcon(desktopFile.iconName()))
                            return QIcon::fromTheme(desktopFile.iconName());
                    }

                    // Standard paths icons
                    foreach(QStandardPaths::StandardLocation key, iconsMap.keys()) {
                        if (fileInfo.absoluteFilePath() == QStandardPaths::writableLocation(key)) {
                            if (QIcon::hasThemeIcon(iconsMap[key]))
                                return QIcon::fromTheme(iconsMap[key]);
                        }
                    }
                } else {
                    // Icon from the MIME database
                    QMimeType type = m_mimeDatabase.mimeTypeForFile(fileInfo);
                    QString iconName = type.iconName();

                    // If it's a desktop file try to load its icon
                    if (type.name() == "application/x-desktop") {
                        VDesktopFile desktopFile(fileInfo.absoluteFilePath());
                        iconName = desktopFile.iconName();

                        // Load an image file if the desktop file uses an absolute path
                        if (QFile::exists(iconName))
                            return QIcon(iconName);
                    }

                    // If the icon is not available try with a generic name
                    if (iconName.isEmpty() || !QIcon::hasThemeIcon(iconName)) {
                        if (fileInfo.isExecutable())
                            iconName = QStringLiteral("application-x-executable");
                        else {
                            if (QIcon::hasThemeIcon(type.genericIconName()))
                                iconName = type.genericIconName();
                            else
                                iconName = QString();
                        }
                    }

                    // Return the icon we found above
                    if (!iconName.isEmpty())
                        return QIcon::fromTheme(iconName);
                }
            }
            break;
        default:
            break;
    }

    return QFileSystemModel::data(index, role);
}

#include "moc_vfilesystemmodel.cpp"
