/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Vibe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Vibe.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QDebug>

#include "settings/settingsschemaloader.h"

static void usage()
{
    qWarning() << "Usage: compile-settings-schemas [schema file]";
    exit(1);
}

int main(int argc, char *argv[])
{
    // Check arguments
    if (argc != 2)
        usage();
    if (!argv[1])
        usage();

    // Parse the schema file
    VPrivate::SettingsSchemaLoader loader;
    if (!loader.parseSchemaSource(argv[1]))
        return 1;
    if (!loader.appendParsedSchema())
        return 1;

    return 0;
}
