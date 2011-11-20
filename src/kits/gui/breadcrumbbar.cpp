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
/*!
  \brief This file contains the definition of BreadCrumbBar class.
  \file qirbreadcrumbbar.cpp
  \author Dzimi Mve Alexandre
  \date 07/25/2009
*/
#include <QAction>
#include <QStyle>
#include <QLabel>
#include <QMenu>
#include <QLineEdit>
#include <QPainter>
#include <QStyleOption>
#include <QFontMetrics>
#include <QMouseEvent>
#include <QFocusEvent>
#include <QResizeEvent>
#include <QApplication>
#include <QMessageBox>
#include <QClipboard>
#include <QMimeData>
#include <QCompleter>
#include <QDrag>
#include <QMimeData>
#include <QUrl>

#include "breadcrumbbar.h"
#include "breadcrumbdirmodel.h"
#include "breadcrumbbar_ui.h"
#include "breadcrumbbar_p.h"
#include "qstyleoptionbreadcrumbbar.h"

#define ICON_SIZE 16

namespace Qube
{
    namespace Gui
    {
/////////////////////
//BreadCrumbItem
/////////////////////
        BreadCrumbItem::BreadCrumbItem(BreadCrumbComboBoxContainer * container, BreadCrumbItem::Type type)
            : m_visible(false), m_type(type), m_container(container)
        {
        }

/////////////////////////
//BreadCrumbEmptyArea
/////////////////////////
        BreadCrumbEmptyArea::BreadCrumbEmptyArea( BreadCrumbComboBoxContainer * container )
            : BreadCrumbItem(container,EmptyArea)
        {
            setVisible(true);
        }
        void BreadCrumbEmptyArea::clicked( const QPoint & )
        {
            container()->comboBox()->edit();
        }


/////////////////////////
//BreadCrumbIndicator
/////////////////////////
        BreadCrumbIndicator::BreadCrumbIndicator(BreadCrumbLabel * label, BreadCrumbComboBoxContainer * container)
            : BreadCrumbItem(container,Indicator), m_trunc(false), m_label(label)
        {
            if ( !label )
                setVisible(true);
            else
                label->setIndicator(this);
        }
        BreadCrumbIndicator::~BreadCrumbIndicator()
        {
        }
        void BreadCrumbIndicator::clicked( const QPoint & )
        {
            BreadCrumbComboBoxContainer * cont = container();
            BreadCrumbComboBox * comboBox = cont->comboBox();
            AbstractBreadCrumbModel * model = comboBox->bar()->model();
            BreadCrumbModelNode node("",BreadCrumbModelNode::Global);
            QMenu * menu;

            menu = model->buildMenu(m_label ? m_label->node() : node);
            if ( !menu || (menu && menu->actions().isEmpty()))
                return;
            menu->connect(menu,SIGNAL(triggered( QAction * )),container()->comboBox(),SLOT(slotSetLocation( QAction * )));
            menu->exec(cont->mapToGlobal(rect().bottomLeft()));
        }

/////////////////////////
//BreadCrumbLabel
/////////////////////////
        BreadCrumbLabel::BreadCrumbLabel(BreadCrumbIndicator * indicator, BreadCrumbComboBoxContainer * container, const BreadCrumbModelNode & node)
            : BreadCrumbItem(container,Label),  m_node(node), m_indicator(indicator)
        {
        }
        void BreadCrumbLabel::clicked( const QPoint & )
        {
            BreadCrumbComboBoxContainer * cont = container();

            //To change
            cont->comboBox()->setLocation(m_node.path());
        }


/////////////////////////////////
//BreadCrumbComboBoxContainer
/////////////////////////////////
        BreadCrumbComboBoxContainer::BreadCrumbComboBoxContainer( BreadCrumbComboBox * comboBox )
            : QWidget(comboBox), m_hoverItem(-1), m_downItem(-1),  m_comboBox(comboBox), m_clicked(false)
        {
            setMouseTracking(true);
            m_rootIndicator = new BreadCrumbIndicator(0,this);
            m_emptyArea = new BreadCrumbEmptyArea(this);

        }
        BreadCrumbComboBoxContainer::~BreadCrumbComboBoxContainer()
        {
            clearAll();
        }
        void BreadCrumbComboBoxContainer::splitPath( const QString & location )
        {
            AbstractBreadCrumbModel * model = m_comboBox->bar()->model();

            m_nodeList = model->splitPath(location);
            refresh();
        }
        void BreadCrumbComboBoxContainer::refresh()
        {
            AbstractBreadCrumbModel * model = m_comboBox->bar()->model();
            BreadCrumbModelNode node("",BreadCrumbModelNode::Global);
            BreadCrumbLabel * item;
            BreadCrumbIndicator * indic;

            clear();

            if ( m_nodeList.isEmpty() )
                return;
            if ( model->supportsMenuNavigation() )
                m_items << m_rootIndicator;

            for (int i = 0; i < m_nodeList.count(); i++) {
                node = m_nodeList[i];
                indic = 0;
                item = new BreadCrumbLabel(0,this,node);
                m_items.append(item);
                if ( node.type() != BreadCrumbModelNode::Leaf && model->supportsMenuNavigation())
                    m_items.append(indic = new BreadCrumbIndicator(item,this));
                if ( i == m_nodeList.count() - 1) {
                    item->setVisible(true);
                    if ( indic )
                        indic->setVisible(true);
                }
            }
            m_items << m_emptyArea;
            updateGeometries();
            update();
        }
        void BreadCrumbComboBoxContainer::mousePressEvent( QMouseEvent * evt )
        {
            if ( evt->button() == Qt::LeftButton ) {
                m_downItem = itemAt(evt->pos());
                if (m_downItem != -1 && m_items[m_downItem]->type() == BreadCrumbItem::Indicator) {
                    m_clicked = true;
                    update();
                    m_items[m_downItem]->clicked(evt->pos());
                    m_downItem = -1;
                    m_clicked = false;
                    update();
                } else
                    update();
            }
        }
        void BreadCrumbComboBoxContainer::mouseMoveEvent( QMouseEvent * evt )
        {
            int hover = m_hoverItem;

            m_hoverItem = itemAt(evt->pos());
            if ( hover != m_hoverItem )
                update();
        }
        void BreadCrumbComboBoxContainer::mouseReleaseEvent(QMouseEvent * evt )
        {
            if ( evt->button() == Qt::LeftButton ) {
                if ( m_downItem != -1 && m_downItem == m_hoverItem &&  m_items[m_downItem]->type() != BreadCrumbItem::Indicator)
                    m_items[m_downItem]->clicked(evt->pos());
                m_downItem = -1;
                update();
            }
        }
        void BreadCrumbComboBoxContainer::paintEvent( QPaintEvent * )
        {
            QPainter painter(this);
            BreadCrumbBar * bar =  m_comboBox->bar();
            BreadCrumbIndicator * indicator;
            BreadCrumbLabel * label;
            AbstractBreadCrumbModel * model = bar->model();
            QStyle *style = bar->style();
            BreadCrumbItem * item;
            QStyleOptionComboBox option;

            QStyleOptionBreadCrumbIndicator incOption;
            QStyleOptionBreadCrumbLabel labelOption;
            QStyle::State state;
            int nextIndex, previousIndex;

            option.initFrom(m_comboBox);
            incOption.initFrom(m_comboBox);
            labelOption.initFrom(m_comboBox);

            if ( bar->isEditable() ) {
                option.rect = style->subControlRect(QStyle::CC_ComboBox, &option, (QStyle::SubControl)SC_BreadCrumbEditField, bar);
                option.rect.moveTopLeft(mapFromParent(option.rect.topLeft()));
                style->drawPrimitive((QStyle::PrimitiveElement)PE_BreadCrumbContainerBase,&option,&painter,bar);
            }
            for (int i = 0; i < m_items.count(); i++) {
                state = isEnabled() ? QStyle::State_Enabled : QStyle::State_None;
                item = m_items[i];
                if ( item->isVisible() ) {
                    if ( item->type() == BreadCrumbItem::Indicator ) {
                        previousIndex = i - 1;
                        indicator = static_cast< BreadCrumbIndicator * >(item);
                        incOption.rect = indicator->rect();
                        incOption.hasLabel = (indicator->label() != 0);
                        incOption.isTruncated = indicator->isTruncated();
                        incOption.usePseudoState = false;
                        if ( incOption.hasLabel )
                            incOption.isValid = model->isValid(indicator->label()->node().path());
                        if ((m_downItem == i && m_hoverItem == i) || (m_downItem == i && m_clicked))
                            state |= QStyle::State_Sunken;
                        else if (m_downItem == -1 && i == m_hoverItem)
                            state |= QStyle::State_MouseOver;
                        else if ( incOption.hasLabel ) {
                            if (m_downItem == previousIndex && m_hoverItem == previousIndex) {
                                incOption.usePseudoState = true;
                                state |= QStyle::State_Sunken;
                            } else if (m_downItem == -1 && m_hoverItem == previousIndex) {
                                incOption.usePseudoState = true;
                                state |= QStyle::State_MouseOver;
                            }
                        }
                        incOption.state = state;
                        style->drawControl((QStyle::ControlElement)CE_BreadCrumbIndicator,&incOption,&painter,bar);
                    } else if ( item->type() == BreadCrumbItem::Label ) {
                        nextIndex = i + 1;
                        label = static_cast< BreadCrumbLabel * >(item);
                        labelOption.rect = label->rect();
                        labelOption.hasIndicator = (label->indicator() != 0);
                        labelOption.text = label->node().label();
                        labelOption.usePseudoState = false;
                        labelOption.isValid = model->isValid(label->node().path());
                        if (m_downItem == i && m_hoverItem == i)
                            state |= QStyle::State_Sunken;
                        else if (m_downItem == -1 && m_hoverItem == i)
                            state |= QStyle::State_MouseOver;
                        else if ( labelOption.hasIndicator ) {
                            if ((m_downItem == nextIndex && m_hoverItem == nextIndex) || (m_downItem == nextIndex && m_clicked)) {
                                labelOption.usePseudoState = true;
                                state |= QStyle::State_Sunken;
                            } else if (m_downItem == -1 && m_hoverItem == nextIndex) {
                                labelOption.usePseudoState = true;
                                state |= QStyle::State_MouseOver;
                            }
                        }
                        labelOption.state = state;
                        style->drawControl((QStyle::ControlElement)CE_BreadCrumbLabel,&labelOption,&painter,bar);
                    } else {
                        option.state = state;
                        option.rect = item->rect();
                        style->drawControl((QStyle::ControlElement)CE_BreadCrumbEmptyArea,&option,&painter,bar);
                    }
                }
            }
        }
        void BreadCrumbComboBoxContainer::resizeEvent( QResizeEvent * )
        {
            updateGeometries();
        }
        void BreadCrumbComboBoxContainer::leaveEvent( QEvent * )
        {
            m_hoverItem = -1;
            update();
        }
        void BreadCrumbComboBoxContainer::clear()
        {
            if ( m_items.isEmpty() )
                return;
            m_items.removeFirst();
            m_items.removeLast();
            while ( !m_items.isEmpty() )
                delete m_items.takeFirst();
        }
        void BreadCrumbComboBoxContainer::clearAll()
        {
            while ( !m_items.isEmpty() )
                delete m_items.takeFirst();
        }
        void BreadCrumbComboBoxContainer::updateGeometries()
        {
            BreadCrumbBar * bar = m_comboBox->bar();
            QStyle *style = bar->style();
            QStyleOptionBreadCrumbIndicator arrowOption;
            QStyleOptionBreadCrumbLabel labelOption;
            QStyleOption emptyAreaOption;
            AbstractBreadCrumbModel * model = bar->model();
            QRect r = rect(), arrowRect, labelRect, tempRect, emptyAreaRect;
            int lastLabelWidth,
                incWidth = 0,
                accWidth = 0,
                tempWidth,
                remainingWidth = 0,
                lastLabelIndex = m_items.count() - 2,
                i;
            bool trunc = false;
            QPoint startPoint;
            BreadCrumbItem * item;
            BreadCrumbIndicator * indic;
            BreadCrumbLabel * label;
            QRect containerRect;

            if ( m_items.isEmpty() )
                return;

            arrowOption.rect = rect();

            if ( model->supportsMenuNavigation() )
                arrowRect = style->subElementRect((QStyle::SubElement)SE_BreadCrumbIndicator,&arrowOption,bar);
            if ( m_items[lastLabelIndex]->type() == BreadCrumbItem::Indicator ) {
                incWidth = arrowRect.width();
                lastLabelIndex = m_items.count() - 3;
                m_items[lastLabelIndex - 1]->setVisible(true);
            }
            labelOption.text = static_cast< BreadCrumbLabel * >(m_items[lastLabelIndex])->node().label();
            labelOption.rect = rect();
            labelRect = style->subElementRect((QStyle::SubElement)SE_BreadCrumbLabel,&labelOption,bar);
            lastLabelWidth = labelRect.width();
            emptyAreaOption.initFrom(this);
            emptyAreaRect = style->subElementRect((QStyle::SubElement)SE_BreadCrumbEmptyArea,&emptyAreaOption,bar);
            r.setWidth(r.width() - emptyAreaRect.width() - ( arrowRect.width() + incWidth ) - lastLabelWidth);
            m_items[lastLabelIndex]->setVisible(true);
            if ( model->supportsMenuNavigation() ) {
                for (i = lastLabelIndex - 1; i > 1; i -= 2) {
                    item = m_items[i];
                    indic = static_cast< BreadCrumbIndicator * >(item);
                    label = indic->label();
                    indic->setVisible(false);
                    label->setVisible(false);
                    labelOption.text = label->node().label();
                    labelRect = style->subElementRect((QStyle::SubElement)SE_BreadCrumbLabel,&labelOption,bar);
                    if ( !trunc ) {
                        tempWidth = arrowRect.width() + labelRect.width();
                        if ( r.width() < (accWidth + tempWidth) ) {
                            trunc = true;
                        } else {
                            indic->setVisible(true);
                            label->setVisible(true);
                            accWidth += tempWidth;
                        }
                    }
                }
            } else {
                for (i = lastLabelIndex - 1; i >= 0; i--) {
                    item = m_items[i];
                    label = static_cast< BreadCrumbLabel * >(item);
                    label->setVisible(false);
                    labelOption.text = label->node().label();
                    labelRect = style->subElementRect((QStyle::SubElement)SE_BreadCrumbLabel,&labelOption,bar);
                    if ( !trunc ) {
                        tempWidth = labelRect.width();
                        if ( r.width() < (accWidth + tempWidth) ) {
                            trunc = true;
                        } else {
                            label->setVisible(true);
                            accWidth += tempWidth;
                        }
                    }
                }
            }

            remainingWidth = r.width() - accWidth + emptyAreaRect.width();

            m_rootIndicator->setTruncated(trunc);
            foreach (BreadCrumbItem * item, m_items) {
                if ( item->isVisible() ) {
                    if ( item == m_emptyArea )  {
                        tempRect = emptyAreaRect;
                        tempRect.setWidth(remainingWidth);
                    } else if ( item->type() == BreadCrumbItem::Label ) {
                        labelOption.text = static_cast< BreadCrumbLabel * >(item)->node().label();
                        tempRect = style->subElementRect((QStyle::SubElement)SE_BreadCrumbLabel,&labelOption,bar);
                    } else
                        tempRect = arrowRect;
                    tempRect.moveTo(startPoint);
                    item->setRect(tempRect);
                    startPoint.setX(startPoint.x() + tempRect.width());
                }
            }
        }
        int BreadCrumbComboBoxContainer::itemAt( const QPoint & pos )
        {
            for (int i = 0; i < m_items.count(); i++)
                if ( m_items[i]->isVisible() )
                    if ( m_items[i]->rect().contains(pos) )
                        return i;
            return -1;
        }

////////////////////////
//BreadCrumbIconWidget
//////////////////////
        BreadCrumbIconWidget::BreadCrumbIconWidget( BreadCrumbComboBox * comboBox ) : QWidget(comboBox), m_comboBox(comboBox), m_clicked(false)
        {
            QAction * action;

            setContextMenuPolicy(Qt::ActionsContextMenu);
            addAction(action = new QAction("Copy Address",this));
            connect(action,SIGNAL(triggered()),this,SLOT(slotCopyAddress()));
            addAction(action = new QAction("Edit Address",this));
            connect(action,SIGNAL(triggered()),this,SLOT(slotEditAddress()));
        }
        void BreadCrumbIconWidget::paintEvent( QPaintEvent * )
        {
            QPainter painter(this);

            if ( m_pixmap.isNull() )
                return;
            QIcon icon(m_pixmap);

            QRect r(QPoint(),m_pixmap.size());

            if ( m_comboBox->bar()->isEditable() )
                painter.fillRect(rect(),m_comboBox->palette().base().color());

            r.moveCenter(rect().center());

            painter.drawPixmap(r,icon.pixmap(m_pixmap.size(),isEnabled() ? QIcon::Normal : QIcon::Disabled));
        }

        void BreadCrumbIconWidget::mousePressEvent( QMouseEvent * evt )
        {
            if ( evt->button() == Qt::LeftButton ) {
                m_comboBox->edit();
            }
        }
        void BreadCrumbIconWidget::slotCopyAddress()
        {
            qApp->clipboard()->setText(m_comboBox->currentText());
        }
        void BreadCrumbIconWidget::slotEditAddress()
        {
            m_comboBox->edit();
        }

/////////////////////////////////
//BreadCrumbComboBox
/////////////////////////////////
        BreadCrumbComboBox::BreadCrumbComboBox( BreadCrumbBar * parent ) : QComboBox(parent),
            m_flat(false), m_bar(parent), m_clicked(false)
        {
            QPalette pal = palette();
            QCompleter * c;

            setObjectName("Location Bar");
            setDuplicatesEnabled(false);
            c = new QCompleter(this);
            setCompleter(c);
            m_iconLabel = new BreadCrumbIconWidget(this);
            pal.setBrush(QPalette::Normal,QPalette::Window,pal.brush(QPalette::Normal,QPalette::Base));
            pal.setBrush(QPalette::Disabled,QPalette::Window,pal.brush(QPalette::Disabled,QPalette::Base));
            pal.setBrush(QPalette::Inactive,QPalette::Window,pal.brush(QPalette::Inactive,QPalette::Base));
            m_iconLabel->setPalette(pal);
            setEditable(true);
            connect(lineEdit(),SIGNAL(returnPressed()),this,SLOT(slotHandleEditTextChanged()));
            m_container = new BreadCrumbComboBoxContainer(this);
            m_container->setAutoFillBackground(false);
            connect(this,SIGNAL(activated(int)),this,SLOT(slotActivated()));
        }
        BreadCrumbComboBox::~BreadCrumbComboBox()
        {
        }
        bool BreadCrumbComboBox::event( QEvent * e )
        {
            if ( e->type() == QEvent::Paint ) {
                if ( !m_bar->isEditable() )
                    return true;
            } else if ( e->type() == QEvent::ContextMenu )
                if ( lineEdit()->isHidden() )
                    return true;
            return QComboBox::event(e);

        }
        void BreadCrumbComboBox::setLocation( const QString & location )
        {
            AbstractBreadCrumbModel * model = m_bar->model();
            QString tempLocation = location;
            if ( tempLocation.isEmpty() )
                tempLocation = model->defaultPath();
            else if ( !model->isValid(tempLocation) )
                return;
            m_location = model->cleanPath(tempLocation);
            setEditText(m_location);
            m_container->splitPath(m_location);
            m_iconLabel->setPixmap(model->icon(m_container->m_nodeList.last()).pixmap(ICON_SIZE,ICON_SIZE));
        }
        void BreadCrumbComboBox::showPopup()
        {
            if ( m_flat )
                return;
            edit();
            QComboBox::showPopup();
        }
        void BreadCrumbComboBox::showBreadCrumbs( bool popupError )
        {
            QString text = currentText();
            AbstractBreadCrumbModel * model = m_bar->model();

            if ( !m_flat ) {
                if ( text.isEmpty() )
                    return;
                if ( !model->isValid(text) ) {
                    if ( popupError ) {
                        QString caption =  qApp->applicationName();

                        if ( !caption.isEmpty() )
                            caption += " - ";
                        caption += objectName();
                        QMessageBox::critical(this,caption,QString("Location '%1' cannot be found. Check the spelling and try again.").arg(text));
                        lineEdit()->selectAll();
                        setFocus(Qt::OtherFocusReason);
                    } else
                        return;
                } else {
                    setLocation(model->cleanPath(text));
                    lineEdit()->hide();
                    m_container->show();
                }
            } else {
                if (text.isEmpty() || !model->isValid(text))
                    setLocation(model->defaultPath());
                else
                    setLocation(model->cleanPath(text));
                lineEdit()->hide();
                m_container->show();
            }
        }
        void BreadCrumbComboBox::setFlat( bool f )
        {
            if ( m_flat == f )
                return;
            m_flat = f;
            m_iconLabel->actions()[1]->setEnabled(!f);
            showBreadCrumbs(false);
            updateGeometries();
            update();
        }
        void BreadCrumbComboBox::edit()
        {
            QLineEdit * lineEdit = this->lineEdit();

            if ( m_flat )
                return;
            m_container->hide();
            lineEdit->show();
            lineEdit->selectAll();
        }
        void BreadCrumbComboBox::slotSetLocation( QAction * action )
        {
            setLocation(action->data().toString());
        }
        void BreadCrumbComboBox::slotHandleEditTextChanged()
        {
            updateGeometries();
            showBreadCrumbs();
        }
        void BreadCrumbComboBox::slotActivated()
        {
            showBreadCrumbs(false);
        }
        void BreadCrumbComboBox::resizeEvent( QResizeEvent * evt )
        {
            QComboBox::resizeEvent(evt);
            updateGeometries();
        }
        void BreadCrumbComboBox::focusOutEvent( QFocusEvent * evt )
        {
            QWidget * focus = qApp->focusWidget();

            if ( focus && focus != this && evt->reason() != Qt::PopupFocusReason)
                showBreadCrumbs(false);
            QComboBox::focusOutEvent(evt);
        }
        void BreadCrumbComboBox::updateGeometries()
        {
            QStyleOptionComboBox option;
            QLineEdit * lineEdit = this->lineEdit();
            QStyle *style = m_bar->style();

            initStyleOption(&option);
            m_iconLabel->setGeometry(style->subControlRect(QStyle::CC_ComboBox,&option,(QStyle::SubControl)SC_BreadCrumbIcon,m_bar));
            if ( !m_flat && lineEdit )
                lineEdit->setGeometry(style->subControlRect(QStyle::CC_ComboBox,&option,(QStyle::SubControl)SC_BreadCrumbEditField,m_bar));
            m_container->setGeometry(style->subControlRect(QStyle::CC_ComboBox,&option,(QStyle::SubControl)SC_BreadCrumbContainer,m_bar));
        }

//////////////////////////
// BreadCrumbBarUi
//////////////////////////
        BreadCrumbBarPrivate::BreadCrumbBarPrivate(BreadCrumbBar *q_ptr) :
            flat(false),
            editable(true),
            comboBox(0),
            model(0)
        {
            comboBox = new BreadCrumbComboBox(q_ptr);
        }

        BreadCrumbBarPrivate::~BreadCrumbBarPrivate()
        {
        }

////////////////////
// BreadCrumbBar
////////////////////
        BreadCrumbBar::BreadCrumbBar(QWidget *parent) :
            QWidget(parent, 0),
            d_ptr(new BreadCrumbBarPrivate(this))
        {
Q_D(BreadCrumbBar);

            setModel(new BreadCrumbDirModel);
            setLocation(QString());
            d->comboBox->showBreadCrumbs(false);
            setSizePolicy(d->comboBox->sizePolicy());
        }
        BreadCrumbBar::~BreadCrumbBar()
        {
        }
        QComboBox * BreadCrumbBar::comboBox() const
        {
            Q_D(const BreadCrumbBar);

            return d->comboBox;
        }
        bool BreadCrumbBar::isEditable() const
        {
            Q_D(const BreadCrumbBar);

            return d->editable;
        }
        void BreadCrumbBar::setEditable( bool e )
        {
            Q_D(BreadCrumbBar);

            d->editable = e;
            d->comboBox->setFlat(!e);
        }
        QString BreadCrumbBar::location() const
        {
            Q_D(const BreadCrumbBar);

            return d->comboBox->location();
        }
        QSize BreadCrumbBar::sizeHint() const
        {
            Q_D(const BreadCrumbBar);

            QSize size = d->comboBox->sizeHint();
            QStyleOptionComboBox option;
            QStyle *s = style();

            option.initFrom(d->comboBox);
            size = QSize(size.width() + s->subElementRect((QStyle::SubElement)SE_BreadCrumbEmptyArea,&option,this).width(),size.height());
            return size;
        }
        void BreadCrumbBar::setLocation( const QString & location )
        {
            Q_D(BreadCrumbBar);
            QString old = this->location(), current;

            d->comboBox->setLocation(location);
            if ( old != (current = this->location()) )
                emit locationChanged(current);
        }
        AbstractBreadCrumbModel * BreadCrumbBar::model() const
        {
            Q_D(const BreadCrumbBar);

            return d->model;
        }
        void BreadCrumbBar::setModel( AbstractBreadCrumbModel * model )
        {
            Q_D(BreadCrumbBar);
            QAbstractItemModel * itemModel;

            if ( !model )
                return;
            if ( d->model != model ) {
                d->model = model;
                if ( d->comboBox && (itemModel = model->itemModel())) {
                    d->comboBox->completer()->setCompletionMode(QCompleter::PopupCompletion);
                    d->comboBox->completer()->setModel(itemModel);
                }
                d->comboBox->container()->refresh();
            }
        }
        void BreadCrumbBar::refresh()
        {
            Q_D(BreadCrumbBar);

            d->comboBox->updateGeometries();
            d->comboBox->container()->refresh();
        }
        void BreadCrumbBar::edit()
        {
            Q_D(BreadCrumbBar);

            d->comboBox->edit();
        }
        void BreadCrumbBar::goToDefault()
        {
            Q_D(BreadCrumbBar);

            setLocation(d->model->defaultPath());
        }
        void BreadCrumbBar::resizeEvent( QResizeEvent * )
        {
            Q_D(BreadCrumbBar);

            d->comboBox->setGeometry(QRect(QPoint(),size()));
        }
        void BreadCrumbBar::paintEvent( QPaintEvent * )
        {
            Q_D(BreadCrumbBar);

            QPainter painter(this);
            QStyleOption option;

            if ( !d->editable ) {
                option.initFrom(this);
                style()->drawPrimitive((QStyle::PrimitiveElement)PE_FrameBreadCrumbBar,&option,&painter,this);
            }
        }
    }
}
