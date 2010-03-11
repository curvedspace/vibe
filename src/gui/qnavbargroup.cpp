/****************************************************************************
 *
 * Copyright (c) 2010 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 * Copyright (c) 2008 Matteo Bertozzi <theo.bertozzi@gmail.com>
 * All rights reserved.
 *
 * Contact: Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * GNU Lesser General Public License Usage
 * This file may be used under the terms of the GNU Lesser
 * General Public License version 2.1 as published by the Free Software
 * Foundation and appearing in the file LICENSE.LGPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU Lesser General Public License version 2.1 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 ***************************************************************************/

#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtGui/QStyle>
#include <QtGui/QStyleOption>

#include "qclickablelabel.h"
#include "qnavbargroup.h"

class ArrowLabel : public QClickableLabel
{
public:
    ArrowLabel(bool isStatic)
        : QClickableLabel(),
        m_isStatic(isStatic),
        m_arrow(QStyle::PE_IndicatorArrowDown)
    {
        QFont f = font();
        f.setBold(true);
        f.setPointSizeF(f.pointSizeF() * 0.75f);
        setFont(f);
    }

    bool isStatic() const
    {
        return m_isStatic;
    }

    void setStatic(bool flag)
    {
        m_isStatic = flag;
    }

protected:
    void paintEvent(QPaintEvent *)
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

    void mouseReleaseEvent(QMouseEvent *event)
    {
        QClickableLabel::mouseReleaseEvent(event);

        // Can't continue if static
        if (m_isStatic)
            return;

        // Change arrow
        if (m_arrow == QStyle::PE_IndicatorArrowDown)
            m_arrow = QStyle::PE_IndicatorArrowRight;
        else
            m_arrow = QStyle::PE_IndicatorArrowDown;
    }

private:
    bool m_isStatic;
    QStyle::PrimitiveElement m_arrow;
};

class QNavBarGroup::Private
{
public:
    QList<QNavBarItem *> listItems;
    ArrowLabel *labelTitle;
    QVBoxLayout *layout;
    bool isExpanded;
    bool isStatic;

    void initialize(QNavBarGroup *group);
};

void QNavBarGroup::Private::initialize(QNavBarGroup *group)
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

QNavBarGroup::QNavBarGroup(QWidget *parent)
    : QWidget(parent),
    d(new QNavBarGroup::Private)
{
    d->initialize(this);

    connect(d->labelTitle, SIGNAL(clicked()),
            this, SLOT(titleClicked()));
}

QNavBarGroup::QNavBarGroup(const QString &title, QWidget *parent)
        : QWidget(parent),
        d(new QNavBarGroup::Private)
{
    d->initialize(this);
    d->labelTitle->setText(title);

    connect(d->labelTitle, SIGNAL(clicked()),
            this, SLOT(titleClicked()));
}

QNavBarGroup::~QNavBarGroup()
{
    delete d;
}

void QNavBarGroup::addItem(QNavBarItem *item)
{
    // Insert item space
    item->insertSpacing(0, 10);

    // Insert item into item list
    d->listItems.append(item);

    // Add item to Layout
    d->layout->addWidget(item);

    connect(item, SIGNAL(selected(QSelectableWidget *)),
            this, SLOT(itemSelected(QSelectableWidget *)));
}

void QNavBarGroup::addItem(QNavBarItem *item, int index)
{
    // Insert item space
    item->insertSpacing(0, 20);

    // Insert item into item list
    d->listItems.insert(index, item);

    // Add item to layout
    d->layout->insertWidget(index, item);

    connect(item, SIGNAL(selected(QSelectableWidget *)),
            this, SLOT(itemSelected(QSelectableWidget *)));
}

QNavBarItem *QNavBarGroup::addItem(const QString &text)
{
    QNavBarItem *item = new QNavBarItem(text);
    addItem(item);
    return item;
}

QNavBarItem *QNavBarGroup::addItem(const QPixmap &icon, const QString &text)
{
    QNavBarItem *item = new QNavBarItem(icon, text);
    addItem(item);
    return item;
}

QNavBarItem *QNavBarGroup::addItem(const QPixmap &icon, const QString &text, int index)
{
    QNavBarItem *item = new QNavBarItem(icon, text);
    addItem(item, index);
    return item;
}

bool QNavBarGroup::containsItem(QNavBarItem *item)
{
    return d->listItems.contains(item);
}

QString QNavBarGroup::title() const
{
    return d->labelTitle->text();
}

void QNavBarGroup::setTitle(const QString &title)
{
    d->labelTitle->setText(title);
}

bool QNavBarGroup::isExpanded() const
{
    return d->isExpanded;
}

bool QNavBarGroup::isStatic() const
{
    return d->labelTitle->isStatic();
}

void QNavBarGroup::setStatic(bool flag)
{
    // Static navbar groups are always expanded
    if (flag)
        expand(true);

    d->labelTitle->setStatic(flag);
}

void QNavBarGroup::setTitleColor (const QColor &color)
{
    QPalette palette = d->labelTitle->palette();
    palette.setColor(QPalette::WindowText, color);
    d->labelTitle->setPalette(palette);
}

void QNavBarGroup::expand(bool expand)
{
    if (d->isExpanded == expand)
        return;
    if (d->isStatic)
        return;

    if (expand) {
        foreach (QNavBarItem *item, d->listItems) {
            d->layout->addWidget(item);
            item->show();
        }
    } else {
        foreach (QNavBarItem *item, d->listItems) {
            d->layout->removeWidget(item);
            item->hide();
        }
    }

    d->isExpanded = expand;

    // Raise expanded event
    emit expanded(this);
}

void QNavBarGroup::itemSelected(QSelectableWidget *item)
{
    emit selected(this, (QNavBarItem *)item);
}

void QNavBarGroup::titleClicked()
{
    if (!d->labelTitle->isStatic())
        expand(!d->isExpanded);
}

#include "qnavbargroup.moc"
