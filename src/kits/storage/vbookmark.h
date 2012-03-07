/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2000-2005 David Faure
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

#ifndef VBOOKMARK_H
#define VBOOKMARK_H

#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QUrl>
#include <QtCore/QMimeType>
#include <QtXml/QDomElement>

#include <VibeCore/VGlobal>

class QMimeData;
class QMimeType;

/** \addtogroup storage Storage Kit
 *  @{
 */

class VBookmarkManager;
class VBookmarkGroup;

/**
 * \class VBookmark vbookmark.h <VBookmark>
 *
 * \brief Bookmark item.
 *
 * \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VIBE_EXPORT VBookmark
{
    friend class VBookmarkGroup;

    Q_DECLARE_TR_FUNCTIONS(VBookmark)
public:
    enum MetaDataOverwriteMode {
        OverwriteMetaData,
        DontOverwriteMetaData
    };

    /**
     * Constructs a null bookmark, i.e. a bookmark for which isNull() returns true
     * If you want to create a new bookmark use VBookmarkGroup::addBookmark().
     *
     * \see VBookmarkGroup::addBookmark
     */
    VBookmark();

    /**
     * Creates the VBookmark wrapper for @param elem
     * Mostly for internal usage.
     */
    explicit VBookmark(const QDomElement &elem);

    /**
     * Creates a stand alone bookmark. This is fairly expensive since a new QDom Tree is build.
     */
    static VBookmark standaloneVBookmark(const QString &text, const QUrl &url, const QString &icon = QString());

    /**
     * Whether the bookmark is a group or a normal bookmark
     */
    bool isGroup() const;

    /**
     * Whether the bookmark is a separator
     */
    bool isSeparator() const;

    /**
     * @return true if this is a null bookmark. This will never
     * be the case for a real bookmark (in a menu), but it's used
     * for instance as the end condition for VBookmarkGroup::next()
     */
    bool isNull() const;

    /**
     * @return true if bookmark is contained by a QDomDocument,
     * if not it is most likely that it has become separated and
     * is thus invalid and/or has been deleted from the bookmarks.
     */
    bool hasParent() const;

    /**
     * Text shown for the bookmark
     * If bigger than 40, the text is shortened by
     * replacing middle characters with "..." (see KStringHandler::csqueeze)
     */
    QString text() const;

    /**
     * Text shown for the bookmark, not truncated.
     * You should not use this - this is mainly for keditbookmarks.
     */
    QString fullText() const;

    /**
     * Set the text shown for the bookmark.
     *
     * @param fullText the new bookmark title
     */
    void setFullText(const QString &fullText);

    /**
     * URL contained by the bookmark
     */
    QUrl url() const;

    /**
     * Set the URL of the bookmark
     *
     * @param url the new bookmark URL
     */
    void setUrl(const QUrl &url);

    /**
     * @return the pixmap file for this bookmark
     * (i.e. the name of the icon)
     */
    QString icon() const;

    /**
     * Set the icon name of the bookmark
     *
     * @param icon the new icon name for this bookmark
     */
    void setIcon(const QString &icon);

    /**
     * @return Description of the bookmark
     */
    QString description() const;

    /**
     * Set the description of the bookmark
     *
     * @param description
     */
    void setDescription(const QString &description);

    /**
     * @return MIME type of this item
     */
    QMimeType mimeType() const;

    /**
     * Set the MIME type of this item
     *
     * @param MIME type
     */
    void setMimeType(const QMimeType &mimeType);

    /**
     * @return if the bookmark should be shown in the toolbar
     * (used by the filtered toolbar).
     *
     */
    bool showInToolbar() const;

    /**
     * Set whether this bookmark is show in a filterd toolbar
     */
    void setShowInToolbar(bool show);


    /**
     * @return the group containing this bookmark
     */
    VBookmarkGroup parentGroup() const;

    /**
     * Convert this to a group - do this only if
     * isGroup() returns true.
     */
    VBookmarkGroup toGroup() const;

    /**
     * Return the "address" of this bookmark in the whole tree.
     * This is used when telling other processes about a change
     * in a given bookmark. The encoding of the address is "/4/2", for
     * instance, to designate the 2nd child inside the 4th child of the
     * root bookmark.
     */
    QString address() const;

    /**
     * Return the position in the parent, i.e. the last number in the address
     */
    int positionInParent() const;

    // Hard to decide. Good design would imply that each bookmark
    // knows about its manager, so that there can be several managers.
    // But if we say there is only one manager (i.e. set of bookmarks)
    // per application, then VBookmarkManager::self() is much easier.
    //VBookmarkManager * manager() const { return m_manager; }

    /**
     * @internal
     */
    QDomElement internalElement() const;

    /**
     * Updates the bookmarks access metadata
     * Call when a user accesses the bookmark
     */
    void updateAccessMetadata();

    // Utility functions (internal)

    /**
     * @internal
     * @return address of parent
     */
    static QString parentAddress(const QString &address);

    /**
     * @internal
     * @return position in parent (e.g. /4/5/2 -> 2)
     */
    static uint positionInParent(const QString &address);

    /**
     * @internal
     * @return address of previous sibling (e.g. /4/5/2 -> /4/5/1)
     * Returns QString() for a first child
     */
    static QString previousAddress(const QString &address);

    /**
     * @internal
     * @return address of next sibling (e.g. /4/5/2 -> /4/5/3)
     * This doesn't check whether it actually exists
     */
    static QString nextAddress(const QString &address);

    /**
     * @internal
     * @return the common parent of both addresses which
     * has the greatest depth
     */
    static QString commonParent(const QString &A, const QString &B);

    /**
     * @internal
     * @return the metadata container node for a certain matadata owner
     */
    QDomNode metaData(const QString &owner, bool create) const;

    /**
     * Get the value of a specific metadata item (owner = "http://www.kde.org").
     * @param key Name of the metadata item
     * @return Value of the metadata item. QString() is returned in case
     * the specified key does not exist.
     */
    QString metaDataItem(const QString &key) const;

    /**
     * Change the value of a specific metadata item, or create the given item
     * if it doesn't exist already (owner = "http://www.kde.org").
     * @param key Name of the metadata item to change
     * @param value Value to use for the specified metadata item
     * @param mode Whether to overwrite the item's value if it exists already or not.
     */
    void setMetaDataItem(const QString &key, const QString &value, MetaDataOverwriteMode mode = OverwriteMetaData);

    /**
     * Adds this bookmark into the given QMimeData.
     *
     * WARNING: do not call this method multiple times, use VBookmark::List::populateMimeData instead.
     *
     * @param mimeData the QMimeData instance used to drag or copy this bookmark
     */
    void populateMimeData(QMimeData *mimeData) const;

    /**
     * Comparison operator
     */
    bool operator==(const VBookmark &rhs) const;

protected:
    QDomElement element;

    // NOTE: you can't add new member variables here.
    // The VBookmark objecs are created on the fly, as wrappers
    // around internal QDomElements. Any additional information
    // has to be implemented as an attribute of the QDomElement.
};

/**
 * \class VBookmarkList vbookmark.h <VBookmark>
 *
 * \brief A list of bookmarks.
 *
 * A QList that contains bookmarks with a few convenience methods.
 *
 * \see VBookmark
 * \see QList
 *
 * \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VBookmarkList : public QList<VBookmark>
{
public:
    VBookmarkList();

    /**
     * Adds this list of bookmark into the given QMimeData.
     *
     * @param mimeData the QMimeData instance used to drag or copy this bookmark
     */
    void populateMimeData(QMimeData *mimeData) const;

    /**
     * Return true if @p mimeData contains bookmarks
     */
    static bool canDecode(const QMimeData *mimeData);

    /**
     * Return the list of mimeTypes that can be decoded by fromMimeData
     */
    static QStringList mimeDataTypes();

    /**
     * Extract a list of bookmarks from the contents of @p mimeData.
     * Decoding will fail if @p mimeData does not contain any bookmarks.
     * @param mimeData the mime data to extract from; cannot be 0
     * @param parentDocument pass an empty QDomDocument here, it will be used as
     * container for the bookmarks. You just need to make sure it stays alive longer
     * (or just as long) as the returned bookmarks.
     * @return the list of bookmarks
     */
    static VBookmarkList fromMimeData(const QMimeData *mimeData, QDomDocument &parentDocument);
};

/**
 * \class VBookmarkGroup vbookmark.h <VBookmark>
 *
 * \brief A group of bookmarks
 *
 * \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VIBE_EXPORT VBookmarkGroup : public VBookmark
{
public:
    /**
     * Create an invalid group. This is mostly for use in QValueList,
     * and other places where we need a null group.
     * Also used as a parent for a bookmark that doesn't have one
     * (e.g. Netscape bookmarks)
     */
    VBookmarkGroup();

    /**
     * Create a bookmark group as specified by the given element
     */
    VBookmarkGroup(const QDomElement &elem);

    /**
     * @return true if the bookmark folder is opened in the bookmark editor
     */
    bool isOpen() const;

    /**
     * Return the first child bookmark of this group
     */
    VBookmark first() const;
    /**
     * Return the prevous sibling of a child bookmark of this group
     * @param current has to be one of our child bookmarks.
     */
    VBookmark previous(const VBookmark &current) const;
    /**
     * Return the next sibling of a child bookmark of this group
     * @param current has to be one of our child bookmarks.
     */
    VBookmark next(const VBookmark &current) const;

    /**
     * Return the index of a child bookmark, -1 if not found
     */
    int indexOf(const VBookmark &child) const;

    /**
     * Create a new bookmark folder, as the last child of this group
     * @param text for the folder.
     */
    VBookmarkGroup createNewFolder(const QString &text);

    /**
     * Create a new bookmark separator
     * Don't forget to use VBookmarkManager::self()->emitChanged(parentVBookmark);
     */
    VBookmark createNewSeparator();

    /**
     * Create a new bookmark, as the last child of this group
     * Don't forget to use VBookmarkManager::self()->emitChanged(parentVBookmark);
     * @param bm the bookmark to add
     */
    VBookmark addBookmark(const VBookmark &bm);

    /**
     * Create a new bookmark, as the last child of this group
     * Don't forget to use VBookmarkManager::self()->emitChanged(parentVBookmark);
     * @param text for the bookmark
     * @param url the URL that the bookmark points to
     * @param icon the name of the icon to associate with the bookmark. A suitable default
     * will be determined from the URL if not specified.
     */
    VBookmark addBookmark(const QString &text, const QUrl &url, const QString &icon = QString());

    /**
     * Moves @p bookmark after @p after (which should be a child of ours).
     * If after is null, @p bookmark is moved as the first child.
     * Don't forget to use VBookmarkManager::self()->emitChanged(parentVBookmark);
     */
    bool moveBookmark(const VBookmark &bookmark, const VBookmark &after);

    /**
     * Delete a bookmark - it has to be one of our children !
     * Don't forget to use VBookmarkManager::self()->emitChanged(parentVBookmark);
     */
    void deleteBookmark(const VBookmark &bk);

    /**
     * @return true if this is the toolbar group
     */
    bool isToolbarGroup() const;

    /**
     * @internal
     */
    QDomElement findToolbar() const;

    /**
     * @return the list of urls of bookmarks at top level of the group
     */
    QList<QUrl> groupUrlList() const;

protected:
    QDomElement nextKnownTag(const QDomElement &start, bool goNext) const;

private:

    // NOTE: you can't add other member variables here, except for caching info.
    // The VBookmark objects are created on the fly, as wrappers
    // around internal QDomElements. Any additional information
    // has to be implemented as an attribute of the QDomElement.
};

/**
 * \class VBookmarkGroupTraverser vbookmark.h <VBookmark>
 *
 * \brief Bookmark group traverser.
 *
 * \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VIBE_EXPORT VBookmarkGroupTraverser
{
protected:
    virtual ~VBookmarkGroupTraverser();

    void traverse(const VBookmarkGroup &);
    virtual void visit(const VBookmark &);
    virtual void visitEnter(const VBookmarkGroup &);
    virtual void visitLeave(const VBookmarkGroup &);
};

Q_DECLARE_METATYPE(VBookmark)

#endif // VBOOKMARK_H
