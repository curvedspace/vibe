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

#ifndef VFILESYSTEMMODEL_H
#define VFILESYSTEMMODEL_H

#include <QFileSystemModel>

#include <VibeCore/VGlobal>

class VIBE_EXPORT VFileSystemModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit VFileSystemModel(QObject *parent = 0);
    ~VFileSystemModel();

    QIcon fileIcon(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    QHash<QString, QIcon> *m_mimeIcons;
    QHash<QString, QString> *m_mimeGlob;
    QHash<QString, QString> *m_mimeGeneric;
    QHash<QString, QString> *m_mimeDescr;

    void loadMimeTypes() const;
};

#endif // VFILESYSTEMMODEL_H
