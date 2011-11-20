/**********************************************************************
**
** This file is part of on Toolkit.
**
** Copyright (C) 2009-2020 Dzimi Mve Alexandre <dzimiwine@gmail.com>
**
** Contact: dzimiwine@gmail.com
**
** on is a free toolkit developed in Qt by Dzimi Mve A.; you can redistribute
** sources and libraries of this library and/or modify them under the terms of
** the GNU Library General Public License version 3.0 as published by the
** Free Software Foundation and appearing in the file LICENSE.txt included in
** the packaging of this file.
** Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** This SDK is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
**
**********************************************************************/

#ifndef BREADCRUMBBAR_H
#define BREADCRUMBBAR_H

#include <QWidget>

#include <Qube/Gui/InterfaceMacros>

class QComboBox;
template < typename T >
class QList;

namespace Qube
{
    namespace Gui
    {
        class AbstractBreadCrumbModel;
        class BreadCrumbBarPrivate;

        class BreadCrumbBar : public QWidget
        {
            Q_OBJECT
            Q_DECLARE_PRIVATE(BreadCrumbBar)
            Q_PROPERTY(bool editable READ isEditable WRITE setEditable RESET goToDefault DESIGNABLE true)
            Q_PROPERTY(QString location READ location WRITE setLocation DESIGNABLE true NOTIFY locationChanged)

        public:
            QUBE_WIDGET_CAST

            explicit BreadCrumbBar( QWidget * parent = 0 );
            ~BreadCrumbBar();

            QSize sizeHint() const;

            bool isEditable() const;
            void setEditable( bool );

            QString location() const;
            void setLocation( const QString & );

            QComboBox * comboBox() const;

            AbstractBreadCrumbModel * model() const;
            void setModel( AbstractBreadCrumbModel * );

            void refresh();

        public slots:
            void edit();
            void goToDefault();

        protected:
            void resizeEvent( QResizeEvent * );
            void paintEvent( QPaintEvent * );

        signals:
            void locationChanged( const QString & );

private:
BreadCrumbBarPrivate *d_ptr;
        };
    }
}

#endif // BREADCRUMBBAR_H
