/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#include <QDebug>
#include <QContextMenuEvent>
#include <QKeyEvent>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QUrl>
#include <QPersistentModelIndex>
#include <QTimeLine>
#include <QTimer>
#include <QScrollBar>

#include <VibeHardware/VStorageAccess>

#include "vfileplacesview.h"
#include "vfileplacesview_p.h"
#include "vfileplacesmodel.h"
#include "private/fileplacesitem.h"

using namespace VPrivate;

/*
 * VFilePlacesView
 */

VFilePlacesViewPrivate::VFilePlacesViewPrivate(VFilePlacesView *parent) :
    q_ptr(parent),
    showAll(false),
    delegate(new FilePlacesViewDelegate(parent))
{
}

void VFilePlacesViewPrivate::setCurrentIndex(const QModelIndex &index)
{
    Q_Q(VFilePlacesView);

    VFilePlacesModel *placesModel = qobject_cast<VFilePlacesModel *>(q->model());
    if (!placesModel)
        return;

    QUrl url = placesModel->url(index);
    if (url.isValid()) {
        currentUrl = url;
        updateHiddenRows(placesModel->parent(index));
        emit q->urlChanged(url);

        if (showAll)
            q->setShowAll(false);
    } else
        q->setUrl(currentUrl);
}

void VFilePlacesViewPrivate::updateHiddenRows(const QModelIndex &parent)
{
    Q_Q(VFilePlacesView);

    VFilePlacesModel *placesModel = qobject_cast<VFilePlacesModel *>(q->model());
    if (!placesModel)
        return;

    int rowCount = placesModel->rowCount(parent);
    QModelIndex current = placesModel->closestItem(currentUrl);

    for (int i = 0; i < rowCount; ++i) {
        QModelIndex index = placesModel->index(i, 0, parent);
        if (index != current && placesModel->isHidden(index) && !showAll)
            q->setRowHidden(i, placesModel->parent(index), true);
        else
            q->setRowHidden(i, placesModel->parent(index), false);
    }
}

void VFilePlacesViewPrivate::_q_placeClicked(const QModelIndex &index)
{
    Q_Q(VFilePlacesView);

    VFilePlacesModel *placesModel = qobject_cast<VFilePlacesModel *>(q->model());
    if (!placesModel)
        return;

    lastClickedIndex = QPersistentModelIndex();

    if (placesModel->isSetupNeeded(index)) {
        QObject::connect(placesModel, SIGNAL(setupDone(QModelIndex, boo)),
                         q, SLOT(_q_storageSetupDone(QModelIndex, bool)));
        lastClickedIndex = index;
        placesModel->requestSetup(index);
        return;
    }

    setCurrentIndex(index);
}

void VFilePlacesViewPrivate::_q_storageSetupDone(const QModelIndex &index, bool success)
{
    Q_Q(VFilePlacesView);

    if (index != lastClickedIndex)
        return;

    VFilePlacesModel *placesModel = qobject_cast<VFilePlacesModel *>(q->model());
    if (!placesModel)
        return;

    QObject::disconnect(placesModel, SIGNAL(setupDone(QModelIndex, bool)),
                        q, SLOT(_q_storageSetupDone(QModelIndex, bool)));
    if (success)
        setCurrentIndex(lastClickedIndex);
    else
        q->setUrl(currentUrl);

    lastClickedIndex = QPersistentModelIndex();
}

/*
 * VFilePlacesView
 */

VFilePlacesView::VFilePlacesView(QWidget *parent) :
    QTreeView(parent),
    d_ptr(new VFilePlacesViewPrivate(this))
{
    // Set style matters
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    setMinimumWidth(125);
    setMaximumWidth(245);
    setFrameShape(QFrame::NoFrame);
    setContentsMargins(0, 0, 0, 0);
    setHeaderHidden(true);
    setWordWrap(true);
    setAnimated(true);
    setItemsExpandable(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setRootIsDecorated(false);
    setUniformRowHeights(false);
    setAlternatingRowColors(false);
    setAutoFillBackground(false);
    verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);

    // Set D&D up
    setDragEnabled(true);
    setAcceptDrops(true);
    setMouseTracking(true);
    setDropIndicatorShown(false);

    // Use the appropriate model and delegate
    VFilePlacesModel *model = new VFilePlacesModel(this);
    setModel(model);
    setItemDelegate(d_ptr->delegate);

    // Adjust colors
    QPalette palette = viewport()->palette();
    palette.setColor(viewport()->backgroundRole(), Qt::transparent);
    palette.setColor(viewport()->foregroundRole(), palette.color(QPalette::WindowText));
    viewport()->setPalette(palette);

    // Handle the click on items
    connect(this, SIGNAL(clicked(QModelIndex)),
            this, SLOT(_q_placeClicked(QModelIndex)));

    // Expand all items
    QTimer::singleShot(0, this, SLOT(expandAll()));
}

VFilePlacesView::~VFilePlacesView()
{
    delete d_ptr;
}

void VFilePlacesView::setUrl(const QUrl &url)
{
    Q_D(VFilePlacesView);

    VFilePlacesModel *placesModel = qobject_cast<VFilePlacesModel *>(model());
    if (!placesModel)
        return;

    QUrl oldUrl = d->currentUrl;

    QModelIndex index = placesModel->closestItem(url);
    QModelIndex current = selectionModel()->currentIndex();

    if (index.isValid()) {
        if (current != index && placesModel->isHidden(current) && !d->showAll) {
            FilePlacesViewDelegate *delegate =
                dynamic_cast<FilePlacesViewDelegate *>(itemDelegate());
            delegate->addDisappearingItem(current);

            if (d->itemDisappearTimeLine.state() != QTimeLine::Running) {
                delegate->setDisappearingItemProgress(0.0);
                d->itemDisappearTimeLine.start();
            }
        }

        if (current != index && placesModel->isHidden(index) && !d->showAll) {
            FilePlacesViewDelegate *delegate =
                dynamic_cast<FilePlacesViewDelegate *>(itemDelegate());
            delegate->addAppearingItem(current);

            if (d->itemAppearTimeLine.state() != QTimeLine::Running) {
                delegate->setAppearingItemProgress(0.0);
                d->itemAppearTimeLine.start();
            }

            setRowHidden(index.row(), placesModel->parent(index), false);
        }

        d->currentUrl = url;
        selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
    } else {
        d->currentUrl = QUrl();
        selectionModel()->clear();
    }

    if (!current.isValid())
        d->updateHiddenRows(placesModel->parent(index));
}

void VFilePlacesView::setShowAll(bool showAll)
{
    Q_D(VFilePlacesView);

    VFilePlacesModel *placesModel = qobject_cast<VFilePlacesModel *>(model());
    if (!placesModel)
        return;

    d->showAll = showAll;

    QModelIndex current = placesModel->closestItem(d->currentUrl);
    int rowCount = placesModel->rowCount(placesModel->parent(current));

    FilePlacesViewDelegate *delegate = dynamic_cast<FilePlacesViewDelegate *>(itemDelegate());

    if (showAll) {
        d->updateHiddenRows(placesModel->parent(current));

        for (int i = 0; i < rowCount; ++i) {
            QModelIndex index = placesModel->index(i, 0, placesModel->parent(current));
            if (index != current && placesModel->isHidden(index))
                delegate->addAppearingItem(index);
        }

        if (d->itemAppearTimeLine.state() != QTimeLine::Running) {
            delegate->setAppearingItemProgress(0.0);
            d->itemAppearTimeLine.start();
        }
    } else {
        for (int i = 0; i < rowCount; i++) {
            QModelIndex index = placesModel->index(i, 0, placesModel->parent(current));
            if (index != current && placesModel->isHidden(index))
                delegate->addDisappearingItem(index);
        }

        if (d->itemDisappearTimeLine.state() != QTimeLine::Running) {
            delegate->setDisappearingItemProgress(0.0);
            d->itemDisappearTimeLine.start();
        }
    }
}

void VFilePlacesView::contextMenuEvent(QContextMenuEvent *event)
{
    Q_D(VFilePlacesView);

    VFilePlacesModel *placesModel = qobject_cast<VFilePlacesModel *>(model());
    //FilePlacesViewDelegate *delegate = dynamic_cast<FilePlacesViewDelegate *>(itemDelegate());

    if (!placesModel)
        return;

    QModelIndex index = indexAt(event->pos());
    QString label = placesModel->text(index).replace('&', "&&");

    QMenu menu;

    QAction *emptyTrashAction = 0;
    QAction *addAction = 0;
    QAction *editAction = 0;
    QAction *ejectAction = 0;
    QAction *teardownAction = 0;
    QAction *hideAction = 0;
    QAction *mainSeparator = 0;

    if (index.isValid() && !placesModel->isTopLevel(index)) {
        if (!placesModel->isDevice(index)) {
            if (placesModel->url(index) == QUrl("trash:///")) {
                emptyTrashAction = menu.addAction(QIcon::fromTheme("trash-empty"),
                                                  tr("Empty Trash"));
                // TODO: enabled only if not empty
                menu.addSeparator();
            }

            addAction = menu.addAction(QIcon::fromTheme("document-new"),
                                       tr("Add Entry..."));

            mainSeparator = menu.addSeparator();

            editAction = menu.addAction(QIcon::fromTheme("document-properties"),
                                        tr("&Edit Entry '%1'").arg(label));
        } else {
            ejectAction = placesModel->ejectActionForIndex(index);
            if (ejectAction) {
                ejectAction->setParent(&menu);
                menu.addAction(ejectAction);
            }

            teardownAction = placesModel->teardownActionForIndex(index);
            if (teardownAction) {
                teardownAction->setParent(&menu);
                menu.addAction(teardownAction);
            }

            if (ejectAction || teardownAction)
                mainSeparator = menu.addSeparator();
        }

        if (!addAction)
            addAction = menu.addAction(QIcon::fromTheme("document-new"),
                                       tr("Add Entry..."));

        hideAction = menu.addAction(tr("&Hide Entry '%1'").arg(label));
        hideAction->setCheckable(true);
        hideAction->setCheckable(placesModel->isHidden(index));
    } else {
        addAction = menu.addAction(QIcon::fromTheme("document-new"),
                                   tr("Add Entry..."));
    }

    // Show all entries if there's at least one hidden
    QAction *showAllAction = 0;
    if (placesModel->hiddenCount(placesModel->parent(index)) > 0) {
        showAllAction = new QAction(tr("&Show All Entries"), &menu);
        showAllAction->setCheckable(true);
        showAllAction->setChecked(d->showAll);

        if (!mainSeparator)
            mainSeparator = menu.addSeparator();

        menu.insertAction(mainSeparator, showAllAction);
    }

    // Bookmark items can be removed
    // TODO: Not the system ones!
    QAction *removeAction = 0;
    if (index.isValid() && !placesModel->isDevice(index) && !placesModel->isTopLevel(index))
        removeAction = menu.addAction(QIcon::fromTheme("edit-delete"),
                                      tr("&Remove Entry '%1'").arg(label));

    // Add additional actions
    menu.addActions(actions());

    if (menu.isEmpty())
        return;

    // Execute the menu
    QAction *result = menu.exec(event->globalPos());

    if (emptyTrashAction && result == emptyTrashAction) {
        // Handle empty trash action
        const QString info = tr("Do you really want to empty the Trash?");
        const QString text = tr("All items will be deleted and it won't be possible to restore them.");
        QMessageBox msg(QMessageBox::Warning, "", info, QMessageBox::Yes | QMessageBox::No);
        msg.setDetailedText(text);

        if (msg.exec() == QMessageBox::Yes) {
            // TODO:
        }
    } else if (editAction && result == editAction) {
        // Handle edit action
        VBookmark bookmark = placesModel->bookmarkForIndex(index);
    } else if (removeAction && result == removeAction) {
        // Handle remove action
    } else if (hideAction && result == hideAction) {
        // Handle hide action
    } else if (showAllAction && result == showAllAction) {
        // Handle show all action
    } else if (teardownAction && result == teardownAction) {
        // Teardown volume
        placesModel->requestTeardown(index);
    } else if (ejectAction && result == ejectAction) {
        // Eject media
        placesModel->requestEject(index);
    } else if (addAction && result == addAction) {
        // Handle add entry action
    }

    index = placesModel->closestItem(d->currentUrl);
    selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
}

void VFilePlacesView::keyPressEvent(QKeyEvent *event)
{
    Q_D(VFilePlacesView);

    QTreeView::keyPressEvent(event);

    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        d->_q_placeClicked(currentIndex());
}

#include "moc_vfileplacesview.cpp"
