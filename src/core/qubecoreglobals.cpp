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

#include "qubecoreglobals.h"

namespace QubeCore
{
    Globals::Globals()
    {
    }
}

extern "C" {
    static void qubecore_init() __attribute__ ((constructor));
    static void qubecore_fini() __attribute__ ((destructor));

    void qubecore_init()
    {
    }

    void qubecore_fini()
    {
    }
}
