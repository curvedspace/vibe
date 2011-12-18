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

#ifndef VSTANDARDDIRECTORIES_H
#define VSTANDARDDIRECTORIES_H

#include <QtCore/QString>

namespace VStandardDirectories
{
    typedef enum {
        /*
         * Kernel directories.
         */

        /*! Boot files directory (/system/kernel/boot) */
        KernelBootDirectory,

        /*! Device files directory, used by udev (/system/kernel/devices) */
        KernelDevicesDirectory,

        /*! Kernel status files, better known as /proc (/system/kernel/status) */
        KernelStatusDirectory,

        /*! Kernel objects directory, better known as /sys (/system/kernel/objects) */
        KernelObjectsDirectory,

        /*! Kernel modules directory (/system/kernel/modules) */
        KernelModulesDirectory,

        /*
         * System reserved directories.
         */

        /*! System directory (/system) */
        SystemDirectory = 1000,

        /*! System libraries directory (/system/lib) */
        SystemLibrariesDirectory,

        /*! System plugins directory (/system/plugins) */
        SystemPluginsDirectory,

        /*! System binaries directory (/system/bin) */
        SystemBinariesDirectory,

        /*! System bundles directory (/system/bundles) */
        SystemBundlesDirectory,

        /*! System applications directory (/system/apps) */
        SystemApplicationsDirectory,

        /*! System data directory (/system/data) */
        SystemDataDirectory,

        /*! System fonts directory (/system/data/fonts) */
        SystemFontsDirectory,

        /*! System manual directory (/system/data/man) */
        SystemManDirectory,

        /*! System information directory (/system/data/info) */
        SystemInfoDirectory,

        /*! System translations directory (/system/data/translations) */
        SystemTranslationsDirectory,

        /*! System themes directory (/system/data/themes) */
        SystemThemesDirectory,

        /*! System documentation directory (/system/doc) */
        SystemDocumentationDirectory,

        /*
         * Common directories (shared among all users).
         */

        /*! Common directory (/common) */
        CommonDirectory = 2000,

        /*! Common settings directory (/common/settings) */
        CommonSettingsDirectory,

        /*! Common libraries directory (/common/lib) */
        CommonLibrariesDirectory,

        /*! Common plugins directory (/common/plugins) */
        CommonPluginsDirectory,

        /*! Common binaries directory (/common/bin) */
        CommonBinariesDirectory,

        /*! Common bundles directory (/common/bundles) */
        CommonBundlesDirectory,

        /*! Common applications directory (/common/apps) */
        CommonApplicationsDirectory,

        /*! Common development directory (/common/development) */
        CommonDevelopmentDirectory,

        /*! Common headers directory (/common/development/headers) */
        CommonHeadersDirectory,

        /*! Common data directory (/common/data) */
        CommonDataDirectory,

        /*! Common fonts directory (/common/data/fonts) */
        CommonFontsDirectory,

        /*! Common manual directory (/common/data/man) */
        CommonManDirectory,

        /*! Common information directory (/common/data/info) */
        CommonInfoDirectory,

        /*! Common translations directory (/common/data/translations) */
        CommonTranslationsDirectory,

        /*! Common themes directory (/common/data/themes) */
        CommonThemesDirectory,

        /*! Common documentation directory (/common/doc) */
        CommonDocumentationDirectory,

        /*! Common cache directory (/common/cache) */
        CommonCacheDirectory,

        /*! Common variable directory (/common/var) */
        CommonVariableDirectory,

        /*! Common spool directory (/common/var/spool) */
        CommonSpoolDirectory,

        /*! Common logs directory (/common/var/log) */
        CommonLogDirectory,

        /*! Common temporary directory (/common/var/tmp) */
        CommonTemporaryDirectory,

        /*! Common directory for run files (/common/var/run) */
        CommonRunDirectory,

        /*! Common directory for lock files (/common/var/lock) */
        CommonLockDirectory,

        /*
         * User directories, these are interpreted in the context
         * of the user.
         */

        /*! Users directory (/users) */
        UsersDirectory = 3000,

        /*! User's home directory (/users/$USER) */
        UserHomeDirectory,

        /*! User's desktop directory (/users/$USER/.desktop) */
        UserDesktopDirectory,

        /*! User's downloads directory (/users/$USER/downloads) */
        UserDownloadsDirectory,

        /*! User's documents directory (/users/$USER/documents) */
        UserDocumentsDirectory,

        /*! User's music directory (/users/$USER/music) */
        UserMusicDirectory,

        /*! User's movies directory (/users/$USER/movies) */
        UserMoviesDirectory,

        /*! User's pictures directory (/users/$USER/pictures) */
        UserPicturesDirectory,

        /*! User's temporary directory (/users/$USER/.tmp) */
        UserTemporaryDirectory,

        /*! User's settings directory (/users/$USER/.settings) */
        UserSettingsDirectory,

        /*! User's bundles directory (/users/$USER/.bundles) */
        UserBundlesDirectory,

        /*! User's bundles directory (/users/$USER/.apps) */
        UserApplicationsDirectory,

        /*! User's data directory (/users/$USER/.data) */
        UserDataDirectory,

        /*! User's themes directory (/users/$USER/.data/themes) */
        UserThemesDirectory,

        /*! User's fonts directory (/users/$USER/.data/fonts) */
        UserFontsDirectory,

        /*! User's cache directory (/users/$USER/.cache) */
        UserCacheDirectory
    } DirectoryWhich;

    /*!
     *
     */
    QString findDirectory(DirectoryWhich which);
}

#endif // VSTANDARDDIRECTORIES_H
