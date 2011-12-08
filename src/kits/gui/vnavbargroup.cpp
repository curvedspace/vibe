/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2008 Matteo Bertozzi
 * Copyright (c) 2010-2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Matteo Bertozzi <theo.bertozzi@gmail.com>
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtGui/QStyle>
#include <QtGui/QStyleOption>

#include "vclickablelabel.h"
#include "vnavbargroup.h"

class ArrowLabel : public VClickableLabel
{
public:
    ArrowLabel(bool isStatic);

    bool isStatic() const;

    void setStatic(bool flag);

protected:
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    bool m_isStatic;
    QStyle::PrimitiveElement m_arrow;
};

ArrowLabel::ArrowLabel(bool isStatic)
    : VClickableLabel(),
      m_isStatic(isStatic),
      m_arrow(QStyle::PE_IndicatorArrowDown)
{
    QFont f = font();
    f.setBold(true);
    f.setPointSizeF(f.pointSizeF() * 0.75f);
    setFont(f);
}

bool ArrowLabel::isStatic() const
{
    return m_isStatic;
}

void ArrowLabel::setStatic(bool flag)
{
    m_isStatic = flag;
}

void ArrowLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    const unsigned int arrowSize = contentsRect().height();

    // Set text and arrow's color
    QBrush brush = palette().brush(QPalette::WindowText);
    painter.setBrush(brush);

    // Draw arrow from style
    QStyleOption opt;
    opt.init(this);
    if (isEnabled())
        opt.state |= QStyle::State_Enabled;
    opt.rect = QRect(0, 0, arrowSize, arrowSize);
    style()->drawPrimitive(m_arrow, &opt, &painter, this);

    // Draw text
    QRect r = contentsRect();
    r.adjust(arrowSize + 2, 0, arrowSize + 2, 0);
    style()->drawItemText(&painter, r, 0, palette(), isEnabled(),
                          text(), QPalette::WindowText);
}

void ArrowLabel::mouseReleaseEvent(QMouseEvent *event)
{
    VClickableLabel::mouseReleaseEvent(event);

    // Can't continue if static
    if (m_isStatic)
        return;

    // Change arrow
    if (m_arrow == QStyle::PE_IndicatorArrowDown)
        m_arrow = QStyle::PE_IndicatorArrowRight;
    else
        m_arrow = QStyle::PE_IndicatorArrowDown;
}

class VNavBarGroupPrivate
{
public:
    QList<VNavBarItem *> listItems;
    ArrowLabel *labelTitle;
    QVBoxLayout *layout;
    bool isExpanded;
    bool isStatic;

    void initialize(VNavBarGroup *group);
};

void VNavBarGroupPrivate::initialize(VNavBarGroup *group)
{
    // Initialize members
    layout = new QVBoxLayout();
    labelTitle = new ArrowLabel(false);

    // Set expanded flag
    isExpanded = true;

    // Set static flag
    isStatic = false;

    // Add layout items
    layout->addWidget(labelTitle);

    // Set label title alignment
    labelTitle->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    // Setup layout
    layout->setSpacing(1);
    layout->setContentsMargins(0, 0, 0, 0);
    group->setLayout(layout);
}

VNavBarGroup::VNavBarGroup(QWidget *parent)
    : QWidget(parent),
      d_ptr(new VNavBarGroupPrivate())
{
    Q_D(VNavBarGroup);

    d->initialize(this);
    connect(d->labelTitle, SIGNAL(clicked()),
            this, SLOT(titleClicked()));
}

VNavBarGroup::VNavBarGroup(const QString &title, QWidget *parent)
    : QWidget(parent),
      d_ptr(new VNavBarGroupPrivate())
{
    Q_D(VNavBarGroup);

    d->initialize(this);
    d->labelTitle->setText(title);
    connect(d->labelTitle, SIGNAL(clicked()),
            this, SLOT(titleClicked()));
}

void VNavBarGroup::addItem(VNavBarItem *item)
{
    Q_D(VNavBarGroup);

    // Insert item space
    item->insertSpacing(0, 10);

    // Insert item into item list
    d->listItems.append(item);
    // Add item to Layout

    d->layout->addWidget(item);
    connect(item, SIGNAL(selected(VSelectableWidget *)),
            this, SLOT(itemSelected(VSelectableWidget *)));
}

void VNavBarGroup::addItem(VNavBarItem *item, int index)
{
    Q_D(VNavBarGroup);

    // Insert item space
    item->insertSpacing(0, 20);

    // Insert item into item list
    d->listItems.insert(index, item);
    // Add item to layout

    d->layout->insertWidget(index, item);
    connect(item, SIGNAL(selected(VSelectableWidget *)),
            this, SLOT(itemSelected(VSelectableWidget *)));
}

VNavBarItem *VNavBarGroup::addItem(const QString &text)
{
    VNavBarItem *item = new VNavBarItem(text);
    addItem(item);
    return item;
}

VNavBarItem *VNavBarGroup::addItem(const QPixmap &icon, const QString &text)
{
    VNavBarItem *item = new VNavBarItem(icon, text);
    addItem(item);
    return item;
}

VNavBarItem *VNavBarGroup::addItem(const QPixmap &icon, const QString &text, int index)
{
    VNavBarItem *item = new VNavBarItem(icon, text);
    addItem(item, index);
    return item;
}

bool VNavBarGroup::containsItem(VNavBarItem *item)
{
    Q_D(VNavBarGroup);
    return d->listItems.contains(item);
}

QString VNavBarGroup::title() const
{
    Q_D(const VNavBarGroup);
    return d->labelTitle->text();
}

void VNavBarGroup::setTitle(const QString &title)
{
    Q_D(VNavBarGroup);
    d->labelTitle->setText(title);
}

bool VNavBarGroup::isExpanded() const
{
    Q_D(const VNavBarGroup);
    return d->isExpanded;
}

bool VNavBarGroup::isStatic() const
{
    Q_D(const VNavBarGroup);
    return d->labelTitle->isStatic();
}

void VNavBarGroup::setStatic(bool flag)
{
    Q_D(VNavBarGroup);

    // Static navbar groups are always expanded
    if (flag)
        expand(true);
    d->labelTitle->setStatic(flag);
}

void VNavBarGroup::setTitleColor(const QColor &color)
{
    Q_D(VNavBarGroup);

    QPalette palette = d->labelTitle->palette();
    palette.setColor(QPalette::WindowText, color);
    d->labelTitle->setPalette(palette);
}

void VNavBarGroup::expand(bool expand)
{
    Q_D(VNavBarGroup);

    if (d->isExpanded == expand)
        return;
    if (d->isStatic)
        return;

    if (expand) {
        foreach(VNavBarItem * item, d->listItems) {
            d->layout->addWidget(item);
            item->show();
        }
    } else {
        foreach(VNavBarItem * item, d->listItems) {
            d->layout->removeWidget(item);
            item->hide();
        }
    }

    d->isExpanded = expand;

    // Raise expanded event
    emit expanded(this);
}

void VNavBarGroup::itemSelected(VSelectableWidget *item)
{
    emit selected(this, (VNavBarItem *)item);
}

void VNavBarGroup::titleClicked()
{
    Q_D(VNavBarGroup);

    if (!d->labelTitle->isStatic())
        expand(!d->isExpanded);
}

#include "vnavbargroup.moc"
