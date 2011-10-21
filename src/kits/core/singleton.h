/****************************************************************************
 *
 * Copyright (c) 2010 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * All rights reserved.
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

#ifndef SINGLETON_H
#define SINGLETON_H

#include <QubeCore/Global>

namespace QubeCore
{
    template<typename T>
    class QUBESHARED_EXPORT Singleton
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

#endif // SINGLETON_H
