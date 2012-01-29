/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
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

#ifndef TOOLTIPAREA_H
#define TOOLTIPAREA_H

#include <QTimer>
#include <QQuickItem>

class TooltipArea : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(bool containsMouse READ containsMouse NOTIFY containsMouseChanged)

public:
    explicit TooltipArea(QQuickItem *parent = 0);

    void setText(const QString &t);
    QString text() const {
        return m_text;
    }
    bool containsMouse() const {
        return m_containsMouse;
    }
    void showToolTip(const QString &str) const;
    void hoverEnterEvent(QHoverEvent *event);
    void hoverLeaveEvent(QHoverEvent *event);

public slots:
    void timeout();

signals:
    void textChanged();
    void containsMouseChanged();

private:

    QTimer m_tiptimer;
    QString m_text;
    bool m_containsMouse;
};

#endif // TOOLTIPAREA_H
