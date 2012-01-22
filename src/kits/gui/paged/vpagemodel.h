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

#ifndef VPAGEMODEL_H
#define VPAGEMODEL_H

#include <QAbstractItemModel>

#include <VibeCore/VGlobal>

class VPageModelPrivate;

/** \addtogroup gui Gui Kit
 *  @{
 */

/**
 * \class VPageModel vpagemodel.h <VPageModel>
 *
 * @short A base class for a model used by VPageView.
 *
 * This class is an abstract base class which must be used to
 * implement custom models for VPageView. Additional to the standard
 * Qt::ItemDataRoles it provides the two roles
 *
 *   @li HeaderRole
 *   @li WidgetRole
 *
 * which are used to return a header string for a page and a QWidget
 * pointer to the page itself.
 *
 * <b>Example:</b>\n
 *
 * \code
 *   VPageView *view = new VPageView(this);
 *   VPageModel *model = new MyPageModel(this);
 *
 *   view->setModel(model);
 * \endcode
 *
 * @see VPageView
 *
 * @author Tobias Koenig <tokoe@kde.org>
 * @author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VIBE_EXPORT VPageModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(VPageModel)
public:
    /**
     * Additional roles that VPageView uses.
     */
    enum Role {
        /**
         * A string to be rendered as page header.
         */
        HeaderRole = Qt::UserRole + 1,

        /**
         * A pointer to the page widget. This is the widget that is shown when the item is
         * selected.
         *
         * You can make QVariant take a QWidget using
         * \code
         * QWidget *myWidget = new QWidget;
         * QVariant v = QVariant::fromValue(myWidget);
         * \endcode
         */
        WidgetRole
    };

    /**
     * Constructs a page model with the given parent.
     */
    explicit VPageModel(QObject *parent = 0);

    /**
     * Destroys the page model.
     */
    virtual ~VPageModel();

protected:
    VPageModel(VPageModelPrivate &dd, QObject *parent);
    VPageModelPrivate *const d_ptr;
};

/** @}*/

#endif // VPAGEMODEL_H
