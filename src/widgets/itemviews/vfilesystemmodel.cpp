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

#include <QStyle>
#include <QStandardPaths>
#include <QFileIconProvider>

#include <VibeCore/VDesktopFile>

#include "vfilesystemmodel.h"

class VFileIconProvider : public QFileIconProvider
{
public:
    VFileIconProvider();

    QIcon icon(const QFileInfo &fileInfo) const;
    QString type(const QFileInfo &fileInfo) const;

private:
    QMimeDatabase m_mimeDatabase;
};

VFileIconProvider::VFileIconProvider()
    : QFileIconProvider()
{
}

QIcon VFileIconProvider::icon(const QFileInfo &fileInfo) const
{
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
        if (!QIcon::hasThemeIcon(iconName)) {
            if (fileInfo.isExecutable())
                iconName = QStringLiteral("application-x-executable");
            else if (QIcon::hasThemeIcon(type.genericIconName()))
                iconName = type.genericIconName();
        }

        // Fallback to the default icon
        if (iconName.isEmpty())
            iconName = QStringLiteral("text-x-preview");

        // Return the icon we found above
        return QIcon::fromTheme(iconName);
    }

    return QFileIconProvider::icon(fileInfo);
}

QString VFileIconProvider::type(const QFileInfo &fileInfo) const
{
    // MIME type description for files
    if (!fileInfo.isDir()) {
        QMimeType type = m_mimeDatabase.mimeTypeForFile(fileInfo);
        return type.comment();
    }

    return QFileIconProvider::type(fileInfo);
}

VFileSystemModel::VFileSystemModel(QObject *parent)
    : QFileSystemModel(parent)
{
    setIconProvider(new VFileIconProvider());
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

    if (role == Qt::DisplayRole && index.column() == 0) {
        if (fileInfo.isDir()) {
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
        } else {
            QMimeType type = m_mimeDatabase.mimeTypeForFile(fileInfo);

            // If it's a desktop file try to load it
            if (type.name() == "application/x-desktop") {
                VDesktopFile desktopFile(fileInfo.absoluteFilePath());
                if (!desktopFile.name().isEmpty())
                    return desktopFile.name();
            }
        }
    }

    return QFileSystemModel::data(index, role);
}

#include "moc_vfilesystemmodel.cpp"
