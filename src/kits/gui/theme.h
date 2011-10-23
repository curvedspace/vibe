/****************************************************************************
 * This file is part of Qube.
 *
 * Copyright (c) 2010-2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Qube is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Qube is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Qube.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef THEME_H
#define THEME_H

#include <QtCore/QSettings>
#include <QtCore/QString>

namespace Qube
{
    namespace Gui
    {
        class Theme
        {
        public:
            Theme(const QString &name);
            ~Theme();

        private:
            QSettings *m_theme;
        };
    }
}

#endif // THEME_H
