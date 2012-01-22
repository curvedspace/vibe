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

#include <QtCore/QTextStream>
#include <QtGui/QApplication>
#include <QtGui/QStyle>

#include <VibeCore/VMimeType>

#include "vfilesystemmodel.h"

VFileSystemModel::VFileSystemModel(QObject *parent) :
    QFileSystemModel(parent)
{
    m_mimeIcons = new QHash<QString, QIcon>;
    m_mimeGlob = new QHash<QString, QString>;
    m_mimeGeneric = new QHash<QString, QString>;
}

VFileSystemModel::~VFileSystemModel()
{
    delete m_mimeIcons;
    delete m_mimeGlob;
    delete m_mimeGeneric;
}

QIcon VFileSystemModel::fileIcon(const QModelIndex &index) const
{
    return QFileSystemModel::fileIcon(index);
}

QVariant VFileSystemModel::data(const QModelIndex &index, int role) const
{
    QFileInfo type(filePath(index));

    if (role == Qt::DisplayRole) {
        if (index.column() == 2 && !type.isDir()) {
            QString suffix = type.suffix();

            if (m_mimeDescr->contains(suffix))
                return m_mimeDescr->value(suffix);

            VMimeType mimeType;
            mimeType.fromFileName(filePath(index));
            m_mimeDescr->insert(suffix, mimeType.mimeType());
            return mimeType.mimeType();
        } else
            return QFileSystemModel::data(index, role);
    } else if (role == Qt::DecorationRole) {
        if (index.column() != 0)
            return QVariant();

        if (type.isDir()) {
            return QFileSystemModel::data(index, role);
        } else {
            QString suffix = type.suffix();
            if (m_mimeIcons->contains(suffix))
                return m_mimeIcons->value(suffix);

            QIcon icon;

            if (suffix.isEmpty()) {
                // If suffix is empty, just use either exec for executable files or none
                if (type.isExecutable())
                    suffix = "exec";
                else
                    suffix = "none";

                // Return the icon if already cached
                if (m_mimeIcons->contains(suffix))
                    return m_mimeIcons->value(suffix);

                // Otherwise take it from theme
                if (suffix == "exec")
                    icon = QIcon::fromTheme("application-x-executable");
                else
                    icon = QIcon(qApp->style()->standardIcon(QStyle::SP_FileIcon));
            } else {
                // Load mime types if needed
                if (m_mimeGlob->count() == 0)
                    loadMimeTypes();

                QString mimeTypeStr = m_mimeGlob->value(type.suffix().toLower());
                if (QIcon::hasThemeIcon(mimeTypeStr))
                    icon = QIcon::fromTheme(mimeTypeStr);
                else {
                    // Try matching the generic icon
                    if (QIcon::hasThemeIcon(m_mimeGeneric->value(mimeTypeStr)))
                        icon = QIcon::fromTheme(m_mimeGeneric->value(mimeTypeStr));
                    else {
                        // Last chance, try adding "-x-generic" to the base type
                        if (QIcon::hasThemeIcon(mimeTypeStr.split("-").at(0) + "-x-generic"))
                            icon = QIcon::fromTheme(mimeTypeStr.split("-").at(0) + "-x-generic");
                        else
                            icon = QIcon(qApp->style()->standardIcon(QStyle::SP_FileIcon));
                    }
                }
            }

            m_mimeIcons->insert(suffix, icon);
            return icon;
        }
    }

    return QFileSystemModel::data(index, role);
}

void VFileSystemModel::loadMimeTypes() const
{
    QFile mimeInfo("/usr/share/mime/globs");
    mimeInfo.open(QIODevice::ReadOnly);
    QTextStream out(&mimeInfo);

    do {
        QStringList line = out.readLine().split(":");
        if (line.count() == 2) {
            QString suffix = line.at(1);
            suffix.remove("*.");
            QString mimeName = line.at(0);
            mimeName.replace("/", "-");
            m_mimeGlob->insert(suffix, mimeName);
        }
    } while (!out.atEnd());

    mimeInfo.close();

    mimeInfo.setFileName("/usr/share/mime/generic-icons");
    mimeInfo.open(QIODevice::ReadOnly);
    out.setDevice(&mimeInfo);

    do {
        QStringList line = out.readLine().split(":");
        if (line.count() == 2) {
            QString mimeName = line.at(0);
            mimeName.replace("/", "-");
            QString icon = line.at(1);
            m_mimeGeneric->insert(mimeName, icon);
        }
    } while (!out.atEnd());

    mimeInfo.close();
}

#include "vfilesystemmodel.moc"
