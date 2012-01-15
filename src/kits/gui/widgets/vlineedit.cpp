/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1
 * as published by the Free Software Foundation.
 *
 * Vibe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Vibe.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QEvent>

#include "vlineedit.h"
#include "vlineedit_p.h"

using namespace VPrivate;

/*
 * VLineEditPrivate
 */

VLineEditPrivate::VLineEditPrivate(VLineEdit *parent) :
    q_ptr(parent),
    drawingHintText(false)
{
    for (int i = 0; i < 2; i++) {
        menu[i] = 0;
        menuTabFocusTrigger[i] = false;
        iconButton[i] = new IconButton(parent);
        iconButton[i]->hide();
        iconButton[i]->setAutoHide(false);
        iconEnabled[i] = false;
    }
}

bool VLineEditPrivate::eventFilter(QObject *obj, QEvent *event)
{
    Q_Q(VLineEdit);

    int buttonIndex = -1;

    for (int i = 0; i < 2; i++) {
        if (obj == iconButton[i]) {
            buttonIndex = i;
            break;
        }
    }

    if (buttonIndex == -1)
        return QObject::eventFilter(obj, event);

    if (event->type() == QEvent::FocusIn) {
        if (menuTabFocusTrigger[buttonIndex] && menu[buttonIndex]) {
            q->setFocus();
            menu[buttonIndex]->exec(iconButton[buttonIndex]->mapToGlobal(
                                        iconButton[buttonIndex]->rect().center()));
            return true;
        }
    }

    return QObject::eventFilter(obj, event);
}

void VLineEditPrivate::updateMargins()
{
    Q_Q(VLineEdit);

    bool leftToRight = (q->layoutDirection() == Qt::LeftToRight);

    VLineEdit::Side realLeft = (leftToRight ? VLineEdit::LeftSide : VLineEdit::RightSide);
    VLineEdit::Side realRight = (leftToRight ? VLineEdit::RightSide : VLineEdit::LeftSide);

    int leftMargin = iconButton[realLeft]->pixmap().width() + 8;
    int rightMargin = iconButton[realRight]->pixmap().width() + 8;

    QMargins margins((iconEnabled[realLeft] ? leftMargin : 0), 0,
                     (iconEnabled[realRight] ? rightMargin : 0), 0);
    q->setTextMargins(margins);
}

void VLineEditPrivate::updateButtonPositions()
{
    Q_Q(VLineEdit);

    QRect contentRect(q->rect());

    for (int i = 0; i < 2; i++) {
        VLineEdit::Side iconSide = (VLineEdit::Side)i;
        if (q->layoutDirection() == Qt::RightToLeft)
            iconSide = (iconSide == VLineEdit::LeftSide ?
                        VLineEdit::RightSide : VLineEdit::LeftSide);

        const int iconOffset = q->textMargins().right() + 4;
        if (iconSide == VLineEdit::RightSide)
            iconButton[i]->setGeometry(contentRect.adjusted(q->width() - iconOffset, 0, 0, 0));
        else
            iconButton[i]->setGeometry(contentRect.adjusted(0, 0, -q->width() + iconOffset, 0));
    }
}

/*
 * VLineEdit
 */

VLineEdit::VLineEdit(QWidget *parent) :
    QLineEdit(parent),
    d_ptr(new VLineEditPrivate(this))
{
    Q_D(VLineEdit);

    ensurePolished();
    d->updateMargins();

    connect(this, SIGNAL(textChanged(QString)),
            this, SLOT(slotCheckButtons(QString)));
    connect(d->iconButton[LeftSide], SIGNAL(clicked()),
            this, SLOT(slotIconClicked()));
    connect(d->iconButton[RightSide], SIGNAL(clicked()),
            this, SLOT(slotIconClicked()));
}

bool VLineEdit::isButtonVisible(VLineEdit::Side side) const
{
    Q_D(const VLineEdit);
    return d->iconEnabled[side];
}

void VLineEdit::setButtonVisible(Side side, bool visible)
{
    Q_D(VLineEdit);

    d->iconButton[side]->setVisible(visible);
    d->iconEnabled[side] = visible;
    d->updateMargins();
}

void VLineEdit::setButtonToolTip(Side side, const QString &toolTip)
{
    Q_D(VLineEdit);

    d->iconButton[side]->setToolTip(toolTip);
}

void VLineEdit::setButtonFocusPolicy(Side side, Qt::FocusPolicy policy)
{
    Q_D(VLineEdit);

    d->iconButton[side]->setFocusPolicy(policy);
}

bool VLineEdit::hasMenuTabFocusTrigger(Side side) const
{
    Q_D(const VLineEdit);

    return d->menuTabFocusTrigger[side];
}

void VLineEdit::setMenuTabFocusTrigger(Side side, bool value)
{
    Q_D(VLineEdit);

    if (d->menuTabFocusTrigger[side] == value)
        return;
    d->menuTabFocusTrigger[side] = value;
    d->iconButton[side]->setFocusPolicy(value ? Qt::TabFocus : Qt::NoFocus);
}

bool VLineEdit::hasAutoHideButton(Side side) const
{
    Q_D(const VLineEdit);

    return d->iconButton[side]->hasAutoHide();
}

void VLineEdit::setAutoHideButton(Side side, bool value)
{
    Q_D(VLineEdit);

    d->iconButton[side]->setAutoHide(value);
    if (value)
        d->iconButton[side]->setIconOpacity(text().isEmpty() ? 0.0 : 1.0);
    else
        d->iconButton[side]->setIconOpacity(1.0);
}

void VLineEdit::slotCheckButtons(const QString &text)
{
    Q_D(VLineEdit);

    if (d->oldText.isEmpty() || text.isEmpty()) {
        for (int i = 0; i < 2; i++) {
            if (d->iconButton[i]->hasAutoHide())
                d->iconButton[i]->animateShow(!text.isEmpty());
        }

        d->oldText = text;
    }
}

void VLineEdit::resizeEvent(QResizeEvent *)
{
    Q_D(VLineEdit);
    d->updateButtonPositions();
}

void VLineEdit::slotIconClicked()
{
    Q_D(VLineEdit);

    IconButton *button = qobject_cast<IconButton *>(sender());
    int index = -1;

    for (int i = 0; i < 2; i++) {
        if (d->iconButton[i] == button)
            index = i;
    }
    if (index == -1)
        return;
    if (d->menu[index])
        d->menu[index]->exec(QCursor::pos());
    else {
        emit buttonClicked((VLineEdit::Side)index);
        if (index == VLineEdit::LeftSide)
            emit leftButtonClicked();
        else
            emit rightButtonClicked();
    }
}
