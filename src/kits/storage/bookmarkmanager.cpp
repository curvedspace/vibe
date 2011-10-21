// -*- c-basic-offset:4; indent-tabs-mode:nil -*-
// vim: set ts=4 sts=4 sw=4 et:
/* This file is part of the KDE libraries
   Copyright (C) 2000 David Faure <faure@kde.org>
   Copyright (C) 2003 Alexander Kellett <lypanov@kde.org>
   Copyright (C) 2008 Norbert Frese <nf2@scheinwelt.at>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QProcess>
#include <QtCore/QRegExp>
#include <QtCore/QTextStream>
#include <QtCore/QFileSystemWatcher>
#include <QtDBus/QtDBus>
#include <QtGui/QApplication>

#include <QubeCore/SaveFile>

#include "bookmarkmanager.h"

#define BOOKMARK_CHANGE_NOTIFY_INTERFACE "org.kde.KIO.BookmarkManager"

namespace QubeStorage
{
    /*
     * BookmarkManagerList
     */

    class BookmarkManagerList : public QList<BookmarkManager *>
    {
    public:
        ~BookmarkManagerList() {
            qDeleteAll(begin() , end()); // auto-delete functionality
        }

        QReadWriteLock lock;
    };

    QUBE_GLOBAL_STATIC(BookmarkManagerList, s_pSelf)

    /*
     * BookmarkMap
     */

    class BookmarkMap : private BookmarkGroupTraverser
    {
    public:
        BookmarkMap() : m_mapNeedsUpdate(true) {}
        void setNeedsUpdate() {
            m_mapNeedsUpdate = true;
        }
        void update(BookmarkManager*);
        QList<Bookmark> find(const QString &url) const {
            return m_bk_map.value(url);
        }

    private:
        virtual void visit(const Bookmark &);
        virtual void visitEnter(const BookmarkGroup &) {
            ;
        }
        virtual void visitLeave(const BookmarkGroup &) {
            ;
        }

        typedef QList<Bookmark> BookmarkList;
        QMap<QString, BookmarkList> m_bk_map;
        bool m_mapNeedsUpdate;
    };

    void BookmarkMap::update(BookmarkManager *manager)
    {
        if (m_mapNeedsUpdate) {
            m_mapNeedsUpdate = false;

            m_bk_map.clear();
            BookmarkGroup root = manager->root();
            traverse(root);
        }
    }

    void BookmarkMap::visit(const Bookmark &bk)
    {
        if (!bk.isSeparator()) {
            // add bookmark to url map
            m_bk_map[bk.internalElement().attribute("href")].append(bk);
        }
    }

    /*
     * BookmarkManager::Private
     */

    class BookmarkManager::Private
    {
    public:
        Private(bool bDocIsloaded, const QString &dbusObjectName = QString())
            : m_doc("xbel")
            , m_dbusObjectName(dbusObjectName)
            , m_docIsLoaded(bDocIsloaded)
            , m_update(false)
            , m_browserEditor(false)
            , m_typeExternal(false)
            , m_watcher(0)
        {}

        ~Private() {
            delete m_watcher;
        }

        mutable QDomDocument m_doc;
        mutable QDomDocument m_toolbarDoc;
        QString m_bookmarksFile;
        QString m_dbusObjectName;
        mutable bool m_docIsLoaded;
        bool m_update;

        bool m_browserEditor;
        QString m_editorCaption;

        bool m_typeExternal;
        QFileSystemWatcher *m_watcher;

        BookmarkMap m_map;
    };

    /*
     * BookmarkManager
     */

    static BookmarkManager* lookupExisting(const QString& bookmarksFile)
    {
        for (BookmarkManagerList::ConstIterator bmit = s_pSelf->constBegin(), bmend = s_pSelf->constEnd();
             bmit != bmend; ++bmit) {
            if ((*bmit)->path() == bookmarksFile)
                return *bmit;
        }

        return 0;
    }


    BookmarkManager* BookmarkManager::managerForFile(const QString& bookmarksFile, const QString& dbusObjectName)
    {
        BookmarkManager* mgr(0);
        {
            QReadLocker readLock(&s_pSelf->lock);
            mgr = lookupExisting(bookmarksFile);
            if (mgr)
                return mgr;
        }

        QWriteLocker writeLock(&s_pSelf->lock);
        mgr = lookupExisting(bookmarksFile);
        if (mgr)
            return mgr;

        mgr = new BookmarkManager(bookmarksFile, dbusObjectName);
        s_pSelf->append(mgr);
        return mgr;
    }

    BookmarkManager* BookmarkManager::managerForExternalFile(const QString& bookmarksFile)
    {
        BookmarkManager* mgr(0);
        {
            QReadLocker readLock(&s_pSelf->lock);
            mgr = lookupExisting(bookmarksFile);
            if (mgr)
                return mgr;
        }

        QWriteLocker writeLock(&s_pSelf->lock);
        mgr = lookupExisting(bookmarksFile);
        if (mgr)
            return mgr;

        mgr = new BookmarkManager(bookmarksFile);
        s_pSelf->append(mgr);
        return mgr;
    }

    BookmarkManager* BookmarkManager::createTempManager()
    {
        BookmarkManager* mgr = new BookmarkManager();
        s_pSelf->append(mgr);
        return mgr;
    }

#define PI_DATA "version=\"1.0\" encoding=\"UTF-8\""

    static QDomElement createXbelTopLevelElement(QDomDocument & doc)
    {
        QDomElement topLevel = doc.createElement("xbel");
        topLevel.setAttribute("xmlns:mime", "http://www.freedesktop.org/standards/shared-mime-info");
        topLevel.setAttribute("xmlns:bookmark", "http://www.freedesktop.org/standards/desktop-bookmarks");
        topLevel.setAttribute("xmlns:kdepriv", "http://www.kde.org/kdepriv");
        doc.appendChild(topLevel);
        doc.insertBefore(doc.createProcessingInstruction("xml", PI_DATA), topLevel);
        return topLevel;
    }

    BookmarkManager::BookmarkManager(const QString & bookmarksFile, const QString & dbusObjectName)
        : d(new Private(false, dbusObjectName))
    {
        if(dbusObjectName.isNull()) // get dbusObjectName from file
            if (QFile::exists(d->m_bookmarksFile))
                parse(); //sets d->m_dbusObjectName

        init("/BookmarkManager/"+d->m_dbusObjectName);

        d->m_update = true;

        Q_ASSERT(!bookmarksFile.isEmpty());
        d->m_bookmarksFile = bookmarksFile;

        if (!QFile::exists(d->m_bookmarksFile)) {
            QDomElement topLevel = createXbelTopLevelElement(d->m_doc);
            topLevel.setAttribute("dbusName", dbusObjectName);
            d->m_docIsLoaded = true;
        }
    }

    BookmarkManager::BookmarkManager(const QString & bookmarksFile)
        : d(new Private(false))
    {
        // use KDirWatch to monitor this bookmarks file
        d->m_typeExternal = true;
        d->m_update = true;

        Q_ASSERT(!bookmarksFile.isEmpty());
        d->m_bookmarksFile = bookmarksFile;

        if (!QFile::exists(d->m_bookmarksFile)) {
            createXbelTopLevelElement(d->m_doc);
        } else {
            parse();
        }
        d->m_docIsLoaded = true;

        // Start watching
        d->m_watcher = new QFileSystemWatcher();
        d->m_watcher->addPath(d->m_bookmarksFile);
        QObject::connect(d->m_watcher, SIGNAL(fileChanged(const QString&)),
                         this, SLOT(slotFileChanged(const QString&)));
        qDebug() << "Starting QFileSystemWatcher for " << d->m_bookmarksFile;
    }

    BookmarkManager::BookmarkManager()
        : d(new Private(true))
    {
        init("/BookmarkManager/generated");
        d->m_update = false; // TODO - make it read/write

        createXbelTopLevelElement(d->m_doc);
    }

    void BookmarkManager::init(const QString& dbusPath)
    {
#ifdef GIGI
        // A BookmarkManager without a dbus name is a temporary one, like those used by importers;
        // no need to register them to dbus
        if (dbusPath != "/BookmarkManager/" && dbusPath != "/BookmarkManager/generated") {
            new BookmarkManagerAdaptor(this);
            QDBusConnection::sessionBus().registerObject(dbusPath, this);

            QDBusConnection::sessionBus().connect(QString(), dbusPath, BOOKMARK_CHANGE_NOTIFY_INTERFACE,
                                                  "bookmarksChanged", this, SLOT(notifyChanged(QString,QDBusMessage)));
            QDBusConnection::sessionBus().connect(QString(), dbusPath, BOOKMARK_CHANGE_NOTIFY_INTERFACE,
                                                  "bookmarkConfigChanged", this, SLOT(notifyConfigChanged()));
        }
#endif
    }

    void BookmarkManager::slotFileChanged(const QString& path)
    {
        if (path == d->m_bookmarksFile) {
            qDebug() << "File changed " << path ;

            // Reparse
            parse();

            // Tell our GUI
            // (emit where group is "" to directly mark the root menu as dirty)
            emit changed("", QString());
        }
    }

    BookmarkManager::~BookmarkManager()
    {
        if (!s_pSelf.isDestroyed()) {
            s_pSelf->removeAll(this);
        }

        delete d;
    }

    void BookmarkManager::setUpdate(bool update)
    {
        d->m_update = update;
    }

    QDomDocument BookmarkManager::internalDocument() const
    {
        if(!d->m_docIsLoaded) {
            parse();
            d->m_toolbarDoc.clear();
        }
        return d->m_doc;
    }


    void BookmarkManager::parse() const
    {
        d->m_docIsLoaded = true;
        //qDebug() << "BookmarkManager::parse " << d->m_bookmarksFile;
        QFile file(d->m_bookmarksFile);
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Can't open " << d->m_bookmarksFile;
            return;
        }
        d->m_doc = QDomDocument("xbel");
        d->m_doc.setContent(&file);

        if (d->m_doc.documentElement().isNull()) {
            qWarning() << "BookmarkManager::parse : main tag is missing, creating default " << d->m_bookmarksFile;
            QDomElement element = d->m_doc.createElement("xbel");
            d->m_doc.appendChild(element);
        }

        QDomElement docElem = d->m_doc.documentElement();

        QString mainTag = docElem.tagName();
        if (mainTag != "xbel")
            qWarning() << "BookmarkManager::parse : unknown main tag " << mainTag;

        if(d->m_dbusObjectName.isNull()) {
            d->m_dbusObjectName = docElem.attribute("dbusName");
        } else if(docElem.attribute("dbusName") != d->m_dbusObjectName) {
            docElem.setAttribute("dbusName", d->m_dbusObjectName);
            save();
        }

        QDomNode n = d->m_doc.documentElement().previousSibling();
        if (n.isProcessingInstruction()) {
            QDomProcessingInstruction pi = n.toProcessingInstruction();
            pi.parentNode().removeChild(pi);
        }

        QDomProcessingInstruction pi;
        pi = d->m_doc.createProcessingInstruction("xml", PI_DATA);
        d->m_doc.insertBefore(pi, docElem);

        file.close();

        d->m_map.setNeedsUpdate();
    }

    bool BookmarkManager::save(bool toolbarCache) const
    {
        return saveAs(d->m_bookmarksFile, toolbarCache);
    }

    bool BookmarkManager::saveAs(const QString & filename, bool toolbarCache) const
    {
        qDebug() << "BookmarkManager::save " << filename;

        // Save the bookmark toolbar folder for quick loading
        // but only when it will actually make things quicker
        const QString cacheFilename = filename + QLatin1String(".tbcache");
        if(toolbarCache && !root().isToolbarGroup()) {
            QubeCore::SaveFile cacheFile(cacheFilename);
            if (cacheFile.open()) {
                QString str;
                QTextStream stream(&str, QIODevice::WriteOnly);
                stream << root().findToolbar();
                const QByteArray cstr = str.toUtf8();
                cacheFile.write(cstr.data(), cstr.length());
                cacheFile.finalize();
            }
        } else { // remove any (now) stale cache
            QFile::remove(cacheFilename);
        }

        QubeCore::SaveFile file(filename);
        if (file.open()) {
            file.simpleBackupFile(file.fileName(), QString(), ".bak");
            QTextStream stream(&file);
            stream.setCodec(QTextCodec::codecForName("UTF-8"));
            stream << internalDocument().toString();
            stream.flush();
            if (file.finalize()) {
                return true;
            }
        }

        static int hadSaveError = false;
        file.abort();
        if (!hadSaveError) {
            QString err = tr("Unable to save bookmarks in %1. Reported error was: %2. "
                             "This error message will only be shown once. The cause "
                             "of the error needs to be fixed as quickly as possible, "
                             "which is most likely a full hard drive.")
                          .arg(filename).arg(file.errorString());

            qCritical() << QString("Unable to save bookmarks in %1. File reported the following error-code: %2.").arg(filename).arg(file.error());
            emit const_cast<BookmarkManager*>(this)->error(err);
        }
        hadSaveError = true;
        return false;
    }

    QString BookmarkManager::path() const
    {
        return d->m_bookmarksFile;
    }

    BookmarkGroup BookmarkManager::root() const
    {
        return BookmarkGroup(internalDocument().documentElement());
    }

    BookmarkGroup BookmarkManager::toolbar()
    {
        qDebug() << "BookmarkManager::toolbar begin";
        // Only try to read from a toolbar cache if the full document isn't loaded
        if(!d->m_docIsLoaded) {
            qDebug() << "BookmarkManager::toolbar trying cache";
            const QString cacheFilename = d->m_bookmarksFile + QLatin1String(".tbcache");
            QFileInfo bmInfo(d->m_bookmarksFile);
            QFileInfo cacheInfo(cacheFilename);
            if (d->m_toolbarDoc.isNull() &&
                QFile::exists(cacheFilename) &&
                bmInfo.lastModified() < cacheInfo.lastModified()) {
                qDebug() << "BookmarkManager::toolbar reading file";
                QFile file(cacheFilename);

                if (file.open(QIODevice::ReadOnly)) {
                    d->m_toolbarDoc = QDomDocument("cache");
                    d->m_toolbarDoc.setContent(&file);
                    qDebug() << "BookmarkManager::toolbar opened";
                }
            }
            if (!d->m_toolbarDoc.isNull()) {
                qDebug() << "BookmarkManager::toolbar returning element";
                QDomElement elem = d->m_toolbarDoc.firstChild().toElement();
                return BookmarkGroup(elem);
            }
        }

        // Fallback to the normal way if there is no cache or if the bookmark file
        // is already loaded
        QDomElement elem = root().findToolbar();
        if (elem.isNull())
            return root(); // Root is the bookmark toolbar if none has been set.
        else
            return BookmarkGroup(root().findToolbar());
    }

    Bookmark BookmarkManager::findByAddress(const QString & address)
    {
        //qDebug() << "BookmarkManager::findByAddress " << address;
        Bookmark result = root();
        // The address is something like /5/10/2+
        const QStringList addresses = address.split(QRegExp("[/+]"),QString::SkipEmptyParts);
        // qWarning() << addresses.join(",");
        for (QStringList::const_iterator it = addresses.begin() ; it != addresses.end() ;) {
            bool append = ((*it) == "+");
            uint number = (*it).toUInt();
            Q_ASSERT(result.isGroup());
            BookmarkGroup group = result.toGroup();
            Bookmark bk = group.first(), lbk = bk; // last non-null bookmark
            for (uint i = 0 ; ((i<number) || append) && !bk.isNull() ; ++i) {
                lbk = bk;
                bk = group.next(bk);
                //qWarning() << i;
            }
            it++;
            //qWarning() << "found section";
            result = bk;
        }
        if (result.isNull()) {
            qWarning() << "BookmarkManager::findByAddress: couldn't find item " << address;
        }
        //qWarning() << "found " << result.address();
        return result;
    }

    void BookmarkManager::emitChanged()
    {
        emitChanged(root());
    }


    void BookmarkManager::emitChanged(const BookmarkGroup & group)
    {
        (void) save(); // KDE5 TODO: emitChanged should return a bool? Maybe rename it to saveAndEmitChanged?

        // Tell the other processes too
        // qDebug() << "BookmarkManager::emitChanged : broadcasting change " << group.address();

        emit bookmarksChanged(group.address());

        // We do get our own broadcast, so no need for this anymore
        //emit changed(group);
    }

    void BookmarkManager::emitConfigChanged()
    {
        emit bookmarkConfigChanged();
    }

    void BookmarkManager::notifyCompleteChange(const QString &caller) // DBUS call
    {
        if (!d->m_update)
            return;

        qDebug() << "BookmarkManager::notifyCompleteChange";

        // The bk editor tells us we should reload everything
        // Reparse
        parse();

        // Tell our GUI
        // (emit where group is "" to directly mark the root menu as dirty)
        emit changed("", caller);
    }

    void BookmarkManager::notifyConfigChanged() // DBUS call
    {
        qDebug() << "reloaded bookmark config!";
        parse(); // reload, and thusly recreate the menus
        emit configChanged();
    }

    void BookmarkManager::notifyChanged(const QString &groupAddress, const QDBusMessage &msg) // DBUS call
    {
        qDebug() << "BookmarkManager::notifyChanged ("<<groupAddress<<")";
        if (!d->m_update)
            return;

        // Reparse (the whole file, no other choice)
        // if someone else notified us
        if (msg.service() != QDBusConnection::sessionBus().baseService())
            parse();

        //qDebug() << "BookmarkManager::notifyChanged " << groupAddress;
        //BookmarkGroup group = findByAddress(groupAddress).toGroup();
        //Q_ASSERT(!group.isNull());
        emit changed(groupAddress, QString());
    }

///////
    bool BookmarkManager::updateAccessMetadata(const QString & url)
    {
        d->m_map.update(this);
        QList<Bookmark> list = d->m_map.find(url);
        if (list.count() == 0)
            return false;

        for (QList<Bookmark>::iterator it = list.begin();
             it != list.end(); ++it)
            (*it).updateAccessMetadata();

        return true;
    }

    void BookmarkManager::updateFavicon(const QString &url, const QString &/*faviconurl*/)
    {
        d->m_map.update(this);
        QList<Bookmark> list = d->m_map.find(url);
        for (QList<Bookmark>::iterator it = list.begin();
             it != list.end(); ++it) {
            // TODO - update favicon data based on faviconurl
            //        but only when the previously used icon
            //        isn't a manually set one.
        }
    }

    BookmarkManager* BookmarkManager::userBookmarksManager()
    {
#ifdef GIGI
        const QString bookmarksFile = KStandardDirs::locateLocal("data", QString::fromLatin1("konqueror/bookmarks.xml"));
#else
        const QString bookmarksFile("~/.local/share/colombo/bookmarks.xml");
#endif
        BookmarkManager* bookmarkManager = BookmarkManager::managerForFile(bookmarksFile, "colombo");
        return bookmarkManager;
    }
}
