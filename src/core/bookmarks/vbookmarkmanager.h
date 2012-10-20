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

#ifndef VBOOKMARKMANAGER_H
#define VBOOKMARKMANAGER_H

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QObject>
#include <QtCore/QPair>
#include <QtCore/QUrl>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

#include <VibeCore/VBookmark>

class QDBusMessage;

class VBookmarkGroup;

/**
 * \class VBookmarkManager vbookmarkmanager.h <VBookmarkManager>
 *
 * \brief Bookmarks manager.
 *
 * This class implements the reading/writing of bookmarks in XML.
 * The bookmarks file is read and written using the XBEL standard
 * (http://pyxml.sourceforge.net/topics/xbel/)
 *
 * A sample file looks like this:
 * \code
 * <xbel>
 *   <bookmark href="http://www.google.com"><title>Google</title></bookmark>
 *   <folder folded="no">
 *     <title>Title of this folder</title>
 *     <bookmark icon="maui" href="http://www.maui-os.org"><title>Maui</title></bookmark>
 *     <folder toolbar="yes">
 *       <title>Personal Bookmarks</title>
 *       <bookmark href="http://qt.nokia.com"><title>Qt</title></bookmark>
 *       <separator/>
 *       <bookmark href="http://doc.qt.nokia.com"><title>Qt Documentation</title></bookmark>
 *     </folder>
 *   </folder>
 * </xbel>
 * \endcode
 *
 * \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VIBECORE_EXPORT VBookmarkManager : public QObject
{
    Q_OBJECT
public:
    /**
     * Destructor
     */
    ~VBookmarkManager();

    /**
     * Set the update flag. Defaults to true.
     * @param update if true then VBookmarkManager will listen to DBus update requests.
     */
    void setUpdate(bool update);

    /**
     * Save the bookmarks to the given XML file on disk.
     * @param filename full path to the desired bookmarks file location
     * @param toolbarCache if true save a cache of the toolbar folder, too
     * @return true if saving was successful
     */
    // TODO: Use an enum and not a bool
    bool saveAs(const QString &filename, bool toolbarCache = true) const;

    /**
     * Update access time stamps for a given url.
     * @param url the viewed url
     * @return true if any metadata was modified (bookmarks file is not saved automatically)
     */
    bool updateAccessMetadata(const QString &url);

    /*
     * NB. currently *unimplemented*
     *
     * Update favicon url for a given url.
     * @param url the viewed url
     * @param faviconurl the favicion url
     */
    void updateFavicon(const QString &url, const QString &faviconurl);

    /**
     * This will return the path that this manager is using to read
     * the bookmarks.
     * @internal
     * @return the path containing the bookmarks
     */
    QString path() const;

    /**
     * Returns the root bookmark.  It is used to iterate
     * through the bookmarks manually.  It is mostly used internally.
     *
     * @return the root (top-level) bookmark
     */
    VBookmarkGroup root() const;

    /**
     * Returns the root of the toolbar menu.
     * In the XML, this is the group with the attribute toolbar=yes
     *
     * @return the toolbar group
     */
    VBookmarkGroup toolbar();

    /**
     * @return the bookmark designated by @p address
     * @param address the address belonging to the bookmark you're looking for
     * @param tolerate when true tries to find the most tolerable bookmark position
     * @see VBookmark::address
     */
    VBookmark findByAddress(const QString &address);

    /**
     * Saves the bookmark file and notifies everyone.
     *
     **/
    void emitChanged();

    /**
     * Saves the bookmark file and notifies everyone.
     * @param group the parent of all changed bookmarks
     */
    void emitChanged(const VBookmarkGroup &group);

    /**
     * Save the bookmarks to an XML file on disk.
     * You should use emitChanged() instead of this function, it saves
     * and notifies everyone that the file has changed.
     * Only use this if you don't want the emitChanged signal.
     * @param toolbarCache iff true save a cache of the toolbar folder, too
     * @return true if saving was successful
     */
    // TODO: Use an enum and not a bool
    bool save(bool toolbarCache = true) const;

    void emitConfigChanged();

    /**
     * This static function will return an instance of the
     * VBookmarkManager, responsible for the given @p bookmarksFile.
     * If you do not instantiate this class either
     * natively or in a derived class, then it will return an object
     * with the default behaviors.  If you wish to use different
     * behaviors, you <em>must</em> derive your own class and
     * instantiate it before this method is ever called.
     *
     * @param bookmarksFile full path to the bookmarks file.
     *
     * @param dbusObjectName a unique name that represents this bookmark collection,
     * usually your application's name. For example, Colombo uses "colombo", open and
     * save dialogs and Tracker use "filemanager".
     * The final D-Bus object path is /BookmarkManager/dbusObjectName
     * An empty dbusObjectName disables the registration to DBus (used for temporary managers).
     */
    static VBookmarkManager *managerForFile(const QString &bookmarksFile,
                                            const QString &dbusObjectName);

    /**
     * Returns a VBookmarkManager, which will use KDirWatch for change detection
     * This is important when sharing bookmarks with other Desktops.
     * @param bookmarksFile full path to the bookmarks file
     */
    static VBookmarkManager *managerForExternalFile(const QString &bookmarksFile);

    /**
     * only used for VBookmarkBar
     */
    static VBookmarkManager *createTempManager();

    /**
     * Returns a pointer to the user's main bookmark collection.
     */
    static VBookmarkManager *userBookmarksManager();

    /**
     * @internal
     */
    QDomDocument internalDocument() const;

public slots:
    /**
     * Reparse the whole bookmarks file and notify about the change
     * Doesn't send signal over DBus to the other VBookmark managers
     * You probably want to use emitChanged()
     *
     */
    void notifyCompleteChange(const QString &caller);

    /**
     * Emit the changed signal for the group whose address is given
     * @see VBookmark::address()
     * Called by the process that saved the file after
     * a small change (new bookmark or new folder).
     * Does not send signal over DBUS to the other VBookmark Managers
     * You probably want to call emitChanged()
     */
    void notifyChanged(const QString &groupAddress, const QDBusMessage &msg);

    void notifyConfigChanged();

Q_SIGNALS:
    /**
     * Signal send over DBUS
     */
    void bookmarkCompleteChange(QString caller);

    /**
     * Signal send over DBUS
     */
    void bookmarksChanged(QString groupAddress);

    /**
     * Signal send over DBUS
     */
    void bookmarkConfigChanged();

    /**
     * Signals that the group (or any of its children) with the address
     * @p groupAddress (e.g. "/4/5")
     * has been modified by the caller @p caller.
     * connect to this
     */
    void changed(const QString &groupAddress, const QString &caller);

    /**
     * Signals that the config changed
     */
    void configChanged();

    /**
     * Emitted when an error occurs.
     * Contains the translated error message.
     */
    void error(const QString &errorMessage);

private:
    /**
     * Creates a bookmark manager with a path to the bookmarks.  By
     * default, it will use the KDE standard dirs to find and create the
     * correct location.  If you are using your own app-specific
     * bookmarks directory, you must instantiate this class with your
     * own path <em>before</em> VBookmarkManager::managerForFile() is ever
     * called.
     *
     * @param bookmarksFile full path to the bookmarks file,
     * Use ~/.kde/share/apps/konqueror/bookmarks.xml for the konqueror bookmarks
     *
     * @param dbusObjectName a unique name that represents this bookmark collection,
     * usually your kinstance (e.g. kapplication) name. This is "konqueror" for the
     * konqueror bookmarks, "kfile" for KFileDialog bookmarks, etc.
     * The final D-Bus object path is /BookmarkManager/dbusObjectName
     * An empty dbusObjectName disables the registration to dbus (used for temporary managers)
     */
    VBookmarkManager(const QString &bookmarksFile, const QString &dbusObjectName);

    /**
     * Creates a bookmark manager for an external file
     */
    VBookmarkManager(const QString &bookmarksFile);

    /**
     * Creates a temporary bookmark manager.
     */
    VBookmarkManager();

    // consts added to avoid a copy-and-paste of internalDocument
    void parse() const;

    /**
    * You need to pass a dbusObjectName as the second parameter
    * In kde 3 managerForFile had the parameters (const QString &, bool)
    * We want old calls which pass a bool as a second parameter to fail.
    * Unfortunately C++ can convert a bool to a QString, via QString(char(bool))
    * This private overloaded method prevents this, as it is a better match,
    * and thus old calls fail at compile time.
    */
    static VBookmarkManager *managerForFile(const QString &, int);
    void init(const QString &dbusPath);

    class Private;
    Private *const d;

    friend class VBookmarkGroup;

private Q_SLOTS:
    void slotFileChanged(const QString &path); // external bookmarks
};

/**
 * The VBookmarkMenu and VBookmarkBar classes gives the user
 * the ability to either edit bookmarks or add their own.  In the
 * first case, the app may want to open the bookmark in a special way.
 * In the second case, the app <em>must</em> supply the name and the
 * URL for the bookmark.
 *
 * This class gives the app this callback-like ability.
 *
 * If your app does not give the user the ability to add bookmarks and
 * you don't mind using the default bookmark editor to edit your
 * bookmarks, then you don't need to overload this class at all.
 * Rather, just use something like:
 *
 * <CODE>
 * bookmarks = new VBookmarkMenu(mgr, 0, menu, actioncollec)
 * </CODE>
 *
 * If you wish to use your own editor or allow the user to add
 * bookmarks, you must overload this class.
 */
class VIBECORE_EXPORT VBookmarkOwner
{
public:
    virtual ~VBookmarkOwner() {}

    /**
     * This function is called whenever the user wants to add the
     * current page to the bookmarks list.  The title will become the
     * "name" of the bookmark.  You must overload this function if you
     * wish to give your users the ability to add bookmarks.
     * The default returns an empty string.
     *
     * @return the title of the current page.
     */
    virtual QString currentTitle() const {
        return QString();
    }

    /**
     * This function is called whenever the user wants to add the
     * current page to the bookmarks list.  The URL will become the URL
     * of the bookmark.  You must overload this function if you wish to
     * give your users the ability to add bookmarks.
     * The default returns an empty string.
     *
     * @return the URL of the current page.
     */
    virtual QString currentUrl() const {
        return QString();
    }


    /**
     * This function returns whether the owner supports tabs.
     * The default returns @c false.
     */
    virtual bool supportsTabs() const {
        return false;
    }

    /**
     * Returns a list of title, URL pairs of the open tabs.
     * The default returns an empty list.
     */
    virtual QList<QPair<QString, QString> > currentVBookmarkList() const {
        return QList<QPair<QString, QString> >();
    }

private:
    class VBookmarkOwnerPrivate;
    VBookmarkOwnerPrivate *d;
};

#endif // VBOOKMARKMANAGER_H
