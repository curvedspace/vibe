/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef VFILEPLACESVIEW_H
#define VFILEPLACESVIEW_H

#include <VibeWidgets/VNavigationBar>

class QUrl;

/** \addtogroup storage Storage Kit
 *  @{
 */

class VFilePlacesViewPrivate;

/**
 * \class VFilePlacesView vfileplacesview.h <VFilePlacesView>
 *
 * \brief A navigation bar specifically designed for file managers.
 *
 * This class provides a treeview based navigation bar with bookmarks
 * and devices, it's used by Tracker and open/save dialogs.
 *
 * It's reccomended to use this view in all application that needs access
 * to system bookmarks such as Home, Desktop, Pictures, ... and devices and
 * need integration to Tracker's bookmarks.
 *
 * \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VIBE_EXPORT VFilePlacesView : public QTreeView
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(VFilePlacesView)
public:
    /**
     * Creates a file places view.
     */
    explicit VFilePlacesView(QWidget *parent = 0);

    /**
     * Destroys a file places view.
     */
    ~VFilePlacesView();

signals:
    void urlChanged(const QUrl &url);
    void urlsDropped(const QUrl &url, QDropEvent *event, QWidget *parent);

public slots:
    void setUrl(const QUrl &url);
    void setShowAll(bool showAll);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

private:
    Q_PRIVATE_SLOT(d_ptr, void _q_placeClicked(const QModelIndex &))
    Q_PRIVATE_SLOT(d_ptr, void _q_storageSetupDone(const QModelIndex &, bool))

    VFilePlacesViewPrivate *const d_ptr;
};

/** @}*/

#endif // VFILEPLACESVIEW_H
