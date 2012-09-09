/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPL-ONLY$
 *
 * This file may be used under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation and
 * appearing in the file LICENSE.LGPL included in the packaging of
 * this file, either version 2.1 of the License, or (at your option) any
 * later version.  Please review the following information to ensure the
 * GNU Lesser General Public License version 2.1 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 * If you have questions regarding the use of this file, please contact
 * us via http://www.maui-project.org/.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef VSETTINGSTYPES_H
#define VSETTINGSTYPES_H

#include <QVariant>
#include <glib.h>

QVariant::Type vsettings_types_convert(const GVariantType *gtype);
GVariant *vsettings_types_collect(const GVariantType *gtype, const void *argument);
void vsettings_types_unpack(GVariant *value, void *argument);
QVariant vsettings_types_to_qvariant(GVariant *value);

#endif // VSETTINGSTYPES_H
