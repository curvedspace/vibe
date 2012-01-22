/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 * Copyright (C) 2006 Tobias Koenig
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *	Tobias Koenig <tokoe@kde.org>
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

#ifndef VPAGEVIEW_H
#define VPAGEVIEW_H

#include <QtGui/QWidget>

#include <VibeCore/VGlobal>

class QAbstractItemDelegate;
class QAbstractItemView;
class QModelIndex;
class QAbstractItemModel;

class VPageModel;
class VPageViewPrivate;

/** \addtogroup gui Gui Kit
 *  @{
 */

/**
 * \class VPageView vpageview.h <VPageView>
 *
 * @short A base class which can handle multiple pages.
 *
 * This class provides a widget base class which handles multiple
 * pages and allows the user to switch between these pages in
 * different ways.
 *
 * Currently, @p Auto, @p Plain, @p List, @p Tree and @p Tabbed face
 * types are available. @see VPageWidget
 *
 * <b>Example:</b>\n
 *
 * \code
 *  VPageModel *model = new MyPageModel();
 *
 *  VPageView *view = new VPageView(this);
 *  view->setModel( model );
 *
 *  view->setFaceType(VPageView::List);
 * \endcode
 *
 * @author Tobias Koenig <tokoe@kde.org>
 */
class VIBE_EXPORT VPageView : public QWidget
{
    Q_OBJECT
    Q_ENUMS(FaceType)
    Q_PROPERTY(FaceType faceType READ faceType WRITE setFaceType)
    Q_DECLARE_PRIVATE(VPageView)
public:
    /**
     * This enum is used to decide which type of navigation view
     * shall be used in the page view.
     *
     * @li Auto   - Depending on the number of pages in the model,
     *              the Plain (one page), the List (several pages)
     *              or the Tree face (nested pages) will be used.
     *              This is the default face type.
     * @li Plain  - No navigation view will be visible and only the
     *              first page of the model will be shown.
     *
     * @li List   - An icon list is used as navigation view.
     *
     * @li Tree   - A tree list is used as navigation view.
     *
     * @li Tabbed - A tab widget is used as navigation view.
     */
    enum FaceType {
        Auto,
        Plain,
        List,
        Tree,
        Tabbed
    };

    /**
     * Creates a page view with given parent.
     */
    explicit VPageView(QWidget *parent = 0);

    /**
     * Destroys the page view.
     */
    virtual ~VPageView();

    /**
     * Sets the @p model of the page view.
     *
     * The model has to provide data for the roles defined in VPageModel::Role.
     */
    void setModel(QAbstractItemModel *model);

    /**
     * Returns the model of the page view.
     */
    QAbstractItemModel *model() const;

    /**
     * Sets the face type of the page view.
     */
    void setFaceType(FaceType faceType);

    /**
     * Returns the face type of the page view.
     */
    FaceType faceType() const;

    /**
     * Sets the page with @param index to be the current page and emits
     * the @see currentPageChanged signal.
     */
    void setCurrentPage(const QModelIndex &index);

    /**
     * Returns the index for the current page or an invalid index
     * if no current page exists.
     */
    QModelIndex currentPage() const;

    /**
     * Sets the item @param delegate which can be used customize
     * the page view.
     */
    void setItemDelegate(QAbstractItemDelegate *delegate);

    /**
     * Returns the item delegate of the page view.
     */
    QAbstractItemDelegate *itemDelegate() const;

    /**
     * Sets the @p widget which will be shown when a page is selected
     * that has no own widget set.
     */
    void setDefaultWidget(QWidget *widget);

signals:
    /**
     * This signal is emitted whenever the current page changes.
     * The previous page index is replaced by the current index.
     */
    void currentPageChanged(const QModelIndex &current, const QModelIndex &previous);

protected:
    /**
     * Returns the navigation view, depending on the current
     * face type.
     *
     * This method can be reimplemented to provide custom
     * navigation views.
     */
    virtual QAbstractItemView *createView();

    /**
     * Returns whether the page header should be visible.
     *
     * This method can be reimplemented for adapting custom
     * views.
     */
    virtual bool showPageHeader() const;

    /**
     * Returns the position where the navigation view should be
     * located according to the page stack.
     *
     * This method can be reimplemented for adapting custom
     * views.
     */
    virtual Qt::Alignment viewPosition() const;

    VPageView(VPageViewPrivate &dd, QWidget *parent);
    VPageViewPrivate *const d_ptr;

private:
    Q_PRIVATE_SLOT(d_func(), void _q_rebuildGui())
    Q_PRIVATE_SLOT(d_func(), void _q_modelChanged())
    Q_PRIVATE_SLOT(d_func(), void _q_pageSelected(const QItemSelection &, const QItemSelection &))
    Q_PRIVATE_SLOT(d_func(), void _q_dataChanged(const QModelIndex &, const QModelIndex &))
};

/** @}*/

#endif // VPAGEVIEW_H
