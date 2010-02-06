/* 
 *  This file is part of Quartica.
 *
 *  Copyright (c) 2008 Matteo Bertozzi <theo.bertozzi@gmail.com>
 *
 *  Quartica is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Quartica is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Quartica.  If not, see <http://www.gnu.org/licenses/>.
 */

// Qt4 Headers
#include <QVBoxLayout>
#include <QLabel>
#include <QPaintEvent>

// Quartica Headers
#include "qclickablelabel.h"
#include "qnavbargroup.h"

// =============================================================================
//  NavBarGroup: PRIVATE Class
// =============================================================================
class QNavBarGroup::Private {
	public:
                QList<QNavBarItem *> listItems;
                QClickableLabel *labelTitle;
		QVBoxLayout *layout;
		bool isExpanded;
                bool isStatic;

	public:
                void initialize (QNavBarGroup *group);
};

void QNavBarGroup::Private::initialize (QNavBarGroup *group) {
	// Initialize Members
	layout = new QVBoxLayout;
        labelTitle = new QClickableLabel;

        // Set expanded flag
	isExpanded = true;

        // Set static flag
        isStatic = false;

	// Add Layout Items
	layout->addWidget(labelTitle);

        // Set label title
        QFont font = labelTitle->font();
	font.setBold(true);
	font.setPixelSize(10);
	labelTitle->setFont(font);
	labelTitle->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

	// Setup Layout
	layout->setSpacing(1);
	layout->setContentsMargins(0, 0, 0, 0);
	group->setLayout(layout);
}

// =============================================================================
//  NavBarGroup: PUBLIC Constructors/Destructors
// =============================================================================
QNavBarGroup::QNavBarGroup (QWidget *parent)
        : QWidget(parent), d(new QNavBarGroup::Private)
{
	d->initialize(this);
        setStatic(false);

	// Add Events
	connect(d->labelTitle, SIGNAL(clicked()), this, SLOT(onTitleClicked()));
}

QNavBarGroup::QNavBarGroup (const QString& title, QWidget *parent)
        : QWidget(parent), d(new QNavBarGroup::Private)
{
	d->initialize(this);
	d->labelTitle->setText(title);
        setStatic(false);

	// Add Events
	connect(d->labelTitle, SIGNAL(clicked()), this, SLOT(onTitleClicked()));
}

QNavBarGroup::~QNavBarGroup() {
	delete d;
	d = NULL;
}

// =============================================================================
//  NavBarGroup: PUBLIC Methods - Add Item
// =============================================================================
void QNavBarGroup::addItem (QNavBarItem *item) {
	// Insert Item Space
	item->insertSpacing(0, 10);

	// Insert Item into Item List
	d->listItems.append(item);

	// Add Item to Layout
	d->layout->addWidget(item);

	// Add Event
        connect(item, SIGNAL(selected(QSelectableWidget *)),
                        this, SLOT(onItemSelected(QSelectableWidget *)));
}

void QNavBarGroup::addItem (QNavBarItem *item, int index) {
	// Insert Item Space
	item->insertSpacing(0, 20);

	// Insert Item into Item List
	d->listItems.insert(index, item);

	// Add Item to Layout
	d->layout->insertWidget(index, item);

	// Add Event
        connect(item, SIGNAL(selected(QSelectableWidget *)),
                        this, SLOT(onItemSelected(QSelectableWidget *)));
}

// =============================================================================
//  NavBarGroup: PUBLIC Methods - Create and Add Item
// =============================================================================
QNavBarItem *QNavBarGroup::addItem (const QString& text) {
        QNavBarItem *item = new QNavBarItem(text);
	addItem(item);
	return(item);
}

QNavBarItem *QNavBarGroup::addItem (const QPixmap& icon, const QString& text) {
        QNavBarItem *item = new QNavBarItem(icon, text);
	addItem(item);
	return(item);
}

QNavBarItem *QNavBarGroup::addItem (const QPixmap& icon, const QString& text, int index) {
        QNavBarItem *item = new QNavBarItem(icon, text);
	addItem(item, index);	
	return(item);
}

// =============================================================================
//  NavBarGroup: PUBLIC Methods
// =============================================================================
bool QNavBarGroup::containsItem (QNavBarItem *item) {
	return(d->listItems.contains(item));
}

QString QNavBarGroup::title() const
{
        return d->labelTitle->text();
}

bool QNavBarGroup::isExpanded() const
{
        return d->isExpanded;
}

bool QNavBarGroup::isStatic() const
{
    return d->isStatic;
}

// =============================================================================
//  NavBarGroup: PUBLIC Set Properties
// =============================================================================
void QNavBarGroup::setTitle (const QString& title) {
	d->labelTitle->setText(title);
}

void QNavBarGroup::setTitleColor (const QColor& color) {
	QPalette palette = d->labelTitle->palette();
	palette.setColor(QPalette::WindowText, color);
	d->labelTitle->setPalette(palette);
}

void QNavBarGroup::setStatic(bool flag)
{
    // Static navbar groups are always expanded
    if (flag)
        expand(true);

    d->isStatic = flag;
}

void QNavBarGroup::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
#if 0
    if (flag)
        d->labelTitle->setPixmap(QPixmap());
    else {
        QPixmap pix(":/icons/z_apple.png");
        d->labelTitle->setPixmap(pix);
    }
#endif
}

void QNavBarGroup::expand(bool expand)
{
    if (d->isExpanded == expand)
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

    // Raise Expanded Event
    emit expanded(this);
}

void QNavBarGroup::onItemSelected(QSelectableWidget *item)
{
    emit selected(this, (QNavBarItem *)item);
}

void QNavBarGroup::onTitleClicked()
{
    if (!d->isStatic)
        expand(!d->isExpanded);
}
