/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (C) 2009 Dzimi Mve Alexandre
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *    Dzimi Mve Alexandre <dzimiwine@gmail.com>
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

#include "vbreadcrumbbar.h"
#include "vbreadcrumbdirmodel.h"
#include "vbreadcrumbbar_ui.h"
#include "vbreadcrumbbar_p.h"
#include "vstyle.h"
#include "vstyleoptionbreadcrumbbar.h"

#define ICON_SIZE 16

/*
 * VBreadCrumbItem
 */

VBreadCrumbItem::VBreadCrumbItem(VBreadCrumbComboBoxContainer *container, VBreadCrumbItem::Type type) :
    m_visible(false),
    m_type(type),
    m_container(container)
{
}

/*
 * VBreadCrumbEmptyArea
 */

VBreadCrumbEmptyArea::VBreadCrumbEmptyArea(VBreadCrumbComboBoxContainer *container) :
    VBreadCrumbItem(container, EmptyArea)
{
    setVisible(true);
}

void VBreadCrumbEmptyArea::clicked(const QPoint &)
{
    container()->comboBox()->edit();
}


/*
 * VBreadCrumbIndicator
 */

VBreadCrumbIndicator::VBreadCrumbIndicator(VBreadCrumbLabel *label, VBreadCrumbComboBoxContainer *container)
    : VBreadCrumbItem(container, Indicator), m_trunc(false), m_label(label)
{
    if (!label)
        setVisible(true);
    else
        label->setIndicator(this);
}

VBreadCrumbIndicator::~VBreadCrumbIndicator()
{
}

void VBreadCrumbIndicator::clicked(const QPoint &)
{
    VBreadCrumbComboBoxContainer *cont = container();
    VBreadCrumbComboBox *comboBox = cont->comboBox();
    VAbstractBreadCrumbModel *model = comboBox->bar()->model();
    VBreadCrumbModelNode node("", VBreadCrumbModelNode::Global);
    QMenu *menu;

    menu = model->buildMenu(m_label ? m_label->node() : node);
    if (!menu || (menu && menu->actions().isEmpty()))
        return;
    menu->connect(menu, SIGNAL(triggered(QAction *)), container()->comboBox(), SLOT(slotSetLocation(QAction *)));
    menu->exec(cont->mapToGlobal(rect().bottomLeft()));
}

/*
 * VBreadCrumbLabel
 */

VBreadCrumbLabel::VBreadCrumbLabel(VBreadCrumbIndicator *indicator, VBreadCrumbComboBoxContainer *container, const VBreadCrumbModelNode &node) :
    VBreadCrumbItem(container, Label),
    m_node(node),
    m_indicator(indicator)
{
}

void VBreadCrumbLabel::clicked(const QPoint &)
{
    VBreadCrumbComboBoxContainer *cont = container();

    //To change
    cont->comboBox()->setLocation(m_node.path());
}

/*
 * VBreadCrumbComboBoxContainer
 */

VBreadCrumbComboBoxContainer::VBreadCrumbComboBoxContainer(VBreadCrumbComboBox *comboBox)
    : QWidget(comboBox), m_hoverItem(-1), m_downItem(-1),  m_comboBox(comboBox), m_clicked(false)
{
    setMouseTracking(true);
    m_rootIndicator = new VBreadCrumbIndicator(0, this);
    m_emptyArea = new VBreadCrumbEmptyArea(this);

}

VBreadCrumbComboBoxContainer::~VBreadCrumbComboBoxContainer()
{
    clearAll();
}

void VBreadCrumbComboBoxContainer::splitPath(const QString &location)
{
    VAbstractBreadCrumbModel *model = m_comboBox->bar()->model();

    m_nodeList = model->splitPath(location);
    refresh();
}

void VBreadCrumbComboBoxContainer::refresh()
{
    VAbstractBreadCrumbModel *model = m_comboBox->bar()->model();
    VBreadCrumbModelNode node("", VBreadCrumbModelNode::Global);
    VBreadCrumbLabel *item;
    VBreadCrumbIndicator *indic;

    clear();

    if (m_nodeList.isEmpty())
        return;
    if (model->supportsMenuNavigation())
        m_items << m_rootIndicator;

    for (int i = 0; i < m_nodeList.count(); i++) {
        node = m_nodeList[i];
        indic = 0;
        item = new VBreadCrumbLabel(0, this, node);
        m_items.append(item);
        if (node.type() != VBreadCrumbModelNode::Leaf && model->supportsMenuNavigation())
            m_items.append(indic = new VBreadCrumbIndicator(item, this));
        if (i == m_nodeList.count() - 1) {
            item->setVisible(true);
            if (indic)
                indic->setVisible(true);
        }
    }
    m_items << m_emptyArea;
    updateGeometries();
    update();
}

void VBreadCrumbComboBoxContainer::mousePressEvent(QMouseEvent *evt)
{
    if (evt->button() == Qt::LeftButton) {
        m_downItem = itemAt(evt->pos());
        if (m_downItem != -1 && m_items[m_downItem]->type() == VBreadCrumbItem::Indicator) {
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

void VBreadCrumbComboBoxContainer::mouseMoveEvent(QMouseEvent *evt)
{
    int hover = m_hoverItem;

    m_hoverItem = itemAt(evt->pos());
    if (hover != m_hoverItem)
        update();
}

void VBreadCrumbComboBoxContainer::mouseReleaseEvent(QMouseEvent *evt)
{
    if (evt->button() == Qt::LeftButton) {
        if (m_downItem != -1 && m_downItem == m_hoverItem &&  m_items[m_downItem]->type() != VBreadCrumbItem::Indicator)
            m_items[m_downItem]->clicked(evt->pos());
        m_downItem = -1;
        update();
    }
}

void VBreadCrumbComboBoxContainer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    VBreadCrumbBar *bar =  m_comboBox->bar();
    VBreadCrumbIndicator *indicator;
    VBreadCrumbLabel *label;
    VAbstractBreadCrumbModel *model = bar->model();
    QStyle *style = bar->style();
    VBreadCrumbItem *item;
    QStyleOptionComboBox option;

    VStyleOptionBreadCrumbIndicator incOption;
    VStyleOptionBreadCrumbLabel labelOption;
    QStyle::State state;
    int nextIndex, previousIndex;

    option.initFrom(m_comboBox);
    incOption.initFrom(m_comboBox);
    labelOption.initFrom(m_comboBox);

    if (bar->isEditable()) {
        option.rect = style->subControlRect(QStyle::CC_ComboBox, &option, (QStyle::SubControl)VStyle::SC_BreadCrumbEditField, bar);
        option.rect.moveTopLeft(mapFromParent(option.rect.topLeft()));
        style->drawPrimitive((QStyle::PrimitiveElement)VStyle::PE_BreadCrumbContainerBase, &option, &painter, bar);
    }
    for (int i = 0; i < m_items.count(); i++) {
        state = isEnabled() ? QStyle::State_Enabled : QStyle::State_None;
        item = m_items[i];
        if (item->isVisible()) {
            if (item->type() == VBreadCrumbItem::Indicator) {
                previousIndex = i - 1;
                indicator = static_cast< VBreadCrumbIndicator * >(item);
                incOption.rect = indicator->rect();
                incOption.hasLabel = (indicator->label() != 0);
                incOption.isTruncated = indicator->isTruncated();
                incOption.usePseudoState = false;
                if (incOption.hasLabel)
                    incOption.isValid = model->isValid(indicator->label()->node().path());
                if ((m_downItem == i && m_hoverItem == i) || (m_downItem == i && m_clicked))
                    state |= QStyle::State_Sunken;
                else if (m_downItem == -1 && i == m_hoverItem)
                    state |= QStyle::State_MouseOver;
                else if (incOption.hasLabel) {
                    if (m_downItem == previousIndex && m_hoverItem == previousIndex) {
                        incOption.usePseudoState = true;
                        state |= QStyle::State_Sunken;
                    } else if (m_downItem == -1 && m_hoverItem == previousIndex) {
                        incOption.usePseudoState = true;
                        state |= QStyle::State_MouseOver;
                    }
                }
                incOption.state = state;
                style->drawControl((QStyle::ControlElement)VStyle::CE_BreadCrumbIndicator, &incOption, &painter, bar);
            } else if (item->type() == VBreadCrumbItem::Label) {
                nextIndex = i + 1;
                label = static_cast< VBreadCrumbLabel * >(item);
                labelOption.rect = label->rect();
                labelOption.hasIndicator = (label->indicator() != 0);
                labelOption.text = label->node().label();
                labelOption.usePseudoState = false;
                labelOption.isValid = model->isValid(label->node().path());
                if (m_downItem == i && m_hoverItem == i)
                    state |= QStyle::State_Sunken;
                else if (m_downItem == -1 && m_hoverItem == i)
                    state |= QStyle::State_MouseOver;
                else if (labelOption.hasIndicator) {
                    if ((m_downItem == nextIndex && m_hoverItem == nextIndex) || (m_downItem == nextIndex && m_clicked)) {
                        labelOption.usePseudoState = true;
                        state |= QStyle::State_Sunken;
                    } else if (m_downItem == -1 && m_hoverItem == nextIndex) {
                        labelOption.usePseudoState = true;
                        state |= QStyle::State_MouseOver;
                    }
                }
                labelOption.state = state;
                style->drawControl((QStyle::ControlElement)VStyle::CE_BreadCrumbLabel, &labelOption, &painter, bar);
            } else {
                option.state = state;
                option.rect = item->rect();
                style->drawControl((QStyle::ControlElement)VStyle::CE_BreadCrumbEmptyArea, &option, &painter, bar);
            }
        }
    }
}

void VBreadCrumbComboBoxContainer::resizeEvent(QResizeEvent *)
{
    updateGeometries();
}

void VBreadCrumbComboBoxContainer::leaveEvent(QEvent *)
{
    m_hoverItem = -1;
    update();
}

void VBreadCrumbComboBoxContainer::clear()
{
    if (m_items.isEmpty())
        return;
    m_items.removeFirst();
    m_items.removeLast();
    while (!m_items.isEmpty())
        delete m_items.takeFirst();
}

void VBreadCrumbComboBoxContainer::clearAll()
{
    while (!m_items.isEmpty())
        delete m_items.takeFirst();
}

void VBreadCrumbComboBoxContainer::updateGeometries()
{
    VBreadCrumbBar *bar = m_comboBox->bar();
    QStyle *style = bar->style();
    VStyleOptionBreadCrumbIndicator arrowOption;
    VStyleOptionBreadCrumbLabel labelOption;
    QStyleOption emptyAreaOption;
    VAbstractBreadCrumbModel *model = bar->model();
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
    VBreadCrumbItem *item;
    VBreadCrumbIndicator *indic;
    VBreadCrumbLabel *label;
    QRect containerRect;

    if (m_items.isEmpty())
        return;

    arrowOption.rect = rect();

    if (model->supportsMenuNavigation())
        arrowRect = style->subElementRect((QStyle::SubElement)VStyle::SE_BreadCrumbIndicator, &arrowOption, bar);
    if (m_items[lastLabelIndex]->type() == VBreadCrumbItem::Indicator) {
        incWidth = arrowRect.width();
        lastLabelIndex = m_items.count() - 3;
        m_items[lastLabelIndex - 1]->setVisible(true);
    }
    labelOption.text = static_cast< VBreadCrumbLabel * >(m_items[lastLabelIndex])->node().label();
    labelOption.rect = rect();
    labelRect = style->subElementRect((QStyle::SubElement)VStyle::SE_BreadCrumbLabel, &labelOption, bar);
    lastLabelWidth = labelRect.width();
    emptyAreaOption.initFrom(this);
    emptyAreaRect = style->subElementRect((QStyle::SubElement)VStyle::SE_BreadCrumbEmptyArea, &emptyAreaOption, bar);
    r.setWidth(r.width() - emptyAreaRect.width() - (arrowRect.width() + incWidth) - lastLabelWidth);
    m_items[lastLabelIndex]->setVisible(true);
    if (model->supportsMenuNavigation()) {
        for (i = lastLabelIndex - 1; i > 1; i -= 2) {
            item = m_items[i];
            indic = static_cast< VBreadCrumbIndicator * >(item);
            label = indic->label();
            indic->setVisible(false);
            label->setVisible(false);
            labelOption.text = label->node().label();
            labelRect = style->subElementRect((QStyle::SubElement)VStyle::SE_BreadCrumbLabel, &labelOption, bar);
            if (!trunc) {
                tempWidth = arrowRect.width() + labelRect.width();
                if (r.width() < (accWidth + tempWidth)) {
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
            label = static_cast< VBreadCrumbLabel * >(item);
            label->setVisible(false);
            labelOption.text = label->node().label();
            labelRect = style->subElementRect((QStyle::SubElement)VStyle::SE_BreadCrumbLabel, &labelOption, bar);
            if (!trunc) {
                tempWidth = labelRect.width();
                if (r.width() < (accWidth + tempWidth)) {
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
    foreach(VBreadCrumbItem * item, m_items) {
        if (item->isVisible()) {
            if (item == m_emptyArea)  {
                tempRect = emptyAreaRect;
                tempRect.setWidth(remainingWidth);
            } else if (item->type() == VBreadCrumbItem::Label) {
                labelOption.text = static_cast< VBreadCrumbLabel * >(item)->node().label();
                tempRect = style->subElementRect((QStyle::SubElement)VStyle::SE_BreadCrumbLabel, &labelOption, bar);
            } else
                tempRect = arrowRect;
            tempRect.moveTo(startPoint);
            item->setRect(tempRect);
            startPoint.setX(startPoint.x() + tempRect.width());
        }
    }
}

int VBreadCrumbComboBoxContainer::itemAt(const QPoint &pos)
{
    for (int i = 0; i < m_items.count(); i++)
        if (m_items[i]->isVisible())
            if (m_items[i]->rect().contains(pos))
                return i;
    return -1;
}

/*
 * VBreadCrumbIconWidget
 */

VBreadCrumbIconWidget::VBreadCrumbIconWidget(VBreadCrumbComboBox *comboBox) :
    QWidget(comboBox),
    m_comboBox(comboBox),
    m_clicked(false)
{
    QAction *action;

    setContextMenuPolicy(Qt::ActionsContextMenu);
    addAction(action = new QAction("Copy Address", this));
    connect(action, SIGNAL(triggered()), this, SLOT(slotCopyAddress()));
    addAction(action = new QAction("Edit Address", this));
    connect(action, SIGNAL(triggered()), this, SLOT(slotEditAddress()));
}

void VBreadCrumbIconWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if (m_pixmap.isNull())
        return;
    QIcon icon(m_pixmap);

    QRect r(QPoint(), m_pixmap.size());

    if (m_comboBox->bar()->isEditable())
        painter.fillRect(rect(), m_comboBox->palette().base().color());

    r.moveCenter(rect().center());

    painter.drawPixmap(r, icon.pixmap(m_pixmap.size(), isEnabled() ? QIcon::Normal : QIcon::Disabled));
}

void VBreadCrumbIconWidget::mousePressEvent(QMouseEvent *evt)
{
    if (evt->button() == Qt::LeftButton) {
        m_comboBox->edit();
    }
}

void VBreadCrumbIconWidget::slotCopyAddress()
{
    qApp->clipboard()->setText(m_comboBox->currentText());
}

void VBreadCrumbIconWidget::slotEditAddress()
{
    m_comboBox->edit();
}

/*
 * VBreadCrumbComboBox
 */

VBreadCrumbComboBox::VBreadCrumbComboBox(VBreadCrumbBar *parent) : QComboBox(parent),
    m_flat(false), m_bar(parent), m_clicked(false)
{
    QPalette pal = palette();
    QCompleter *c;

    setObjectName("Location Bar");
    setDuplicatesEnabled(false);
    c = new QCompleter(this);
    setCompleter(c);
    m_iconLabel = new VBreadCrumbIconWidget(this);
    pal.setBrush(QPalette::Normal, QPalette::Window, pal.brush(QPalette::Normal, QPalette::Base));
    pal.setBrush(QPalette::Disabled, QPalette::Window, pal.brush(QPalette::Disabled, QPalette::Base));
    pal.setBrush(QPalette::Inactive, QPalette::Window, pal.brush(QPalette::Inactive, QPalette::Base));
    m_iconLabel->setPalette(pal);
    setEditable(true);
    connect(lineEdit(), SIGNAL(returnPressed()), this, SLOT(slotHandleEditTextChanged()));
    m_container = new VBreadCrumbComboBoxContainer(this);
    m_container->setAutoFillBackground(false);
    connect(this, SIGNAL(activated(int)), this, SLOT(slotActivated()));
}

VBreadCrumbComboBox::~VBreadCrumbComboBox()
{
}

bool VBreadCrumbComboBox::event(QEvent *e)
{
    if (e->type() == QEvent::Paint) {
        if (!m_bar->isEditable())
            return true;
    } else if (e->type() == QEvent::ContextMenu)
        if (lineEdit()->isHidden())
            return true;
    return QComboBox::event(e);

}

void VBreadCrumbComboBox::setLocation(const QString &location)
{
    VAbstractBreadCrumbModel *model = m_bar->model();
    QString tempLocation = location;
    if (tempLocation.isEmpty())
        tempLocation = model->defaultPath();
    else if (!model->isValid(tempLocation))
        return;
    m_location = model->cleanPath(tempLocation);
    setEditText(m_location);
    m_container->splitPath(m_location);
    m_iconLabel->setPixmap(model->icon(m_container->m_nodeList.last()).pixmap(ICON_SIZE, ICON_SIZE));
}

void VBreadCrumbComboBox::showPopup()
{
    if (m_flat)
        return;
    edit();
    QComboBox::showPopup();
}

void VBreadCrumbComboBox::showVBreadCrumbs(bool popupError)
{
    QString text = currentText();
    VAbstractBreadCrumbModel *model = m_bar->model();

    if (!m_flat) {
        if (text.isEmpty())
            return;
        if (!model->isValid(text)) {
            if (popupError) {
                QString caption =  qApp->applicationName();

                if (!caption.isEmpty())
                    caption += " - ";
                caption += objectName();
                QMessageBox::critical(this, caption, QString("Location '%1' cannot be found. Check the spelling and try again.").arg(text));
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

void VBreadCrumbComboBox::setFlat(bool f)
{
    if (m_flat == f)
        return;
    m_flat = f;
    m_iconLabel->actions()[1]->setEnabled(!f);
    showVBreadCrumbs(false);
    updateGeometries();
    update();
}

void VBreadCrumbComboBox::edit()
{
    QLineEdit *lineEdit = this->lineEdit();

    if (m_flat)
        return;
    m_container->hide();
    lineEdit->show();
    lineEdit->selectAll();
}

void VBreadCrumbComboBox::slotSetLocation(QAction *action)
{
    setLocation(action->data().toString());
}

void VBreadCrumbComboBox::slotHandleEditTextChanged()
{
    updateGeometries();
    showVBreadCrumbs();
}

void VBreadCrumbComboBox::slotActivated()
{
    showVBreadCrumbs(false);
}

void VBreadCrumbComboBox::resizeEvent(QResizeEvent *evt)
{
    QComboBox::resizeEvent(evt);
    updateGeometries();
}

void VBreadCrumbComboBox::focusOutEvent(QFocusEvent *evt)
{
    QWidget *focus = qApp->focusWidget();

    if (focus && focus != this && evt->reason() != Qt::PopupFocusReason)
        showVBreadCrumbs(false);
    QComboBox::focusOutEvent(evt);
}

void VBreadCrumbComboBox::updateGeometries()
{
    QStyleOptionComboBox option;
    QLineEdit *lineEdit = this->lineEdit();
    QStyle *style = m_bar->style();

    initStyleOption(&option);
    m_iconLabel->setGeometry(style->subControlRect(QStyle::CC_ComboBox, &option, (QStyle::SubControl)VStyle::SC_BreadCrumbIcon, m_bar));
    if (!m_flat && lineEdit)
        lineEdit->setGeometry(style->subControlRect(QStyle::CC_ComboBox, &option, (QStyle::SubControl)VStyle::SC_BreadCrumbEditField, m_bar));
    m_container->setGeometry(style->subControlRect(QStyle::CC_ComboBox, &option, (QStyle::SubControl)VStyle::SC_BreadCrumbContainer, m_bar));
}

/*
 * VBreadCrumbBarPrivate
 */

VBreadCrumbBarPrivate::VBreadCrumbBarPrivate(VBreadCrumbBar *q_ptr) :
    flat(false),
    editable(true),
    comboBox(0),
    model(0)
{
    comboBox = new VBreadCrumbComboBox(q_ptr);
}

VBreadCrumbBarPrivate::~VBreadCrumbBarPrivate()
{
}

/*
 * VBreadCrumbBar
 */

VBreadCrumbBar::VBreadCrumbBar(QWidget *parent) :
    QWidget(parent, 0),
    d_ptr(new VBreadCrumbBarPrivate(this))
{
    Q_D(VBreadCrumbBar);

    setModel(new VBreadCrumbDirModel);
    setLocation(QString());
    d->comboBox->showVBreadCrumbs(false);
    setSizePolicy(d->comboBox->sizePolicy());
}

VBreadCrumbBar::~VBreadCrumbBar()
{
}

QComboBox *VBreadCrumbBar::comboBox() const
{
    Q_D(const VBreadCrumbBar);

    return d->comboBox;
}

bool VBreadCrumbBar::isEditable() const
{
    Q_D(const VBreadCrumbBar);

    return d->editable;
}

void VBreadCrumbBar::setEditable(bool e)
{
    Q_D(VBreadCrumbBar);

    d->editable = e;
    d->comboBox->setFlat(!e);
}

QString VBreadCrumbBar::location() const
{
    Q_D(const VBreadCrumbBar);

    return d->comboBox->location();
}

QSize VBreadCrumbBar::sizeHint() const
{
    Q_D(const VBreadCrumbBar);

    QSize size = d->comboBox->sizeHint();
    QStyleOptionComboBox option;
    QStyle *s = style();

    option.initFrom(d->comboBox);
    size = QSize(size.width() + s->subElementRect((QStyle::SubElement)VStyle::SE_BreadCrumbEmptyArea, &option, this).width(), size.height());
    return size;
}

void VBreadCrumbBar::setLocation(const QString &location)
{
    Q_D(VBreadCrumbBar);
    QString old = this->location(), current;

    d->comboBox->setLocation(location);
    if (old != (current = this->location()))
        emit locationChanged(current);
}

VAbstractBreadCrumbModel *VBreadCrumbBar::model() const
{
    Q_D(const VBreadCrumbBar);

    return d->model;
}

void VBreadCrumbBar::setModel(VAbstractBreadCrumbModel *model)
{
    Q_D(VBreadCrumbBar);
    QAbstractItemModel *itemModel;

    if (!model)
        return;
    if (d->model != model) {
        d->model = model;
        if (d->comboBox && (itemModel = model->itemModel())) {
            d->comboBox->completer()->setCompletionMode(QCompleter::PopupCompletion);
            d->comboBox->completer()->setModel(itemModel);
        }
        d->comboBox->container()->refresh();
    }
}

void VBreadCrumbBar::refresh()
{
    Q_D(VBreadCrumbBar);

    d->comboBox->updateGeometries();
    d->comboBox->container()->refresh();
}

void VBreadCrumbBar::edit()
{
    Q_D(VBreadCrumbBar);

    d->comboBox->edit();
}

void VBreadCrumbBar::goToDefault()
{
    Q_D(VBreadCrumbBar);

    setLocation(d->model->defaultPath());
}

void VBreadCrumbBar::resizeEvent(QResizeEvent *)
{
    Q_D(VBreadCrumbBar);

    d->comboBox->setGeometry(QRect(QPoint(), size()));
}

void VBreadCrumbBar::paintEvent(QPaintEvent *)
{
    Q_D(VBreadCrumbBar);

    QPainter painter(this);
    QStyleOption option;

    if (!d->editable) {
        option.initFrom(this);
        style()->drawPrimitive((QStyle::PrimitiveElement)VStyle::PE_FrameBreadCrumbBar, &option, &painter, this);
    }
}

#include "vbreadcrumbbar.moc"
#include "vbreadcrumbbar_ui.moc"
