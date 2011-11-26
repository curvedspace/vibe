/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
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

#ifndef VIBE_INTERFACEMACROS_H
#define VIBE_INTERFACEMACROS_H

#define VIBE_DECLARE_UI(Class) \
	inline Class##Ui *ui_func() { return reinterpret_cast<Class##Ui *>(ui_ptr); } \
	inline const Class##Ui *ui_func() const { return reinterpret_cast<const Class##Ui *>(ui_ptr); } \
	friend class Class##Ui;

#define VIBE_UI(Class) Class##Ui *ui = ui_func()
#define VIBE_WIDGET(Class) Class *w = w_func()
#define VIBE_WIDGET_CAST inline QWidget *toWidget() { return this; } \
	inline const QWidget * toWidget() const { return this; }

#endif // VIBE_INTERFACEMACROS_H
