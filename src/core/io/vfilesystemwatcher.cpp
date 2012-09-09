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
 *    Flavio Castelli <flavio.castelli@gmail.com>
 *    David Faure <faure@kde.org>
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Dirk Mueller <mueller@kde.org>
 *    Sven Radej <sven@lisa.exp.univie.ac.at>
 *    Rafal Rzepecki <divided.mind@gmail.com>
 *
 * $BEGIN_LICENSE:LGPL-ONLY$
 *
 * This file may be used under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation and
 * appearing in the file LICENSE.LGPL included in the packaging of
 * this file, either version 2.1 of the License, or (at your option) any
 * later version.  Please review the following information to ensure the
 * GNU Lesser General Public License version 2.1 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 * If you have questions regarding the use of this file, please contact
 * us via http://www.maui-project.org/.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSocketNotifier>
#include <QTimer>
#include <QCoreApplication>

#include <VibeCore/VFileSupport>

#include "vfilesystemwatcher.h"
#include "vfilesystemwatcher_p.h"

#include <sys/ioctl.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

// Set this to true for much more verbose debug output
static const bool kVerboseDebug = false;

// The VFileSystemWatcherPrivate instance is refcounted, and deleted by the last VFileSystemWatcher instance
static VFileSystemWatcherPrivate *dwp_self = 0;
static VFileSystemWatcherPrivate *createPrivate()
{
    if (!dwp_self)
        dwp_self = new VFileSystemWatcherPrivate;
    return dwp_self;
}

/*
 * VFileSystemWatcherPrivate
 */

/* All entries (files/directories) to be watched in the
 * application (coming from multiple VFileSystemWatcher instances)
 * are registered in a single VFileSystemWatcherPrivate instance.
 */
VFileSystemWatcherPrivate::VFileSystemWatcherPrivate() :
    timer(),
    freq(3600000),   // 1 hour as upper bound
    statEntries(0),
    m_ref(0),
    delayRemove(false),
    rescan_all(false),
    rescan_timer()
{
    connect(&timer, SIGNAL(timeout()),
            this, SLOT(slotRescan()));

    //m_PollInterval = config.readEntry("PollInterval", 500);
    m_PollInterval = 500;

    rescan_timer.setSingleShot(true);
    connect(&rescan_timer, SIGNAL(timeout()),
            this, SLOT(slotRescan()));

    m_inotify_fd = inotify_init();
    Q_ASSERT(m_inotify_fd > 0);

    fcntl(m_inotify_fd, F_SETFD, FD_CLOEXEC);

    mSn = new QSocketNotifier(m_inotify_fd, QSocketNotifier::Read, this);
    connect(mSn, SIGNAL(activated(int)),
            this, SLOT(inotifyEventReceived()));
}

// This is called on app exit (when Q_GLOBAL_STATIC deletes VFileSystemWatcher::self)
VFileSystemWatcherPrivate::~VFileSystemWatcherPrivate()
{
    // Stop the timer
    timer.stop();

    // Remove all entries being watched
    removeEntries(0);

    // Close inotify
    ::close(m_inotify_fd);
}

void VFileSystemWatcherPrivate::inotifyEventReceived()
{
    int pending = -1;
    int offsetStartRead = 0; // where we read into buffer
    char buf[8192];

    Q_ASSERT(m_inotify_fd > -1);
    ioctl(m_inotify_fd, FIONREAD, &pending);

    while (pending > 0) {
        const int bytesToRead = qMin(pending, (int)sizeof(buf) - offsetStartRead);

        int bytesAvailable = read(m_inotify_fd, &buf[offsetStartRead], bytesToRead);
        pending -= bytesAvailable;
        bytesAvailable += offsetStartRead;
        offsetStartRead = 0;

        int offsetCurrent = 0;
        while (bytesAvailable >= (int)sizeof(struct inotify_event)) {
            const struct inotify_event *const event = (struct inotify_event *) &buf[offsetCurrent];
            const int eventSize = sizeof(struct inotify_event) + event->len;
            if (bytesAvailable < eventSize)
                break;

            bytesAvailable -= eventSize;
            offsetCurrent += eventSize;

            QString path;
            QByteArray cpath(event->name, event->len);
            if (event->len)
                path = QFile::decodeName(cpath);

            if (path.length() && isNoisyFile(cpath))
                continue;

            // Now we're in deep trouble of finding the associated entries,
            // for now, we suck and iterate
            for (EntryMap::Iterator it = m_mapEntries.begin();
                    it != m_mapEntries.end();) {
                Entry *e = &(*it);
                ++it;
                if (e->wd == event->wd) {
                    e->dirty = true;

                    /*
                    if (kVerboseDebug)
                    qDebug() << "got event" << "0x"+QString::number(event->mask, 16) << "for" << e->path;
                    */

                    if (event->mask & IN_DELETE_SELF) {
                        if (kVerboseDebug)
                            qDebug() << "-->got deleteself signal for" << e->path;
                        e->m_status = NonExistent;
                        e->wd = -1;
                        e->m_ctime = invalid_ctime;
                        emitEvent(e, Deleted, e->path);
                        // Add entry to parent dir to notice if the entry gets recreated
                        addEntry(0, e->parentDirectory(), e, true /*isDir*/);
                    }
                    if (event->mask & IN_IGNORED) {
                        // Causes bug #207361 with kernels 2.6.31 and 2.6.32!
                        //e->wd = -1;
                    }
                    if (event->mask & (IN_CREATE | IN_MOVED_TO)) {
                        const QString tpath = e->path + QLatin1Char('/') + path;
                        Entry *sub_entry = e->findSubEntry(tpath);

                        if (kVerboseDebug) {
                            qDebug() << "-->got CREATE signal for" << (tpath) << "sub_entry=" << sub_entry;
                            qDebug() << *e;
                        }

                        // The code below is very similar to the one in checkFAMEvent...
                        if (sub_entry) {
                            // We were waiting for this new file/dir to be created
                            sub_entry->dirty = true;
                            rescan_timer.start(0); // process this asap, to start watching that dir
                        } else if (e->isDir && !e->m_clients.empty()) {
                            bool isDir = false;
                            const QList<Client *> clients = e->clientsForFileOrDir(tpath, &isDir);
                            Q_FOREACH(Client * client, clients) {
                                // See discussion in addEntry for why we don't addEntry for individual
                                // files in WatchFiles mode with inotify.
                                if (isDir) {
                                    addEntry(client->instance, tpath, 0, isDir,
                                             isDir ? client->m_watchModes : VFileSystemWatcher::WatchDirOnly);
                                }
                            }
                            if (!clients.isEmpty()) {
                                emitEvent(e, Created, tpath);
                                qDebug().nospace() << clients.count() << " instance(s) monitoring the new "
                                                   << (isDir ? "dir " : "file ") << tpath;
                            }
                            e->m_pendingFileChanges.append(e->path);
                            if (!rescan_timer.isActive())
                                rescan_timer.start(m_PollInterval); // singleshot
                        }
                    }
                    if (event->mask & (IN_DELETE | IN_MOVED_FROM)) {
                        const QString tpath = e->path + QLatin1Char('/') + path;
                        if (kVerboseDebug)
                            qDebug() << "-->got DELETE signal for" << tpath;
                        if ((e->isDir) && (!e->m_clients.empty())) {
                            Client *client = 0;

                            // A file in this directory has been removed.  It wasn't an explicitly
                            // watched file as it would have its own watch descriptor, so
                            // no addEntry/ removeEntry bookkeeping should be required.  Emit
                            // the event immediately if any clients are interested.
                            Vibe_struct_stat stat_buf;

                            // Unlike clientsForFileOrDir, the stat can fail here (item deleted),
                            // so in that case we'll just take both kinds of clients and emit Deleted.
                            VFileSystemWatcher::WatchModes flag = VFileSystemWatcher::WatchSubDirs | VFileSystemWatcher::WatchFiles;
                            if (Vibe::stat(tpath, &stat_buf) == 0) {
                                bool isDir = S_ISDIR(stat_buf.st_mode);
                                flag = isDir ? VFileSystemWatcher::WatchSubDirs : VFileSystemWatcher::WatchFiles;
                            }
                            int counter = 0;
                            foreach(client, e->m_clients) { // krazy:exclude=foreach
                                if (client->m_watchModes & flag)
                                    counter++;
                            }
                            if (counter != 0)
                                emitEvent(e, Deleted, tpath);
                        }
                    }
                    if (event->mask & (IN_MODIFY | IN_ATTRIB)) {
                        if ((e->isDir) && (!e->m_clients.empty())) {
                            const QString tpath = e->path + QLatin1Char('/') + path;
                            if (kVerboseDebug)
                                qDebug() << "-->got MODIFY signal for" << (tpath);

                            // A file in this directory has been changed.  No
                            // addEntry/ removeEntry bookkeeping should be required.
                            // Add the path to the list of pending file changes if
                            // there are any interested clients.
                            //Vibe_struct_stat stat_buf;
                            //QByteArray tpath = QFile::encodeName(e->path+'/'+path);
                            //Vibe_stat(tpath, &stat_buf);
                            //bool isDir = S_ISDIR(stat_buf.st_mode);

                            // The API doc is somewhat vague as to whether we should emit
                            // dirty() for implicitly watched files when WatchFiles has
                            // not been specified - we'll assume they are always interested,
                            // regardless.
                            // Don't worry about duplicates for the time
                            // being; this is handled in slotRescan.
                            e->m_pendingFileChanges.append(tpath);
                        }
                    }

                    if (!rescan_timer.isActive())
                        rescan_timer.start(m_PollInterval); // singleshot

                    break;
                }
            }
        }
        if (bytesAvailable > 0) {
            // copy partial event to beginning of buffer
            memmove(buf, &buf[offsetCurrent], bytesAvailable);
            offsetStartRead = bytesAvailable;
        }
    }
}

/* In FAM mode, only entries which are marked dirty are scanned.
 * We first need to mark all yet nonexistent, but possible created
 * entries as dirty...
 */
void VFileSystemWatcherPrivate::Entry::propagate_dirty()
{
    foreach(Entry * sub_entry, m_entries) {
        if (!sub_entry->dirty) {
            sub_entry->dirty = true;
            sub_entry->propagate_dirty();
        }
    }
}


/* A VFileSystemWatcher instance is interested in getting events for
 * this file/Dir entry.
 */
void VFileSystemWatcherPrivate::Entry::addClient(VFileSystemWatcher *instance,
                                                 VFileSystemWatcher::WatchModes watchModes)
{
    if (instance == 0)
        return;

    foreach(Client * client, m_clients) {
        if (client->instance == instance) {
            client->count++;
            client->m_watchModes = watchModes;
            return;
        }
    }

    Client *client = new Client;
    client->instance = instance;
    client->count = 1;
    client->watchingStopped = instance->isStopped();
    client->pending = NoChange;
    client->m_watchModes = watchModes;

    m_clients.append(client);
}

void VFileSystemWatcherPrivate::Entry::removeClient(VFileSystemWatcher *instance)
{
    QList<Client *>::iterator it = m_clients.begin();
    const QList<Client *>::iterator end = m_clients.end();
    for (; it != end ; ++it) {
        Client *client = *it;
        if (client->instance == instance) {
            client->count--;
            if (client->count == 0) {
                m_clients.erase(it);
                delete client;
            }
            return;
        }
    }
}

/* get number of clients */
int VFileSystemWatcherPrivate::Entry::clientCount() const
{
    int clients = 0;
    foreach(Client * client, m_clients)
    clients += client->count;

    return clients;
}

QString VFileSystemWatcherPrivate::Entry::parentDirectory() const
{
    return QDir::cleanPath(path + QLatin1String("/.."));
}

QList<VFileSystemWatcherPrivate::Client *> VFileSystemWatcherPrivate::Entry::clientsForFileOrDir(const QString &tpath, bool *isDir) const
{
    QList<Client *> ret;
    Vibe_struct_stat stat_buf;
    if (Vibe::stat(tpath, &stat_buf) == 0) {
        *isDir = S_ISDIR(stat_buf.st_mode);
        const VFileSystemWatcher::WatchModes flag =
            *isDir ? VFileSystemWatcher::WatchSubDirs : VFileSystemWatcher::WatchFiles;
        Q_FOREACH(Client * client, this->m_clients) {
            if (client->m_watchModes & flag) {
                ret.append(client);
            }
        }
    } else {
        // Happens frequently, e.g. ERROR: couldn't stat "/home/dfaure/.viminfo.tmp"
        //qDebug() << "ERROR: couldn't stat" << tpath;
    }
    // If Vibe_stat fails then isDir is not set, but ret is empty anyway
    // so isDir won't be used.
    return ret;
}

QDebug operator<<(QDebug debug, const VFileSystemWatcherPrivate::Entry &entry)
{
    debug.nospace() << "[ Entry for " << entry.path << ", " << (entry.isDir ? "dir" : "file");
    if (entry.m_status == VFileSystemWatcherPrivate::NonExistent)
        debug << ", non-existent";
    debug << " inotify_wd=" << entry.wd;
    debug << ", has " << entry.m_clients.count() << " clients";
    debug.space();
    if (!entry.m_entries.isEmpty()) {
        debug << ", nonexistent subentries:";
        foreach(VFileSystemWatcherPrivate::Entry * subEntry, entry.m_entries)
        debug << subEntry << subEntry->path;
    }
    debug << ']';
    return debug;
}

VFileSystemWatcherPrivate::Entry *VFileSystemWatcherPrivate::entry(const QString &_path)
{
    // We only support absolute paths
    if (_path.isEmpty() || QDir::isRelativePath(_path))
        return 0;

    QString path(_path);

    if (path.length() > 1 && path.endsWith(QLatin1Char('/')))
        path.truncate(path.length() - 1);

    EntryMap::Iterator it = m_mapEntries.find(path);
    if (it == m_mapEntries.end())
        return 0;
    return &(*it);
}

// set polling frequency for a entry and adjust global freq if needed
void VFileSystemWatcherPrivate::useFreq(Entry *e, int newFreq)
{
    e->freq = newFreq;

    // A reasonable frequency for the global polling timer
    if (e->freq < freq) {
        freq = e->freq;

        if (timer.isActive())
            timer.start(freq);

        qDebug() << "Global Poll Freq is now" << freq << "msec";
    }
}

// setup INotify notification, returns false if not possible
bool VFileSystemWatcherPrivate::useINotify(Entry *e)
{
    e->wd = -1;
    e->dirty = false;

    if (e->m_status == NonExistent) {
        addEntry(0, e->parentDirectory(), e, true);
        return true;
    }

    // May as well register for almost everything - it's free!
    int mask = IN_DELETE | IN_DELETE_SELF | IN_CREATE | IN_MOVE | IN_MOVE_SELF |
               IN_DONT_FOLLOW | IN_MOVED_FROM | IN_MODIFY | IN_ATTRIB;

    if ((e->wd = inotify_add_watch(m_inotify_fd,
                                   QFile::encodeName(e->path), mask)) >= 0) {
        if (kVerboseDebug)
            qDebug() << "inotify successfully used for monitoring" << e->path << "wd=" << e->wd;
        return true;
    }

    qDebug() << "inotify failed for monitoring" << e->path << ":" << strerror(errno);
    return false;
}

/* If <instance> !=0, this VFileSystemWatcher instance wants to watch at <_path>,
 * providing in <isDir> the type of the entry to be watched.
 * Sometimes, entries are dependant on each other: if <sub_entry> !=0,
 * this entry needs another entry to watch himself (when notExistent).
 */
void VFileSystemWatcherPrivate::addEntry(VFileSystemWatcher *instance, const QString &_path,
                                         Entry *sub_entry, bool isDir, VFileSystemWatcher::WatchModes watchModes)
{
    // Don't even try to go further if path is not valid
    QString path(_path);
    if (path.isEmpty()
#ifndef Q_WS_WIN
            || path == QLatin1String("/dev")
            || (path.startsWith(QLatin1String("/dev/")) && !path.startsWith(QLatin1String("/dev/.")))
#endif
       )
        return;

    if (path.length() > 1 && path.endsWith(QLatin1Char('/')))
        path.truncate(path.length() - 1);

    EntryMap::Iterator it = m_mapEntries.find(path);
    if (it != m_mapEntries.end()) {
        if (sub_entry) {
            (*it).m_entries.append(sub_entry);
            if (kVerboseDebug) {
                qDebug() << "Added already watched Entry" << path
                         << "(for" << sub_entry->path << ")";
            }

            Entry *e = &(*it);
            if (e->wd >= 0) {
                int mask = IN_DELETE | IN_DELETE_SELF | IN_CREATE | IN_MOVE | IN_MOVE_SELF | IN_DONT_FOLLOW;
                if (!e->isDir)
                    mask |= IN_MODIFY | IN_ATTRIB;
                else
                    mask |= IN_ONLYDIR;

                inotify_rm_watch(m_inotify_fd, e->wd);
                e->wd = inotify_add_watch(m_inotify_fd, QFile::encodeName(e->path),
                                          mask);
                //Q_ASSERT(e->wd >= 0); // fails in KDirListerTest::testDeleteCurrentDir
            }
        } else {
            (*it).addClient(instance, watchModes);
            if (kVerboseDebug) {
                qDebug() << "Added already watched Entry" << path
                         << "(now" << (*it).clientCount() << "clients)"
                         << QString::fromLatin1("[%1]").arg(instance->objectName());
            }
        }
        return;
    }

    // we have a new path to watch

    Vibe_struct_stat stat_buf;
    bool exists = (Vibe::stat(path, &stat_buf) == 0);

    EntryMap::iterator newIt = m_mapEntries.insert(path, Entry());
    // the insert does a copy, so we have to use <e> now
    Entry *e = &(*newIt);

    if (exists) {
        e->isDir = S_ISDIR(stat_buf.st_mode);

        if (e->isDir && !isDir) {
            Vibe::lstat(path, &stat_buf);
            if (S_ISLNK(stat_buf.st_mode))
                // if it's a symlink, don't follow it
                e->isDir = false;
            else
                qWarning() << "VFileSystemWatcher:" << path << "is a directory. Use addDir!";
        } else if (!e->isDir && isDir)
            qWarning("VFileSystemWatcher: %s is a file. Use addFile!", qPrintable(path));

        if (!e->isDir && (watchModes != VFileSystemWatcher::WatchDirOnly)) {
            qWarning() << "VFileSystemWatcher:" << path << "is a file. You can't use recursive or "
                       "watchFiles options";
            watchModes = VFileSystemWatcher::WatchDirOnly;
        }

#ifdef Q_OS_WIN
        // ctime is the 'creation time' on windows - use mtime instead
        e->m_ctime = stat_buf.st_mtime;
#else
        e->m_ctime = stat_buf.st_ctime;
#endif
        e->m_status = Normal;
        e->m_nlink = stat_buf.st_nlink;
        e->m_ino = stat_buf.st_ino;
    } else {
        e->isDir = isDir;
        e->m_ctime = invalid_ctime;
        e->m_status = NonExistent;
        e->m_nlink = 0;
        e->m_ino = 0;
    }

    e->path = path;
    if (sub_entry)
        e->m_entries.append(sub_entry);
    else
        e->addClient(instance, watchModes);

    qDebug().nospace() << "Added " << (e->isDir ? "Dir " : "File ") << path
                       << (e->m_status == NonExistent ? " NotExisting" : "")
                       << " for " << (sub_entry ? sub_entry->path : QString())
                       << " [" << (instance ? instance->objectName() : QString()) << "]";

    e->msecLeft = 0;

    if (isNoisyFile(QFile::encodeName(path)))
        return;

    if (exists && e->isDir && (watchModes != VFileSystemWatcher::WatchDirOnly)) {
        QFlags<QDir::Filter> filters = QDir::NoDotAndDotDot;

        if ((watchModes & VFileSystemWatcher::WatchSubDirs) &&
                (watchModes & VFileSystemWatcher::WatchFiles)) {
            filters |= (QDir::Dirs | QDir::Files);
        } else if (watchModes & VFileSystemWatcher::WatchSubDirs) {
            filters |= QDir::Dirs;
        } else if (watchModes & VFileSystemWatcher::WatchFiles) {
            filters |= QDir::Files;
        }

        //qDebug() << "Ignoring WatchFiles directive - this is implicit with inotify";
        // Placing a watch on individual files is redundant with inotify
        // (inotify gives us WatchFiles functionality "for free") and indeed
        // actively harmful, so prevent it.  WatchSubDirs is necessary, though.
        filters &= ~QDir::Files;

        QDir basedir(e->path);
        const QFileInfoList contents = basedir.entryInfoList(filters);
        for (QFileInfoList::const_iterator iter = contents.constBegin();
                iter != contents.constEnd(); ++iter) {
            const QFileInfo &fileInfo = *iter;
            // treat symlinks as files--don't follow them.
            bool isDir = fileInfo.isDir() && !fileInfo.isSymLink();

            addEntry(instance, fileInfo.absoluteFilePath(), 0, isDir,
                     isDir ? watchModes : VFileSystemWatcher::WatchDirOnly);
        }
    }

    addWatch(e);
}

void VFileSystemWatcherPrivate::addWatch(Entry *e)
{
    useINotify(e);
}

void VFileSystemWatcherPrivate::removeWatch(Entry *e)
{
    (void) inotify_rm_watch(m_inotify_fd, e->wd);
    if (kVerboseDebug) {
        qDebug().nospace() << "Cancelled INotify (fd " << m_inotify_fd << ", "
                           << e->wd << ") for " << e->path;
    }
}

void VFileSystemWatcherPrivate::removeEntry(VFileSystemWatcher *instance,
                                            const QString &_path,
                                            Entry *sub_entry)
{
    if (kVerboseDebug)
        qDebug() << "path=" << _path << "sub_entry:" << sub_entry;

    Entry *e = entry(_path);
    if (!e) {
        qWarning() << "doesn't know" << _path;
        return;
    }

    removeEntry(instance, e, sub_entry);
}

void VFileSystemWatcherPrivate::removeEntry(VFileSystemWatcher *instance,
                                            Entry *e,
                                            Entry *sub_entry)
{
    removeList.remove(e);

    if (sub_entry)
        e->m_entries.removeAll(sub_entry);
    else
        e->removeClient(instance);

    if (e->m_clients.count() || e->m_entries.count())
        return;

    if (delayRemove) {
        removeList.insert(e);
        // now e->isValid() is false
        return;
    }

    if (e->m_status == Normal) {
        removeWatch(e);
    } else {
        // Removed a NonExistent entry - we just remove it from the parent
        if (e->isDir)
            removeEntry(0, e->parentDirectory(), e);
        else
            removeEntry(0, QFileInfo(e->path).absolutePath(), e);
    }

    if (kVerboseDebug) {
        qDebug().nospace() << "Removed " << (e->isDir ? "Dir " : "File ") << e->path
                           << " for " << (sub_entry ? sub_entry->path : QString())
                           << " [" << (instance ? instance->objectName() : QString()) << "]";
    }
    m_mapEntries.remove(e->path);   // <e> not valid any more
}

/* Called from VFileSystemWatcher destructor:
 * remove <instance> as client from all entries
 */
void VFileSystemWatcherPrivate::removeEntries(VFileSystemWatcher *instance)
{
    int minfreq = 3600000;

    QStringList pathList;

    // Put all entries where instance is a client in list
    EntryMap::Iterator it = m_mapEntries.begin();
    for (; it != m_mapEntries.end(); ++it) {
        Client *c = 0;
        foreach(Client * client, (*it).m_clients) {
            if (client->instance == instance) {
                c = client;
                break;
            }
        }
        if (c) {
            c->count = 1; // forces deletion of instance as client
            pathList.append((*it).path);
        }
    }

    foreach(const QString & path, pathList)
    removeEntry(instance, path, 0);

    if (minfreq > freq) {
        // We can decrease the global polling frequency
        freq = minfreq;
        if (timer.isActive())
            timer.start(freq);
        qDebug() << "Poll Freq now" << freq << "msec";
    }
}

// instance ==0: stop scanning for all instances
bool VFileSystemWatcherPrivate::stopEntryScan(VFileSystemWatcher *instance, Entry *e)
{
    int stillWatching = 0;
    foreach(Client * client, e->m_clients) {
        if (!instance || instance == client->instance)
            client->watchingStopped = true;
        else if (!client->watchingStopped)
            stillWatching += client->count;
    }

    qDebug()  << (instance ? instance->objectName() : QString::fromLatin1("all"))
              << "stopped scanning" << e->path << "(now"
              << stillWatching << "watchers)";

    return true;
}

// instance ==0: start scanning for all instances
bool VFileSystemWatcherPrivate::restartEntryScan(VFileSystemWatcher *instance, Entry *e,
                                                 bool notify)
{
    int wasWatching = 0, newWatching = 0;
    foreach(Client * client, e->m_clients) {
        if (!client->watchingStopped)
            wasWatching += client->count;
        else if (!instance || instance == client->instance) {
            client->watchingStopped = false;
            newWatching += client->count;
        }
    }
    if (newWatching == 0)
        return false;

    qDebug() << (instance ? instance->objectName() : QString::fromLatin1("all"))
             << "restarted scanning" << e->path
             << "(now" << wasWatching + newWatching << "watchers)";

    // restart watching and emit pending events
    int ev = NoChange;
    if (wasWatching == 0) {
        if (!notify) {
            Vibe_struct_stat stat_buf;
            bool exists = (Vibe::stat(e->path, &stat_buf) == 0);
            if (exists) {
#ifdef Q_OS_WIN
                // ctime is the 'creation time' on windows - use mtime instead
                e->m_ctime = stat_buf.st_mtime;
#else
                e->m_ctime = stat_buf.st_ctime;
#endif
                e->m_status = Normal;
                if (kVerboseDebug)
                    qDebug() << "Setting status to Normal for" << e << e->path;
                e->m_nlink = stat_buf.st_nlink;
                e->m_ino = stat_buf.st_ino;

                // Same as in scanEntry: ensure no subentry in parent dir
                removeEntry(0, e->parentDirectory(), e);
            } else {
                e->m_ctime = invalid_ctime;
                e->m_status = NonExistent;
                e->m_nlink = 0;
                if (kVerboseDebug)
                    qDebug() << "Setting status to NonExistent for" << e << e->path;
            }
        }
        e->msecLeft = 0;
        ev = scanEntry(e);
    }
    emitEvent(e, ev);

    return true;
}

// instance ==0: stop scanning for all instances
void VFileSystemWatcherPrivate::stopScan(VFileSystemWatcher *instance)
{
    EntryMap::Iterator it = m_mapEntries.begin();
    for (; it != m_mapEntries.end(); ++it)
        stopEntryScan(instance, &(*it));
}


void VFileSystemWatcherPrivate::startScan(VFileSystemWatcher *instance,
                                          bool notify, bool skippedToo)
{
    if (!notify)
        resetList(instance, skippedToo);

    EntryMap::Iterator it = m_mapEntries.begin();
    for (; it != m_mapEntries.end(); ++it)
        restartEntryScan(instance, &(*it), notify);

    // timer should still be running when in polling mode
}


// clear all pending events, also from stopped
void VFileSystemWatcherPrivate::resetList(VFileSystemWatcher * /*instance*/, bool skippedToo)
{
    EntryMap::Iterator it = m_mapEntries.begin();
    for (; it != m_mapEntries.end(); ++it) {

        foreach(Client * client, (*it).m_clients) {
            if (!client->watchingStopped || skippedToo)
                client->pending = NoChange;
        }
    }
}

// Return event happened on <e>
int VFileSystemWatcherPrivate::scanEntry(Entry *e)
{
    // we know nothing has changed, no need to stat
    if (!e->dirty) return NoChange;
    e->dirty = false;

    Vibe_struct_stat stat_buf;
    const bool exists = (Vibe::stat(e->path, &stat_buf) == 0);
    if (exists) {

        if (e->m_status == NonExistent) {
            // ctime is the 'creation time' on windows, but with qMax
            // we get the latest change of any kind, on any platform.
            e->m_ctime = qMax(stat_buf.st_ctime, stat_buf.st_mtime);
            e->m_status = Normal;
            e->m_ino = stat_buf.st_ino;
            if (kVerboseDebug)
                qDebug() << "Setting status to Normal for just created" << e << e->path;
            // We need to make sure the entry isn't listed in its parent's subentries... (#222974, testMoveTo)
            removeEntry(0, e->parentDirectory(), e);

            return Created;
        }

        if (kVerboseDebug) {
            struct tm *tmp = localtime(&e->m_ctime);
            char outstr[200];
            strftime(outstr, sizeof(outstr), "%T", tmp);
            qDebug() << "e->m_ctime=" << e->m_ctime << outstr
                     << "stat_buf.st_ctime=" << stat_buf.st_ctime
                     << "e->m_nlink=" << e->m_nlink
                     << "stat_buf.st_nlink=" << stat_buf.st_nlink
                     << "e->m_ino=" << e->m_ino
                     << "stat_buf.st_ino=" << stat_buf.st_ino;
        }

        if (((e->m_ctime != invalid_ctime) &&
                (qMax(stat_buf.st_ctime, stat_buf.st_mtime) != e->m_ctime ||
                 stat_buf.st_ino != e->m_ino ||
                 stat_buf.st_nlink != nlink_t(e->m_nlink)))
#ifdef Q_OS_WIN
                // we trust QFSW to get it right, the ctime comparisons above
                // fail for example when adding files to directories on Windows
                // which doesn't change the mtime of the directory
                || e->m_mode == QFSWatchMode
#endif
           ) {
            e->m_ctime = qMax(stat_buf.st_ctime, stat_buf.st_mtime);
            e->m_nlink = stat_buf.st_nlink;
            if (e->m_ino != stat_buf.st_ino) {
                // The file got deleted and recreated. We need to watch it again.
                removeWatch(e);
                addWatch(e);
            }
            e->m_ino = stat_buf.st_ino;
            return Changed;
        }

        return NoChange;
    }

    // dir/file doesn't exist

    e->m_nlink = 0;
    e->m_ino = 0;
    e->m_status = NonExistent;

    if (e->m_ctime == invalid_ctime)
        return NoChange;

    e->m_ctime = invalid_ctime;
    return Deleted;
}

/* Notify all interested VFileSystemWatcher instances about a given event on an entry
 * and stored pending events. When watching is stopped, the event is
 * added to the pending events.
 */
void VFileSystemWatcherPrivate::emitEvent(const Entry *e, int event, const QString &fileName)
{
    QString path(e->path);
    if (!fileName.isEmpty()) {
        if (!QDir::isRelativePath(fileName))
            path = fileName;
        else {
#ifdef Q_OS_UNIX
            path += QLatin1Char('/') + fileName;
#elif defined(Q_WS_WIN)
            //current drive is passed instead of /
            path += QDir::currentPath().left(2) + QLatin1Char('/') + fileName;
#endif
        }
    }

    if (kVerboseDebug)
        qDebug() << event << path << e->m_clients.count() << "clients";

    foreach(Client * c, e->m_clients) {
        if (c->instance == 0 || c->count == 0) continue;

        if (c->watchingStopped) {
            // add event to pending...
            if (event == Changed)
                c->pending |= event;
            else if (event == Created || event == Deleted)
                c->pending = event;
            continue;
        }
        // not stopped
        if (event == NoChange || event == Changed)
            event |= c->pending;
        c->pending = NoChange;
        if (event == NoChange) continue;

        // Emit the signals delayed, to avoid unexpected re-entrancy from the slots (#220153)

        if (event & Deleted) {
            QMetaObject::invokeMethod(c->instance, "setDeleted", Qt::QueuedConnection, Q_ARG(QString, path));
            // emit only Deleted event...
            continue;
        }

        if (event & Created) {
            QMetaObject::invokeMethod(c->instance, "setCreated", Qt::QueuedConnection, Q_ARG(QString, path));
            // possible emit Change event after creation
        }

        if (event & Changed) {
            QMetaObject::invokeMethod(c->instance, "setDirty", Qt::QueuedConnection, Q_ARG(QString, path));
        }
    }
}

// Remove entries which were marked to be removed
void VFileSystemWatcherPrivate::slotRemoveDelayed()
{
    delayRemove = false;
    // Removing an entry could also take care of removing its parent
    // (e.g. in FAM or inotify mode), which would remove other entries in removeList,
    // so don't use foreach or iterators here...
    while (!removeList.isEmpty()) {
        Entry *entry = *removeList.begin();
        removeEntry(0, entry, 0); // this will remove entry from removeList
    }
}

/* Scan all entries to be watched for changes. This is done regularly
 * when polling. FAM and inotify use a single-shot timer to call this slot delayed.
 */
void VFileSystemWatcherPrivate::slotRescan()
{
    if (kVerboseDebug)
        qDebug();

    EntryMap::Iterator it;

    // People can do very long things in the slot connected to dirty(),
    // like showing a message box. We don't want to keep polling during
    // that time, otherwise the value of 'delayRemove' will be reset.
    // ### TODO: now the emitEvent delays emission, this can be cleaned up
    bool timerRunning = timer.isActive();
    if (timerRunning)
        timer.stop();

    // We delay deletions of entries this way.
    // removeDir(), when called in slotDirty(), can cause a crash otherwise
    // ### TODO: now the emitEvent delays emission, this can be cleaned up
    delayRemove = true;

    if (rescan_all) {
        // mark all as dirty
        it = m_mapEntries.begin();
        for (; it != m_mapEntries.end(); ++it)
            (*it).dirty = true;
        rescan_all = false;
    } else {
        // progate dirty flag to dependant entries (e.g. file watches)
        it = m_mapEntries.begin();
        for (; it != m_mapEntries.end(); ++it)
            if ((*it).dirty)
                (*it).propagate_dirty();
    }

    QList<Entry *> cList;

    it = m_mapEntries.begin();
    for (; it != m_mapEntries.end(); ++it) {
        // we don't check invalid entries (i.e. remove delayed)
        Entry *entry = &(*it);
        if (!entry->isValid()) continue;

        const int ev = scanEntry(entry);
        if (kVerboseDebug)
            qDebug() << "scanEntry for" << entry->path << "says" << ev;

        if (ev == Deleted) {
            if (kVerboseDebug)
                qDebug() << "scanEntry says" << entry->path << "was deleted";
            addEntry(0, entry->parentDirectory(), entry, true);
        } else if (ev == Created) {
            if (kVerboseDebug)
                qDebug() << "scanEntry says" << entry->path << "was created. wd=" << entry->wd;
            if (entry->wd < 0) {
                cList.append(entry);
                addWatch(entry);
            }
        }

        if (entry->isDir) {
            // Report and clear the the list of files that have changed in this directory.
            // Remove duplicates by changing to set and back again:
            // we don't really care about preserving the order of the
            // original changes.
            QStringList pendingFileChanges = entry->m_pendingFileChanges;
            pendingFileChanges.removeDuplicates();
            Q_FOREACH(const QString & changedFilename, pendingFileChanges) {
                if (kVerboseDebug) {
                    qDebug() << "processing pending file change for" << changedFilename;
                }
                emitEvent(entry, Changed, changedFilename);
            }
            entry->m_pendingFileChanges.clear();
        }

        if (ev != NoChange) {
            emitEvent(entry, ev);
        }
    }

    if (timerRunning)
        timer.start(freq);

    // Remove watch of parent of new created directories
    Q_FOREACH(Entry * e, cList)
    removeEntry(0, e->parentDirectory(), e);

    QTimer::singleShot(0, this, SLOT(slotRemoveDelayed()));
}

bool VFileSystemWatcherPrivate::isNoisyFile(const char *filename)
{
    // $HOME/.X.err grows with debug output, so don't notify change
    if (*filename == '.') {
        if (strncmp(filename, ".X.err", 6) == 0) return true;
        if (strncmp(filename, ".xsession-errors", 16) == 0) return true;
        // fontconfig updates the cache on every KDE app start
        // (inclusive kio_thumbnail slaves)
        if (strncmp(filename, ".fonts.cache", 12) == 0) return true;
    }

    return false;
}

void VFileSystemWatcherPrivate::statistics()
{
    EntryMap::Iterator it;

    qDebug() << "Entries watched:";
    if (m_mapEntries.count() == 0) {
        qDebug() << "  None.";
    } else {
        it = m_mapEntries.begin();
        for (; it != m_mapEntries.end(); ++it) {
            Entry *e = &(*it);
            qDebug() << "  " << *e;

            foreach(Client * c, e->m_clients) {
                QByteArray pending;
                if (c->watchingStopped) {
                    if (c->pending & Deleted)
                        pending += "deleted ";
                    if (c->pending & Created)
                        pending += "created ";
                    if (c->pending & Changed)
                        pending += "changed ";
                    if (!pending.isEmpty())
                        pending = " (pending: " + pending + ')';
                    pending = ", stopped" + pending;
                }
                qDebug() << "    by " << c->instance->objectName()
                         << " (" << c->count << " times)" << pending;
            }
            if (e->m_entries.count() > 0) {
                qDebug() << "    dependent entries:";
                foreach(Entry * d, e->m_entries) {
                    qDebug() << "      " << d << d->path
                             << (d->m_status == NonExistent ? "NonExistent" : "EXISTS!!! ERROR!");
                    if (kVerboseDebug)
                        Q_ASSERT(d->m_status == NonExistent); // it doesn't belong here otherwise
                }
            }
        }
    }
}

/*
 * VFileSystemWatcher
 */

Q_GLOBAL_STATIC(VFileSystemWatcher, s_pVFileSystemWatcherSelf)
VFileSystemWatcher *VFileSystemWatcher::self()
{
    return s_pVFileSystemWatcherSelf();
}

// TODO KDE5: is this used anywhere?
bool VFileSystemWatcher::exists()
{
    return s_pVFileSystemWatcherSelf()->exists();
}

VFileSystemWatcher::VFileSystemWatcher(QObject *parent) :
    QObject(parent),
    d(createPrivate())
{
    static int nameCounter = 0;

    nameCounter++;
    setObjectName(QString::fromLatin1("VFileSystemWatcher-%1").arg(nameCounter));

    d->ref();

    d->_isStopped = false;

    static bool cleanupRegistered = false;
    if (!cleanupRegistered)
        cleanupRegistered = true;
}

VFileSystemWatcher::~VFileSystemWatcher()
{
    d->removeEntries(this);
    if (d->deref()) {
        // delete it if it's the last one
        delete d;
        dwp_self = 0;
    }
}

void VFileSystemWatcher::addDir(const QString &_path, WatchModes watchModes)
{
    if (d)
        d->addEntry(this, _path, 0, true, watchModes);
}

void VFileSystemWatcher::addFile(const QString &_path)
{
    if (!d)
        return;

    d->addEntry(this, _path, 0, false);
}

QDateTime VFileSystemWatcher::ctime(const QString &_path) const
{
    VFileSystemWatcherPrivate::Entry *e = d->entry(_path);

    if (!e)
        return QDateTime();

    return QDateTime::fromTime_t(e->m_ctime);
}

void VFileSystemWatcher::removeDir(const QString &_path)
{
    if (d)
        d->removeEntry(this, _path, 0);
}

void VFileSystemWatcher::removeFile(const QString &_path)
{
    if (d)
        d->removeEntry(this, _path, 0);
}

bool VFileSystemWatcher::stopDirScan(const QString &_path)
{
    if (d) {
        VFileSystemWatcherPrivate::Entry *e = d->entry(_path);
        if (e && e->isDir) return d->stopEntryScan(this, e);
    }

    return false;
}

bool VFileSystemWatcher::restartDirScan(const QString &_path)
{
    if (d) {
        VFileSystemWatcherPrivate::Entry *e = d->entry(_path);
        if (e && e->isDir)
            // Restart without notifying pending events
            return d->restartEntryScan(this, e, false);
    }

    return false;
}

void VFileSystemWatcher::stopScan()
{
    if (d) {
        d->stopScan(this);
        d->_isStopped = true;
    }
}

bool VFileSystemWatcher::isStopped()
{
    return d->_isStopped;
}

void VFileSystemWatcher::startScan(bool notify, bool skippedToo)
{
    if (d) {
        d->_isStopped = false;
        d->startScan(this, notify, skippedToo);
    }
}

bool VFileSystemWatcher::contains(const QString &_path) const
{
    VFileSystemWatcherPrivate::Entry *e = d->entry(_path);
    if (!e)
        return false;

    foreach(VFileSystemWatcherPrivate::Client * client, e->m_clients) {
        if (client->instance == this)
            return true;
    }

    return false;
}

void VFileSystemWatcher::statistics()
{
    if (!dwp_self)
        return;
    dwp_self->statistics();
}

void VFileSystemWatcher::setCreated(const QString &_file)
{
    emit created(_file);
}

void VFileSystemWatcher::setDirty(const QString &_file)
{
    emit dirty(_file);
}

void VFileSystemWatcher::setDeleted(const QString &_file)
{
    emit deleted(_file);
}

#include "moc_vfilesystemwatcher.cpp"
#include "moc_vfilesystemwatcher_p.cpp"
