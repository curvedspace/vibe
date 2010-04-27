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

#include "qubeguiglobals.h"
#include "settings.h"

static QubeGui::Settings *m_settings = 0;

namespace QubeGui
{
    Globals::Globals()
    {
    }

    static Settings *Globals::settings()
    {
        return m_settings;
    }
}

static void qubegui_init() __attribute__((constructor));
static void qubegui_fini() __attribute__((destructor));

void qubegui_init()
{
    m_settings = new QubeGui::Settings();
}

void qubegui_fini()
{
    delete m_settings;
}
