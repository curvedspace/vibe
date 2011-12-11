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

#include <QtGui/QDesktopServices>

#include "vfinddirectory.h"

namespace VStorage
{
    QString findDirectory(DirectoryWhich which)
    {
        switch (which) {
            case KernelBootDirectory:
                return "/system/kernel/boot";
            case KernelDevicesDirectory:
                return "/system/kernel/devices";
            case KernelStatusDirectory:
                return "/system/kernel/status";
            case KernelObjectsDirectory:
                return "/system/kernel/objects";
            case KernelModulesDirectory:
                return "/system/kernel/modules";

            case SystemDirectory:
                return "/system";
            case SystemLibrariesDirectory:
                return "/system/lib";
            case SystemPluginsDirectory:
                return "/system/plugins";
            case SystemBinariesDirectory:
                return "/system/bin";
            case SystemBundlesDirectory:
                return "/system/bundles";
            case SystemApplicationsDirectory:
                return "/system/apps";
            case SystemDataDirectory:
                return "/system/data";
            case SystemFontsDirectory:
                return "/system/data/fonts";
            case SystemManDirectory:
                return "/system/data/man";
            case SystemInfoDirectory:
                return "/system/data/info";
            case SystemTranslationsDirectory:
                return "/system/data/translations";
            case SystemThemesDirectory:
                return "/system/data/themes";
            case SystemDocumentationDirectory:
                return "/system/doc";

            case CommonDirectory:
                return "/common";
            case CommonSettingsDirectory:
                return "/common/settings";
            case CommonLibrariesDirectory:
                return "/common/lib";
            case CommonPluginsDirectory:
                return "/common/plugins";
            case CommonBinariesDirectory:
                return "/common/bin";
            case CommonBundlesDirectory:
                return "/common/bundles";
            case CommonApplicationsDirectory:
                return "/common/apps";
            case CommonDataDirectory:
                return "/common/data";
            case CommonFontsDirectory:
                return "/common/data/fonts";
            case CommonManDirectory:
                return "/common/data/man";
            case CommonInfoDirectory:
                return "/common/data/info";
            case CommonTranslationsDirectory:
                return "/common/data/translations";
            case CommonThemesDirectory:
                return "/common/data/themes";
            case CommonDocumentationDirectory:
                return "/common/doc";
            case CommonCacheDirectory:
                return "/common/cache";
            case CommonVariableDirectory:
                return "/common/var";
            case CommonSpoolDirectory:
                return "/common/var/spool";
            case CommonLogDirectory:
                return "/common/var/log";
            case CommonTemporaryDirectory:
                return "/common/var/tmp";
            case CommonRunDirectory:
                return "/common/var/run";
            case CommonLockDirectory:
                return "/common/var/run/lock";

            case UsersDirectory:
                return "/users";
            case UserHomeDirectory:
                return QDesktopServices::storageLocation(QDesktopServices::HomeLocation);
            case UserSettingsDirectory: {
                QString path(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
                return path + "/.settings";
            }
            case UserBundlesDirectory: {
                QString path(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
                return path + "/.bundles";
            }
            case UserApplicationsDirectory: {
                QString path(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
                return path + "/.apps";
            }
            case UserDesktopDirectory:
                return QDesktopServices::storageLocation(QDesktopServices::DesktopLocation);
            case UserDocumentsDirectory:
                return QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
            case UserMusicDirectory:
                return QDesktopServices::storageLocation(QDesktopServices::MusicLocation);
            case UserMoviesDirectory:
                return QDesktopServices::storageLocation(QDesktopServices::MoviesLocation);
            case UserPicturesDirectory:
                return QDesktopServices::storageLocation(QDesktopServices::PicturesLocation);
            case UserTemporaryDirectory:
                return QDesktopServices::storageLocation(QDesktopServices::TempLocation);
            case UserDataDirectory: {
                QString path(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
                return path + "/.data";
            }
            case UserThemesDirectory: {
                QString path(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
                return path + "/.data/themes";
            }
            case UserFontsDirectory: {
                QString path(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
                return path + "/.data/fonts";
            }
            case UserCacheDirectory: {
                QString path(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
                return path + "/.cache";
            }
            default:
                break;
        }
    }
}

int find_directory(VStorage::DirectoryWhich which, char **name)
{
    /* Sanity check */
    if (name == NULL)
        return -1;

    /* Find directory or return NULL */
    QString location = VStorage::findDirectory(which);
    if (!location.isEmpty())
        *name = strdup(location.toLocal8Bit());
    else
        *name = NULL;

    return 0;
}
