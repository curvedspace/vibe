/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2007 Kevin Ottens
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Kevin Ottens <ervin@kde.org>
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
 * and devices, it's used by Swordfish and open/save dialogs.
 *
 * It's reccomended to use this view in all application that needs access
 * to system bookmarks such as Home, Desktop, Pictures, ... and devices and
 * need integration to Swordfish's bookmarks.
 *
 * \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VIBEWIDGETS_EXPORT VFilePlacesView : public QTreeView
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
