/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 1998 Sven Radej
 * Copyright (c) 2006 Dirk Mueller <mueller@kde.org>
 * Copyright (c) 2007 Flavio Castelli <flavio.castelli@gmail.com>
 * Copyright (c) 2008 Rafal Rzepecki <divided.mind@gmail.com>
 * Copyright (c) 2010 David Faure <faure@kde.org>
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Flavio Castelli <flavio.castelli@gmail.com>
 *	David Faure <faure@kde.org>
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *	Dirk Mueller <mueller@kde.org>
 *	Sven Radej <sven@lisa.exp.univie.ac.at>
 *	Rafal Rzepecki <divided.mind@gmail.com>
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

#ifndef VFILESYSTEMWATCHER_P_H
#define VFILESYSTEMWATCHER_P_H

#include <QList>
#include <QSet>
#include <QMap>
#include <QObject>
#include <QString>
#include <QTimer>

class QSocketNotifier;

#include <unistd.h>
#include <fcntl.h>
#include <sys/inotify.h>

#ifndef IN_DONT_FOLLOW
#define IN_DONT_FOLLOW 0x02000000
#endif

#ifndef IN_ONLYDIR
#define IN_ONLYDIR 0x01000000
#endif

#include <sys/time.h>
#include <sys/param.h>
#include <ctime>

#define invalid_ctime ((time_t)-1)

/* VFileSystemWatcherPrivate is a singleton and does the watching
 * for every VFileSystemWatcher instance in the application.
 */
class VFileSystemWatcherPrivate : public QObject
{
    Q_OBJECT
public:
    enum entryStatus {
        Normal = 0,
        NonExistent
    };

    enum {
        NoChange = 0,
        Changed = 1,
        Created = 2,
        Deleted = 4
    };

    struct Client {
        VFileSystemWatcher *instance;
        int count;
        // did the instance stop watching
        bool watchingStopped;
        // events blocked when stopped
        int pending;
        VFileSystemWatcher::WatchModes m_watchModes;
    };

    class Entry
    {
    public:
        // The last observed modification time
        time_t m_ctime;
        // The last observed link count
        int m_nlink;
        // Last observed inode
        ino_t m_ino;
        entryStatus m_status;
        bool isDir;
        // Instances interested in events
        QList<Client *> m_clients;
        // Nonexistent entries of this directory
        QList<Entry *> m_entries;
        QString path;

        int msecLeft, freq;

        QString parentDirectory() const;
        void addClient(VFileSystemWatcher *, VFileSystemWatcher::WatchModes);
        void removeClient(VFileSystemWatcher *);
        int clientCount() const;
        bool isValid() {
            return m_clients.count() || m_entries.count();
        }

        Entry *findSubEntry(const QString &path) const {
            foreach(Entry * sub_entry, m_entries) {
                if (sub_entry->path == path)
                    return sub_entry;
            }
            return 0;
        }

        bool dirty;
        void propagate_dirty();

        QList<Client *> clientsForFileOrDir(const QString &tpath, bool *isDir) const;

        // inotify file descriptor
        int wd;

        // Creation and Deletion of files happens infrequently, so
        // can safely be reported as they occur.  File changes i.e. those that emity "dirty()" can
        // happen many times per second, though, so maintain a list of files in this directory
        // that can be emitted and flushed at the next slotRescan(...).
        // This will be unused if the Entry is not a directory.
        QList<QString> m_pendingFileChanges;
    };

    typedef QMap<QString, Entry> EntryMap;

    VFileSystemWatcherPrivate();
    ~VFileSystemWatcherPrivate();

    void resetList(VFileSystemWatcher *, bool);
    void useFreq(Entry *e, int newFreq);
    void addEntry(VFileSystemWatcher *instance, const QString &_path, Entry *sub_entry,
                  bool isDir, VFileSystemWatcher::WatchModes watchModes = VFileSystemWatcher::WatchDirOnly);
    void removeEntry(VFileSystemWatcher *, const QString &, Entry *sub_entry);
    void removeEntry(VFileSystemWatcher *, Entry *e, Entry *sub_entry);
    bool stopEntryScan(VFileSystemWatcher *, Entry *);
    bool restartEntryScan(VFileSystemWatcher *, Entry *, bool);
    void stopScan(VFileSystemWatcher *);
    void startScan(VFileSystemWatcher *, bool, bool);

    void removeEntries(VFileSystemWatcher *);
    void statistics();

    void addWatch(Entry *entry);
    void removeWatch(Entry *entry);
    Entry *entry(const QString &);
    int scanEntry(Entry *e);
    void emitEvent(const Entry *e, int event, const QString &fileName = QString());

    // Memory management - delete when last VFileSystemWatcher gets deleted
    void ref() {
        m_ref++;
    }
    bool deref() {
        return (--m_ref == 0);
    }

    static bool isNoisyFile(const char *filename);

public slots:
    void slotRescan();
    void inotifyEventReceived();
    void slotRemoveDelayed();

public:
    QTimer timer;
    EntryMap m_mapEntries;

    int freq;
    int statEntries;
    int m_PollInterval;
    int m_ref;
    bool useStat(Entry *);

    // removeList is allowed to contain any entry at most once
    QSet<Entry *> removeList;
    bool delayRemove;

    bool rescan_all;
    QTimer rescan_timer;

    QSocketNotifier *mSn;
    bool supports_inotify;
    int m_inotify_fd;

    bool useINotify(Entry *);

    bool _isStopped;
};

QDebug operator<<(QDebug debug, const VFileSystemWatcherPrivate::Entry &entry);

#endif // VFILESYSTEMWATCHER_P_H
