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

#ifndef BREADCRUMBBAR_UI_H
#define BREADCRUMBBAR_UI_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the on API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QComboBox>

#include "abstractbreadcrumbmodel.h"
#include "breadcrumbbar.h"

class QAction;
class QLabel;

namespace Qube
{
    namespace Gui
    {
        class BreadCrumbBarStyle;
        class BreadCrumbComboBox;
        class BreadCrumbComboBoxContainer;
        class BreadCrumbLabel;

        class BreadCrumbItem
        {
        public:
            enum Type {EmptyArea,Indicator,Label};

            BreadCrumbItem(BreadCrumbComboBoxContainer *, BreadCrumbItem::Type);
            virtual ~BreadCrumbItem() {}

            inline QRect rect() const {
                return m_rect;
            }
            inline void setRect( const QRect & rect ) {
                m_rect = rect;
            }

            inline void setVisible( bool visible ) {
                m_visible = visible;
            }
            inline bool isVisible() {
                return m_visible;
            }

            inline BreadCrumbComboBoxContainer * container() const {
                return m_container;
            }
            inline Type type() const {
                return m_type;
            }

            virtual void clicked( const QPoint & pos ) = 0;

        private:
            bool m_visible;
            BreadCrumbItem::Type m_type;
            BreadCrumbComboBoxContainer * m_container;
            QRect m_rect;
        };

        class BreadCrumbEmptyArea : public BreadCrumbItem
        {
        public:
            BreadCrumbEmptyArea( BreadCrumbComboBoxContainer * );
            ~BreadCrumbEmptyArea() {}

            void clicked( const QPoint & pos );
        };

        class BreadCrumbIndicator : public BreadCrumbItem
        {
        public:
            BreadCrumbIndicator(BreadCrumbLabel *, BreadCrumbComboBoxContainer *);
            virtual ~BreadCrumbIndicator();

            inline bool isTruncated() const {
                return m_trunc;
            }
            inline void setTruncated( bool trunc ) {
                m_trunc = trunc;
            }

            inline BreadCrumbLabel * label() const {
                return m_label;
            }

            void clicked( const QPoint & pos );

        private:
            bool m_trunc;
            BreadCrumbLabel * m_label;
        };

        class BreadCrumbLabel : public BreadCrumbItem
        {
        public:
            BreadCrumbLabel(BreadCrumbIndicator *, BreadCrumbComboBoxContainer *, const BreadCrumbModelNode & );
            virtual ~BreadCrumbLabel() { }

            inline BreadCrumbModelNode node() const {
                return m_node;
            }

            inline BreadCrumbIndicator * indicator() {
                return m_indicator;
            }
            inline void setIndicator( BreadCrumbIndicator * indicator )  {
                m_indicator = indicator;
            }

            void clicked( const QPoint & pos );

        private:
            BreadCrumbModelNode m_node;
            BreadCrumbIndicator * m_indicator;
        };

        class BreadCrumbComboBoxContainer : public QWidget
        {
            Q_OBJECT

        public:
            BreadCrumbComboBoxContainer( BreadCrumbComboBox * comboBox );
            ~BreadCrumbComboBoxContainer();

            inline BreadCrumbComboBox * comboBox() const {
                return m_comboBox;
            }

            void splitPath( const QString & );

            inline BreadCrumbModelNodeList nodeList() const {
                return m_nodeList;
            }
            void refresh();

        protected:
            void mousePressEvent( QMouseEvent * );
            void mouseMoveEvent( QMouseEvent * );
            void mouseReleaseEvent(QMouseEvent * );
            void paintEvent( QPaintEvent * );
            void resizeEvent( QResizeEvent * );
            void leaveEvent( QEvent * );

        private:
            friend class BreadCrumbComboBox;

            void clear();
            void clearAll();
            void updateGeometries();
            int itemAt( const QPoint & pos );

            QList< BreadCrumbItem * > m_items;
            BreadCrumbModelNodeList m_nodeList;
            BreadCrumbIndicator * m_rootIndicator;
            BreadCrumbEmptyArea * m_emptyArea;

            int m_hoverItem;
            int m_downItem;
            BreadCrumbComboBox * m_comboBox;
            bool m_clicked;
        };

        class BreadCrumbIconWidget : public QWidget
        {
            Q_OBJECT

        public:
            BreadCrumbIconWidget( BreadCrumbComboBox * comboBox );

            inline QPixmap pixmap() const {
                return m_pixmap;
            }
            inline void setPixmap( const QPixmap & p ) {
                m_pixmap = p;
                update();
            }

        protected:
            void paintEvent( QPaintEvent * );
            void mousePressEvent( QMouseEvent * );

        private slots:
            void slotCopyAddress();
            void slotEditAddress();

        private:
            BreadCrumbComboBox * m_comboBox;
            QPixmap m_pixmap;
            bool m_clicked;
        };

        class BreadCrumbComboBox : public QComboBox
        {
            Q_OBJECT

        public:
            explicit BreadCrumbComboBox( BreadCrumbBar * parent );
            ~BreadCrumbComboBox();

            inline bool isFlat() const {
                return m_flat;
            }
            void setFlat( bool f );
            inline BreadCrumbBar * bar() const {
                return m_bar;
            }
            inline QString location() const {
                return m_location;
            }
            void setLocation( const QString & );

            void showPopup();

            void showBreadCrumbs( bool popupError = true );
            void edit();

            inline BreadCrumbComboBoxContainer * container() const {
                return m_container;
            }
            void updateGeometries();
        private slots:
            void slotSetLocation( QAction * );
            void slotHandleEditTextChanged();
            void slotActivated();

        protected:
            bool event( QEvent * );
            void resizeEvent( QResizeEvent * );
            void focusOutEvent( QFocusEvent * );

        private:

            bool m_flat;
            QString m_location;
            BreadCrumbIconWidget * m_iconLabel;
            BreadCrumbComboBoxContainer * m_container;
            BreadCrumbBar * m_bar;
            bool m_clicked;
        };
    }
}

#endif // BREADCRUMBBAR_UI_H
