/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 * Copyright (C) 2006 Tobias Koenig
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Tobias Koenig <tokoe@kde.org>
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

#ifndef VPAGEWIDGET_H
#define VPAGEWIDGET_H

#include <VibeWidgets/VPageWidgetModel>
#include <VibeWidgets/VPageView>

class VPageWidgetPrivate;

/** \addtogroup gui Gui Kit
 *  @{
 */

/**
 * \class VPageWidget vpagewidget.h <VPageWidget>
 *
 * @short Page widget with many layouts (faces).
 * @see VPageView with hierarchical page model.
 *
 * @author Tobias Koenig <tokoe@kde.org>
 * @author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VIBE_EXPORT VPageWidget : public VPageView
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(VPageWidget)
public:
    /**
     * Creates a new page widget.
     *
     * @param parent The parent widget.
     */
    explicit VPageWidget(QWidget *parent = 0);

    /**
     * Destroys the page widget.
     */
    ~VPageWidget();

    /**
     * Adds a new top level page to the widget.
     *
     * @param widget The widget of the page.
     * @param name The name which is displayed in the navigation view.
     *
     * @returns The associated @see VPageWidgetItem.
     */
    VPageWidgetItem *addPage(QWidget *widget, const QString &name);

    /**
     * Adds a new top level page to the widget.
     *
     * @param item The @see VPageWidgetItem which describes the page.
     */
    void addPage(VPageWidgetItem *item);

    /**
     * Inserts a new page in the widget.
     *
     * @param before The new page will be insert before this @see VPageWidgetItem
     *               on the same level in hierarchy.
     * @param widget The widget of the page.
     * @param name The name which is displayed in the navigation view.
     *
     * @returns The associated @see VPageWidgetItem.
     */
    VPageWidgetItem *insertPage(VPageWidgetItem *before, QWidget *widget, const QString &name);

    /**
     * Inserts a new page in the widget.
     *
     * @param before The new page will be insert before this @see VPageWidgetItem
     *               on the same level in hierarchy.
     *
     * @param item The @see VPageWidgetItem which describes the page.
     */
    void insertPage(VPageWidgetItem *before, VPageWidgetItem *item);

    /**
     * Inserts a new sub page in the widget.
     *
     * @param parent The new page will be insert as child of this @see VPageWidgetItem.
     * @param widget The widget of the page.
     * @param name The name which is displayed in the navigation view.
     *
     * @returns The associated @see VPageWidgetItem.
     */
    VPageWidgetItem *addSubPage(VPageWidgetItem *parent, QWidget *widget, const QString &name);

    /**
     * Inserts a new sub page in the widget.
     *
     * @param parent The new page will be insert as child of this @see VPageWidgetItem.
     *
     * @param item The @see VPageWidgetItem which describes the page.
     */
    void addSubPage(VPageWidgetItem *parent, VPageWidgetItem *item);

    /**
     * Removes the page associated with the given @see VPageWidgetItem.
     */
    void removePage(VPageWidgetItem *item);

    /**
     * Sets the page which is associated with the given @see VPageWidgetItem to
     * be the current page and emits the currentPageChanged() signal.
     */
    void setCurrentPage(VPageWidgetItem *item);

    /**
     * Returns the @see VPageWidgetItem for the current page or 0 if there is no
     * current page.
     */
    VPageWidgetItem *currentPage() const;

signals:
    /**
     * This signal is emitted whenever the current page has changed.
     *
     * @param item The new current page or 0 if no current page is available.
     */
    void currentPageChanged(VPageWidgetItem *current, VPageWidgetItem *before);

    /**
     * This signal is emitted whenever a checkable page changes its state. @param checked is true
     * when the @param page is checked, or false if the @param page is unchecked.
     */
    void pageToggled(VPageWidgetItem *page, bool checked);

    /**
     * This signal is emitted when a page is removed.
     * @param page The page which is removed
     * */
    void pageRemoved(VPageWidgetItem *page);

protected:
    VPageWidget(VPageWidgetPrivate &dd, QWidget *parent);

private:
    Q_PRIVATE_SLOT(d_func(), void _q_slotCurrentPageChanged(const QModelIndex &, const QModelIndex &))
};

/** @}*/

#endif // VPAGEWIDGET_H
