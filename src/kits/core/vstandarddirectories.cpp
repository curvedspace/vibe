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

#include <QDir>

#include "vstandarddirectories.h"

namespace VStandardDirectories
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
            case CommonDevelopmentDirectory:
                return "/common/development";
            case CommonHeadersDirectory:
                return "/common/development/headers";
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
                return QDir::homePath();
            case UserSettingsDirectory:
                return QDir::homePath() + "/.settings";
            case UserBundlesDirectory:
                return QDir::homePath() + "/.bundles";
            case UserApplicationsDirectory:
                return QDir::homePath() + "/.apps";
            case UserDesktopDirectory:
                return QDir::homePath() + "/.desktop";
            case UserDocumentsDirectory:
                return QDir::homePath() + "/documents";
            case UserMusicDirectory:
                return QDir::homePath() + "/music";
            case UserMoviesDirectory:
                return QDir::homePath() + "/movies";
            case UserPicturesDirectory:
                return QDir::homePath() + "/pictures";
            case UserTemporaryDirectory:
                return QDir::homePath() + "/.tmp";
            case UserDataDirectory:
                return QDir::homePath() + "/.data";
            case UserThemesDirectory:
                return QDir::homePath() + "/.data/themes";
            case UserFontsDirectory:
                return QDir::homePath() + "/.data/fonts";
            case UserCacheDirectory:
                return QDir::homePath() + "/.data/cache";
            default:
                break;
        }
    }
}
