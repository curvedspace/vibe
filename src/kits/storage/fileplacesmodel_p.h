
#ifndef FILEPLACESMODEL_P_H
#define FILEPLACESMODEL_P_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QSet>

#include <QubeHardware/Predicate>

#include "bookmarkmanager.h"

namespace QubeStorage
{
    class FilePlacesModel;

    class FilePlacesModelPrivate
    {
    public:
        FilePlacesModelPrivate(FilePlacesModel *self) :
            q(self),
            bookmarkManager(0),
            sharedBookmarks(0) {
        }

        ~FilePlacesModelPrivate() {
            delete sharedBookmarks;
            qDeleteAll(items);
        }

        FilePlacesModel *q;

        QList<FilePlacesItem*> items;
        QSet<QString> availableDevices;
        QMap<QObject*, QPersistentModelIndex> setupInProgress;

        QubeHardware::Predicate predicate;
        BookmarkManager *bookmarkManager;
        FilePlacesSharedBookmarks * sharedBookmarks;

        void reloadAndSignal();
        QList<FilePlacesItem *> loadBookmarkList();

        void _q_initDeviceList();
        void _q_deviceAdded(const QString &udi);
        void _q_deviceRemoved(const QString &udi);
        void _q_itemChanged(const QString &udi);
        void _q_reloadBookmarks();
        void _q_storageSetupDone(QubeHardware::ErrorType error, QVariant errorData);
        void _q_storageTeardownDone(QubeHardware::ErrorType error, QVariant errorData);
    };
}

#endif // FILEPLACESMODEL_P_H
