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
 * the Free Software Foundation, either version 3 of the License, or
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

#ifndef VPAGEWIDGETMODEL_H
#define VPAGEWIDGETMODEL_H

#include <VibeGui/VPageModel>

class QWidget;
class QIcon;

/** \addtogroup gui Gui Kit
 *  @{
 */

/**
 * \class VPageWidgetItem vpagewidgetmodel.h <VPageWidgetModel>
 *
 * VPageWidgetItem is used by @ref VPageWidget and represents
 * a page.
 *
 * <b>Example:</b>\n
 *
 * \code
 *  ColorPage *page = new ColorPage;
 *
 *  VPageWidgetItem *item = new VPageWidgetItem(page, tr("Colors"));
 *  item->setHeader(tr("Colors of Main Window"));
 *  item->setIcon(QIcon::fromTheme("colors"));
 *
 *  VPageWidget *pageWidget = new VPageWidget(this);
 *  pageWidget->addPage(item);
 * \endcode
 *
 * @see VPageWidget
 *
 * @author Tobias Koenig <tokoe@kde.org>
 * @author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VIBE_EXPORT VPageWidgetItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString header READ header WRITE setHeader)
    Q_PROPERTY(QIcon icon READ icon WRITE setIcon)
    Q_PROPERTY(bool checkable READ isCheckable WRITE setCheckable)
    Q_PROPERTY(bool checked READ isChecked WRITE setChecked)
    /**
     * This property holds whether the item is enabled.
     *
     * It dis-/enables both the widget and the item in the list-/treeview.
     */
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)
public:
    /**
     * Creates a new page widget item.
     *
     * @param widget The widget that is shown as page in the VPageWidget.
     */
    VPageWidgetItem(QWidget *widget);

    /**
     * Creates a new page widget item.
     *
     * @param widget The widget that is shown as page in the VPageWidget.
     * @param name The localized string that is show in the navigation view
     *             of the VPageWidget.
     */
    VPageWidgetItem(QWidget *widget, const QString &name);

    /**
     * Destroys the page widget item.
     */
    ~VPageWidgetItem();

    /**
     * Returns the widget of the page widget item.
     */
    QWidget *widget() const;

    /**
     * Sets the name of the item as shown in the navigation view of the page
     * widget.
     */
    void setName(const QString &name);

    /**
     * Returns the name of the page widget item.
     */
    QString name() const;

    /**
     * Sets the header of the page widget item.
     *
     * If setHeader(QString()) is used, what is the default if the header
     * does not got set explicit, then the defined name() will also be used
     * for the header. If setHeader("") is used, the header will be hidden
     * even if the @a VPageView::FaceType is something else then Tabbed.
     *
     * @param header Header of the page widget item.
     */
    void setHeader(const QString &header);

    /**
     * Returns the header of the page widget item.
     */
    QString header() const;

    /**
     * Sets the icon of the page widget item.
     * @param icon Icon of the page widget item.
     */
    void setIcon(const QIcon &icon);

    /**
     * Returns the icon of the page widget item.
     */
    QIcon icon() const;

    /**
     * Sets whether the page widget item is checkable in the view.
     * @param checkable True if the page widget is checkable,
     *                  otherwise false.
     */
    void setCheckable(bool checkable);

    /**
     * Returns whether the page widget item is checkable.
     */
    bool isCheckable() const;

    /**
     * Returns whether the page widget item is checked.
     */
    bool isChecked() const;

    /**
     * Returns whether the page widget item is enabled.
     */
    bool isEnabled() const;

public slots:
    /**
     * Sets whether the page widget item is enabled.
     */
    void setEnabled(bool);

    /**
     * Sets whether the page widget item is checked.
     */
    void setChecked(bool checked);

signals:
    /**
     * This signal is emitted whenever the icon or header
     * is changed.
     */
    void changed();

    /**
     * This signal is emitted whenever the user checks or
     * unchecks the item of @see setChecked() is called.
     */
    void toggled(bool checked);

private:
    class Private;
    Private *const d;
};

class VPageWidgetModelPrivate;

/**
 * \class VPageWidgetModel vpagewidgetmodel.h <VPageWidgetModel>
 *
 * This page model is used by @see VPageWidget to provide
 * a hierarchical layout of pages.
 *
 * @author Tobias Koenig <tokoe@kde.org>
 * @author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VIBE_EXPORT VPageWidgetModel : public VPageModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(VPageWidgetModel)
public:
    /**
     * Creates a new page widget model.
     *
     * @param parent The parent object.
     */
    explicit VPageWidgetModel(QObject *parent = 0);

    /**
     * Destroys the page widget model.
     */
    ~VPageWidgetModel();

    /**
     * Adds a new top level page to the model.
     *
     * @param widget The widget of the page.
     * @param name The name which is displayed in the navigation view.
     *
     * @returns The associated @see VPageWidgetItem.
     */
    VPageWidgetItem *addPage(QWidget *widget, const QString &name);

    /**
     * Adds a new top level page to the model.
     *
     * @param item The @see VPageWidgetItem which describes the page.
     */
    void addPage(VPageWidgetItem *item);

    /**
     * Inserts a new page in the model.
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
     * Inserts a new page in the model.
     *
     * @param before The new page will be insert before this @see VPageWidgetItem
     *               on the same level in hierarchy.
     *
     * @param item The @see VPageWidgetItem which describes the page.
     */
    void insertPage(VPageWidgetItem *before, VPageWidgetItem *item);

    /**
     * Inserts a new sub page in the model.
     *
     * @param parent The new page will be insert as child of this @see VPageWidgetItem.
     * @param widget The widget of the page.
     * @param name The name which is displayed in the navigation view.
     *
     * @returns The associated @see VPageWidgetItem.
     */
    VPageWidgetItem *addSubPage(VPageWidgetItem *parent, QWidget *widget, const QString &name);

    /**
     * Inserts a new sub page in the model.
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
     * These methods are reimplemented from QAbstractItemModel.
     */
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     * Returns the @see VPageWidgetItem for a given index or 0 if the index is invalid.
     */
    VPageWidgetItem *item(const QModelIndex &index) const;

    /**
     * Returns the index for a given @see VPageWidgetItem. The index is invalid if the
     * item can't be found in the model.
     */
    QModelIndex index(const VPageWidgetItem *item) const;

Q_SIGNALS:
    /**
     * This signal is emitted whenever a checkable page changes its state. @param checked is true
     * when the @param page is checked, or false if the @param page is unchecked.
     */
    void toggled(VPageWidgetItem *page, bool checked);

private:
    Q_PRIVATE_SLOT(d_func(), void _q_itemChanged())
    Q_PRIVATE_SLOT(d_func(), void _q_itemToggled(bool))
};

/** @}*/

#endif // VPAGEWIDGETMODEL_H
