/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2000-2006 David Faure
 * Copyright (c) 2003 Alexander Kellett
 * Copyright (c) 2008 Norbert Frese
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *    David Faure <faure@kde.org>
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Norbert Frese <nf2@scheinwelt.at>
 *    Alexander Kellett <lypanov@kde.org>
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

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QProcess>
#include <QtCore/QRegExp>
#include <QtCore/QTextStream>
#include <QtCore/QFileSystemWatcher>
#include <QtDBus/QtDBus>
#include <QtWidgets/QApplication>
#include <QStandardPaths>

#include <VibeCore/VSaveFile>

#include "vbookmarkmanager.h"
#include "vbookmarkmanageradaptor.h"

const QString kToolBarCacheExtension = QLatin1String(".toolbarcache");

/*
 * VBookmarkManagerList
 */

class VBookmarkManagerList : public QList<VBookmarkManager *>
{
public:
    ~VBookmarkManagerList() {
        qDeleteAll(begin() , end()); // auto-delete functionality
    }

    QReadWriteLock lock;
};

Q_GLOBAL_STATIC(VBookmarkManagerList, s_pSelf)

/*
 * VBookmarkMap
 */

class VBookmarkMap : private VBookmarkGroupTraverser
{
public:
    VBookmarkMap() : m_mapNeedsUpdate(true) {}
    void setNeedsUpdate() {
        m_mapNeedsUpdate = true;
    }
    void update(VBookmarkManager *);
    QList<VBookmark> find(const QString &url) const {
        return m_bk_map.value(url);
    }

private:
    virtual void visit(const VBookmark &);
    virtual void visitEnter(const VBookmarkGroup &) {
        ;
    }
    virtual void visitLeave(const VBookmarkGroup &) {
        ;
    }

    typedef QList<VBookmark> VBookmarkList;
    QMap<QString, VBookmarkList> m_bk_map;
    bool m_mapNeedsUpdate;
};

void VBookmarkMap::update(VBookmarkManager *manager)
{
    if (m_mapNeedsUpdate) {
        m_mapNeedsUpdate = false;

        m_bk_map.clear();
        VBookmarkGroup root = manager->root();
        traverse(root);
    }
}

void VBookmarkMap::visit(const VBookmark &bk)
{
    if (!bk.isSeparator()) {
        // add bookmark to url map
        m_bk_map[bk.internalElement().attribute("href")].append(bk);
    }
}

/*
 * VBookmarkManager::Private
 */

class VBookmarkManager::Private
{
public:
    Private(bool bDocIsloaded, const QString &dbusObjectName = QString()) :
        m_doc("xbel"),
        m_dbusObjectName(dbusObjectName),
        m_docIsLoaded(bDocIsloaded),
        m_update(false),
        m_browserEditor(false),
        m_typeExternal(false),
        m_watcher(0)
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

    VBookmarkMap m_map;
};

/*
 * VBookmarkManager
 */

static VBookmarkManager *lookupExisting(const QString &bookmarksFile)
{
    for (VBookmarkManagerList::ConstIterator bmit = s_pSelf()->constBegin(), bmend = s_pSelf()->constEnd();
            bmit != bmend; ++bmit) {
        if ((*bmit)->path() == bookmarksFile)
            return *bmit;
    }

    return 0;
}

VBookmarkManager *VBookmarkManager::managerForFile(const QString &bookmarksFile, const QString &dbusObjectName)
{
    VBookmarkManager *mgr(0);
    {
        QReadLocker readLock(&s_pSelf()->lock);
        mgr = lookupExisting(bookmarksFile);
        if (mgr)
            return mgr;
    }

    QWriteLocker writeLock(&s_pSelf()->lock);
    mgr = lookupExisting(bookmarksFile);
    if (mgr)
        return mgr;

    mgr = new VBookmarkManager(bookmarksFile, dbusObjectName);
    s_pSelf()->append(mgr);
    return mgr;
}

VBookmarkManager *VBookmarkManager::managerForExternalFile(const QString &bookmarksFile)
{
    VBookmarkManager *mgr(0);
    {
        QReadLocker readLock(&s_pSelf()->lock);
        mgr = lookupExisting(bookmarksFile);
        if (mgr)
            return mgr;
    }

    QWriteLocker writeLock(&s_pSelf()->lock);
    mgr = lookupExisting(bookmarksFile);
    if (mgr)
        return mgr;

    mgr = new VBookmarkManager(bookmarksFile);
    s_pSelf()->append(mgr);
    return mgr;
}

VBookmarkManager *VBookmarkManager::createTempManager()
{
    VBookmarkManager *mgr = new VBookmarkManager();
    s_pSelf()->append(mgr);
    return mgr;
}

#define PI_DATA "version=\"1.0\" encoding=\"UTF-8\""

static QDomElement createXbelTopLevelElement(QDomDocument &doc)
{
    QDomElement topLevel = doc.createElement("xbel");
    topLevel.setAttribute("xmlns:mime", "http://www.freedesktop.org/standards/shared-mime-info");
    topLevel.setAttribute("xmlns:bookmark", "http://www.freedesktop.org/standards/desktop-bookmarks");
    topLevel.setAttribute("xmlns:kdepriv", "http://www.kde.org/kdepriv");
    doc.appendChild(topLevel);
    doc.insertBefore(doc.createProcessingInstruction("xml", PI_DATA), topLevel);
    return topLevel;
}

VBookmarkManager::VBookmarkManager(const QString &bookmarksFile, const QString &dbusObjectName)
    : d(new Private(false, dbusObjectName))
{
    if (dbusObjectName.isNull()) // get dbusObjectName from file
        if (QFile::exists(d->m_bookmarksFile))
            parse(); //sets d->m_dbusObjectName

    init("/BookmarkManager/" + d->m_dbusObjectName);

    d->m_update = true;

    Q_ASSERT(!bookmarksFile.isEmpty());
    d->m_bookmarksFile = bookmarksFile;

    if (!QFile::exists(d->m_bookmarksFile)) {
        QDomElement topLevel = createXbelTopLevelElement(d->m_doc);
        topLevel.setAttribute("dbusName", dbusObjectName);
        d->m_docIsLoaded = true;
    }
}

VBookmarkManager::VBookmarkManager(const QString &bookmarksFile)
    : d(new Private(false))
{
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
    d->m_watcher = new QFileSystemWatcher(this);
    d->m_watcher->addPath(d->m_bookmarksFile);
    QObject::connect(d->m_watcher, SIGNAL(fileChanged(const QString &)),
                     this, SLOT(slotFileChanged(const QString &)));
    qDebug() << "Starting QFileSystemWatcher for " << d->m_bookmarksFile;
}

VBookmarkManager::VBookmarkManager()
    : d(new Private(true))
{
    init("/BookmarkManager/generated");
    d->m_update = false; // TODO - make it read/write

    createXbelTopLevelElement(d->m_doc);
}

void VBookmarkManager::init(const QString &dbusPath)
{
    // A VBookmarkManager without a DBus name is a temporary one, like those used by importers;
    // no need to register them to DBus
    if (dbusPath != "/BookmarkManager/" && dbusPath != "/BookmarkManager/generated") {
        new VBookmarkManagerAdaptor(this);
        QDBusConnection::sessionBus().registerObject(dbusPath, this);

        QDBusConnection::sessionBus().connect(QString(), dbusPath, VIBE_BOOKMARKMANAGER_DBUS_INTERFACE,
                                              "bookmarksChanged", this, SLOT(notifyChanged(QString, QDBusMessage)));
        QDBusConnection::sessionBus().connect(QString(), dbusPath, VIBE_BOOKMARKMANAGER_DBUS_INTERFACE,
                                              "bookmarkConfigChanged", this, SLOT(notifyConfigChanged()));
    }
}

void VBookmarkManager::slotFileChanged(const QString &path)
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

VBookmarkManager::~VBookmarkManager()
{
    s_pSelf()->removeAll(this);

    delete d;
}

void VBookmarkManager::setUpdate(bool update)
{
    d->m_update = update;
}

QDomDocument VBookmarkManager::internalDocument() const
{
    if (!d->m_docIsLoaded) {
        parse();
        d->m_toolbarDoc.clear();
    }
    return d->m_doc;
}


void VBookmarkManager::parse() const
{
    d->m_docIsLoaded = true;
    //qDebug() << "VBookmarkManager::parse " << d->m_bookmarksFile;
    QFile file(d->m_bookmarksFile);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Can't open " << d->m_bookmarksFile;
        return;
    }
    d->m_doc = QDomDocument("xbel");
    d->m_doc.setContent(&file);

    if (d->m_doc.documentElement().isNull()) {
        qWarning() << "VBookmarkManager::parse : main tag is missing, creating default " << d->m_bookmarksFile;
        QDomElement element = d->m_doc.createElement("xbel");
        d->m_doc.appendChild(element);
    }

    QDomElement docElem = d->m_doc.documentElement();

    QString mainTag = docElem.tagName();
    if (mainTag != "xbel")
        qWarning() << "VBookmarkManager::parse : unknown main tag " << mainTag;

    if (d->m_dbusObjectName.isNull()) {
        d->m_dbusObjectName = docElem.attribute("dbusName");
    } else if (docElem.attribute("dbusName") != d->m_dbusObjectName) {
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

bool VBookmarkManager::save(bool toolbarCache) const
{
    return saveAs(d->m_bookmarksFile, toolbarCache);
}

bool VBookmarkManager::saveAs(const QString &filename, bool toolbarCache) const
{
    qDebug() << "VBookmarkManager::save " << filename;

    // Create the directory if missing
    QFileInfo fileInfo(filename);
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists())
        dir.mkpath(dir.path());

    // Save the bookmark toolbar folder for quick loading
    // but only when it will actually make things quicker
    const QString cacheFilename = filename + kToolBarCacheExtension;
    if (toolbarCache && !root().isToolbarGroup()) {
        VSaveFile cacheFile(cacheFilename);
        if (cacheFile.open()) {
            QString str;
            QTextStream stream(&str, QIODevice::WriteOnly);
            stream << root().findToolbar();
            const QByteArray cstr = str.toUtf8();
            cacheFile.write(cstr.data(), cstr.length());
            cacheFile.finalize();
        }
    } else {
        // Remove any (now) stale cache
        if (QFile::exists(cacheFilename))
            QFile::remove(cacheFilename);
    }

    VSaveFile file(filename);
    if (file.open()) {
        file.simpleBackupFile(file.fileName(), QString(), ".bak");
        QTextStream stream(&file);
        stream.setCodec(QTextCodec::codecForName("UTF-8"));
        stream << internalDocument().toString();
        stream.flush();
        if (file.finalize())
            return true;
    }

    static int hadSaveError = false;
    file.abort();
    if (!hadSaveError) {
        QString err = tr("Unable to save bookmarks in %1. Reported error was: %2. "
                         "This error message will only be shown once. The cause "
                         "of the error needs to be fixed as quickly as possible, "
                         "which is most likely a full hard drive.")
                      .arg(filename).arg(file.errorString());

        qCritical() << qPrintable(QString("Unable to save bookmarks in %1 for the following reason (error code %2): %3")
                    .arg(filename).arg(file.error()).arg(file.errorString()));
        emit const_cast<VBookmarkManager *>(this)->error(err);
    }
    hadSaveError = true;
    return false;
}

QString VBookmarkManager::path() const
{
    return d->m_bookmarksFile;
}

VBookmarkGroup VBookmarkManager::root() const
{
    return VBookmarkGroup(internalDocument().documentElement());
}

VBookmarkGroup VBookmarkManager::toolbar()
{
    qDebug() << "VBookmarkManager::toolbar begin";

    // Only try to read from a toolbar cache if the full document isn't loaded
    if (!d->m_docIsLoaded) {
        qDebug() << "VBookmarkManager::toolbar trying cache";
        const QString cacheFilename = d->m_bookmarksFile + kToolBarCacheExtension;
        QFileInfo bmInfo(d->m_bookmarksFile);
        QFileInfo cacheInfo(cacheFilename);
        if (d->m_toolbarDoc.isNull() &&
                QFile::exists(cacheFilename) &&
                bmInfo.lastModified() < cacheInfo.lastModified()) {
            qDebug() << "VBookmarkManager::toolbar reading file";
            QFile file(cacheFilename);

            if (file.open(QIODevice::ReadOnly)) {
                d->m_toolbarDoc = QDomDocument("cache");
                d->m_toolbarDoc.setContent(&file);
                qDebug() << "VBookmarkManager::toolbar opened";
            }
        }
        if (!d->m_toolbarDoc.isNull()) {
            qDebug() << "VBookmarkManager::toolbar returning element";
            QDomElement elem = d->m_toolbarDoc.firstChild().toElement();
            return VBookmarkGroup(elem);
        }
    }

    // Fallback to the normal way if there is no cache or if the bookmark file
    // is already loaded
    QDomElement elem = root().findToolbar();
    if (elem.isNull())
        return root(); // Root is the bookmark toolbar if none has been set.
    else
        return VBookmarkGroup(root().findToolbar());
}

VBookmark VBookmarkManager::findByAddress(const QString &address)
{
    //qDebug() << "VBookmarkManager::findByAddress " << address;
    VBookmark result = root();
    // The address is something like /5/10/2+
    const QStringList addresses = address.split(QRegExp("[/+]"), QString::SkipEmptyParts);
    // qWarning() << addresses.join(",");
    for (QStringList::const_iterator it = addresses.begin() ; it != addresses.end() ;) {
        bool append = ((*it) == "+");
        uint number = (*it).toUInt();
        Q_ASSERT(result.isGroup());
        VBookmarkGroup group = result.toGroup();
        VBookmark bk = group.first(), lbk = bk; // last non-null bookmark
        for (uint i = 0 ; ((i < number) || append) && !bk.isNull() ; ++i) {
            lbk = bk;
            bk = group.next(bk);
            //qWarning() << i;
        }
        it++;
        //qWarning() << "found section";
        result = bk;
    }
    if (result.isNull()) {
        qWarning() << "VBookmarkManager::findByAddress: couldn't find item " << address;
    }
    //qWarning() << "found " << result.address();
    return result;
}

void VBookmarkManager::emitChanged()
{
    emitChanged(root());
}


void VBookmarkManager::emitChanged(const VBookmarkGroup &group)
{
    (void) save(); // KDE5 TODO: emitChanged should return a bool? Maybe rename it to saveAndEmitChanged?

    // Tell the other processes too
    // qDebug() << "VBookmarkManager::emitChanged : broadcasting change " << group.address();

    emit bookmarksChanged(group.address());

    // We do get our own broadcast, so no need for this anymore
    //emit changed(group);
}

void VBookmarkManager::emitConfigChanged()
{
    emit bookmarkConfigChanged();
}

void VBookmarkManager::notifyCompleteChange(const QString &caller) // DBUS call
{
    if (!d->m_update)
        return;

    qDebug() << "VBookmarkManager::notifyCompleteChange";

    // The bk editor tells us we should reload everything
    // Reparse
    parse();

    // Tell our GUI
    // (emit where group is "" to directly mark the root menu as dirty)
    emit changed("", caller);
}

void VBookmarkManager::notifyConfigChanged() // DBUS call
{
    qDebug() << "reloaded bookmark config!";
    parse(); // reload, and thusly recreate the menus
    emit configChanged();
}

void VBookmarkManager::notifyChanged(const QString &groupAddress, const QDBusMessage &msg) // DBUS call
{
    qDebug() << "VBookmarkManager::notifyChanged (" << groupAddress << ")";
    if (!d->m_update)
        return;

    // Reparse (the whole file, no other choice)
    // if someone else notified us
    if (msg.service() != QDBusConnection::sessionBus().baseService())
        parse();

    //qDebug() << "VBookmarkManager::notifyChanged " << groupAddress;
    //VBookmarkGroup group = findByAddress(groupAddress).toGroup();
    //Q_ASSERT(!group.isNull());
    emit changed(groupAddress, QString());
}

///////
bool VBookmarkManager::updateAccessMetadata(const QString &url)
{
    d->m_map.update(this);
    QList<VBookmark> list = d->m_map.find(url);
    if (list.count() == 0)
        return false;

    for (QList<VBookmark>::iterator it = list.begin();
            it != list.end(); ++it)
        (*it).updateAccessMetadata();

    return true;
}

void VBookmarkManager::updateFavicon(const QString &url, const QString &/*faviconurl*/)
{
    d->m_map.update(this);
    QList<VBookmark> list = d->m_map.find(url);
    for (QList<VBookmark>::iterator it = list.begin();
            it != list.end(); ++it) {
        // TODO - update favicon data based on faviconurl
        //        but only when the previously used icon
        //        isn't a manually set one.
    }
}

VBookmarkManager *VBookmarkManager::userBookmarksManager()
{
    QString bookmarksFile = QStandardPaths::locate(
                QStandardPaths::GenericDataLocation, "colombo/bookmarks.xbel");
    VBookmarkManager *bookmarkManager = VBookmarkManager::managerForFile(bookmarksFile, "colombo");
    return bookmarkManager;
}

#include "moc_vbookmarkmanager.cpp"
