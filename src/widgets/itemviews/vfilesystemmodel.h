/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
 *
 * Author(s):
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

#ifndef VFILESYSTEMMODEL_H
#define VFILESYSTEMMODEL_H

#include <QMimeDatabase>
#include <QFileSystemModel>

#include <VibeWidgets/VibeWidgetsExport>

class VIBEWIDGETS_EXPORT VFileSystemModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit VFileSystemModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;

private:
    QMimeDatabase m_mimeDatabase;
};

#endif // VFILESYSTEMMODEL_H
