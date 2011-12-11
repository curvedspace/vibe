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

#ifndef VSELECTABLEWIDGET_H
#define VSELECTABLEWIDGET_H

#include <QtGui/QWidget>

#include <VibeCore/VGlobal>

class VSelectableWidgetPrivate;

class VIBE_EXPORT VSelectableWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool isSelected READ isSelected WRITE select)
    Q_DECLARE_PRIVATE(VSelectableWidget)
public:
    explicit VSelectableWidget(QWidget *parent = 0);

    bool isSelected() const;

signals:
    void selected();
    void selected(VSelectableWidget *widget);

public slots:
    void select();
    void select(bool isSelected);
    void unselect();

protected:
    void mouseReleaseEvent(QMouseEvent *event);

private:
    VSelectableWidgetPrivate *const d_ptr;
};

#endif // VSELECTABLEWIDGET_H
