/****************************************************************************
 * This file is part of Qube.
 *
 * Copyright (c) 2007 Kevin Ottens
 * Copyright (c) 2007 David Faure
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	David Faure <faure@kde.org>
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *	Kevin Ottens <ervin@kde.org>
 *
 * Qube is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Qube is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Qube.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef QUBE_FILEPLACESMODEL_H
#define QUBE_FILEPLACESMODEL_H

#include <QtCore/QAbstractItemModel>

#include <Qube/Hardware/Device>
#include <Qube/Hardware/qubehardwarenamespace.h>

class QMimeData;
class QAction;
class QIcon;
class QUrl;

namespace Qube
{
    namespace Storage
    {
        class Bookmark;

        /**
         * This class is a list view model. Each entry represents a "place"
         * where user can access files. Only revelant when
         * used with QListView or QTableView.
         */
        class FilePlacesModel : public QAbstractItemModel
        {
            Q_OBJECT
        public:
            enum AdditionalRoles {
                UrlRole = 0x069CD12B,
                HiddenRole = 0x0741CAAC,
                SetupNeededRole = 0x059A935D,
                FixedDeviceRole = 0x332896C1,
                CapacityBarRecommendedRole = 0x1548C5C4
            };

            explicit FilePlacesModel(QObject *parent = 0);
            ~FilePlacesModel();

            QUrl url(const QModelIndex &index) const;
            bool setupNeeded(const QModelIndex &index) const;
            QIcon icon(const QModelIndex &index) const;
            QString text(const QModelIndex &index) const;
            bool isHidden(const QModelIndex &index) const;
            bool isDevice(const QModelIndex &index) const;
            Qube::Hardware::Device deviceForIndex(const QModelIndex &index) const;
            Bookmark bookmarkForIndex(const QModelIndex &index) const;

            QAction *teardownActionForIndex(const QModelIndex &index) const;
            QAction *ejectActionForIndex(const QModelIndex &index) const;
            void requestTeardown(const QModelIndex &index);
            void requestEject(const QModelIndex &index);
            void requestSetup(const QModelIndex &index);

            void addPlace(const QString &text, const QUrl &url, const QString &iconName = QString(), const QString &appName = QString());
            void addPlace(const QString &text, const QUrl &url, const QString &iconName, const QString &appName, const QModelIndex &after);
            void editPlace(const QModelIndex &index, const QString &text, const QUrl &url, const QString &iconName = QString(), const QString &appName = QString());
            void removePlace(const QModelIndex &index) const;
            void setPlaceHidden(const QModelIndex &index, bool hidden);

            int hiddenCount() const;

            /**
             * @brief Get a visible data based on Qt role for the given index.
             * Return the device information for the give index.
             *
             * @param index The QModelIndex which contains the row, column to fetch the data.
             * @param role The Interview data role(ex: Qt::DisplayRole).
             *
             * @return the data for the given index and role.
             */
            QVariant data(const QModelIndex &index, int role) const;

            /**
             * @brief Get the children model index for the given row and column.
             */
            QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

            /**
             * @brief Get the parent QModelIndex for the given model child.
             */
            QModelIndex parent(const QModelIndex &child) const;

            /**
             * @brief Get the number of rows for a model index.
             */
            int rowCount(const QModelIndex &parent = QModelIndex()) const;

            /**
             * @brief Get the number of columns for a model index.
             */
            int columnCount(const QModelIndex &parent = QModelIndex()) const;

            /**
             * Returns the closest item for the URL \a url.
             * The closest item is defined as item which is equal to
             * the URL or at least is a parent URL. If there are more than
             * one possible parent URL candidates, the item which covers
             * the bigger range of the URL is returned.
             *
             * Example: the url is '/home/peter/Documents/Music'.
             * Available items are:
             * - /home/peter
             * - /home/peter/Documents
             *
             * The returned item will the one for '/home/peter/Documents'.
             */
            QModelIndex closestItem(const QUrl &url) const;


            Qt::DropActions supportedDropActions() const;
            Qt::ItemFlags flags(const QModelIndex &index) const;
            QStringList mimeTypes() const;
            QMimeData *mimeData(const QModelIndexList &indexes) const;
            bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                              int row, int column, const QModelIndex &parent);

        Q_SIGNALS:
            void errorMessage(const QString &message);
            void setupDone(const QModelIndex &index, bool success);

        private:
            class Private;
            Private * const d;
            friend class Private;
        };
    }
}

#endif // QUBE_FILEPLACESMODEL_H
