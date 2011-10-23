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

#ifndef QUBE_SINGLETON_H
#define QUBE_SINGLETON_H

namespace Qube
{
    namespace Core
    {
        template<typename T>
        class Singleton
        {
        public:
            static T *instance() {
                if (!m_instance)
                    m_instance = new T();
                return m_instance;
            }

        protected:
            Singleton() {}
            Singleton(const Singleton<T> &);
            virtual ~Singleton() {}

            Singleton<T> &operator=(const Singleton<T> &);

        private:
            static T *m_instance;
        };

        template<typename T> T *Singleton<T>::m_instance = 0;
    }
}

#endif // QUBE_SINGLETON_H
