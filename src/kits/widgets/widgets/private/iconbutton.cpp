/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#include <QPainter>
#include <QPropertyAnimation>

#include "iconbutton.h"

namespace VPrivate
{
    const int kFadeTime = 160;

    IconButton::IconButton(QWidget *parent) :
        QAbstractButton(parent),
        m_autoHide(false)
    {
        setCursor(Qt::ArrowCursor);
        setFocusPolicy(Qt::NoFocus);
    }

    void IconButton::paintEvent(QPaintEvent *e)
    {
        Q_UNUSED(e);

        QPainter painter(this);

        QIcon::Mode state = QIcon::Disabled;
        if (isEnabled())
            state = isDown() ? QIcon::Active : QIcon::Normal;
        QRect pixmapRect = QRect(0, 0, m_pixmap.width(), m_pixmap.height());
        pixmapRect.moveCenter(rect().center());

        if (m_autoHide)
            painter.setOpacity(m_iconOpacity);

        painter.drawPixmap(pixmapRect, m_pixmap);
    }

    float IconButton::iconOpacity() const
    {
        return m_iconOpacity;
    }

    void IconButton::setIconOpacity(float value)
    {
        m_iconOpacity = value;
    }

    bool IconButton::hasAutoHide() const
    {
        return m_autoHide;
    }

    void IconButton::setAutoHide(bool value)
    {
        m_autoHide = value;
    }

    QPixmap IconButton::pixmap() const
    {
        return m_pixmap;
    }

    void IconButton::setPixmap(const QPixmap &pixmap)
    {
        m_pixmap = pixmap;
        update();
    }

    void IconButton::animateShow(bool visible)
    {
        QPropertyAnimation *animation = new QPropertyAnimation(this, "iconOpacity");
        animation->setDuration(kFadeTime);
        animation->setEndValue(visible ? 1.0 : 0);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

#include "moc_iconbutton.cpp"
