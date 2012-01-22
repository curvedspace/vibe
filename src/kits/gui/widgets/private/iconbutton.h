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

#ifndef PRIVATE_ICONBUTTON_H
#define PRIVATE_ICONBUTTON_H

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

#include <QAbstractButton>

namespace VPrivate
{
    class IconButton : public QAbstractButton
    {
        Q_OBJECT
        Q_PROPERTY(float iconOpacity READ iconOpacity WRITE setIconOpacity)
        Q_PROPERTY(bool autoHide READ hasAutoHide WRITE setAutoHide)
        Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap)
    public:
        explicit IconButton(QWidget *parent = 0);

        void paintEvent(QPaintEvent *e);

        float iconOpacity() const;
        void setIconOpacity(float value);

        bool hasAutoHide() const;
        void setAutoHide(bool value);

        QPixmap pixmap() const;
        void setPixmap(const QPixmap &pixmap);

        void animateShow(bool visible);

    private:
        float m_iconOpacity;
        bool m_autoHide;
        QPixmap m_pixmap;
    };
}

#endif // PRIVATE_ICONBUTTON_H
