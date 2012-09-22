/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
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

#include <QDir>

#include "vstandarddirectories.h"
#include "cmakedirs.h"

namespace VStandardDirectories
{
    QString findDirectory(DirectoryWhich which)
    {
        switch (which) {
            case KernelDirectory:
                return "/system/kernel";
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
            case KernelFirmwareDirectory:
                return "/system/kernel/firmware";

            case SystemDirectory:
                return INSTALL_PREFIX;
            case SystemLibrariesDirectory:
                return INSTALL_LIBDIR;
            case SystemPluginsDirectory:
                return INSTALL_PLUGINSDIR;
            case SystemImportsDirectory:
                return INSTALL_IMPORTSDIR;
            case SystemBinariesDirectory:
                return INSTALL_BINDIR;
            case SystemBundlesDirectory:
                return INSTALL_BUNDLESDIR;
            case SystemApplicationsDirectory:
                return INSTALL_APPSDIR;
            case SystemProgramsDirectory:
                return INSTALL_PROGSDIR;
            case SystemServersDirectory:
                return INSTALL_SERVERSDIR;
            case SystemDevelopmentDirectory:
                return INSTALL_DEVELOPDIR;
            case SystemHeadersDirectory:
                return INSTALL_INCLUDEDIR;
            case SystemDataDirectory:
                return INSTALL_DATADIR;
            case SystemFontsDirectory:
                return QString("%1/fonts").arg(INSTALL_DATAROOTDIR);
            case SystemManDirectory:
                return INSTALL_MANDIR;
            case SystemInfoDirectory:
                return INSTALL_INFODIR;
            case SystemTranslationsDirectory:
                return QString("%1/translations").arg(INSTALL_DATAROOTDIR);
            case SystemThemesDirectory:
                return QString("%1/themes").arg(INSTALL_DATAROOTDIR);
            case SystemDocumentationDirectory:
                return INSTALL_DOCDIR;

            case CommonDirectory:
                return "/common";
            case CommonSettingsDirectory:
                return "/common/settings";
            case CommonLibrariesDirectory:
                return "/common/lib";
            case CommonPluginsDirectory:
                return "/common/plugins";
            case CommonImportsDirectory:
                return "/common/imports";
            case CommonBinariesDirectory:
                return "/common/bin";
            case CommonBundlesDirectory:
                return "/common/bundles";
            case CommonApplicationsDirectory:
                return "/common/apps";
            case CommonProgramsDirectory:
                return "/common/progs";
            case CommonServersDirectory:
                return "/common/servers";
            case CommonDevelopmentDirectory:
                return "/common/develop";
            case CommonHeadersDirectory:
                return "/common/develop/headers";
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
                return "/common/data/doc";
            case CommonVariableDirectory:
                return "/common/var";
            case CommonCacheDirectory:
                return "/common/cache";
            case CommonSpoolDirectory:
                return "/common/var/spool";
            case CommonLogDirectory:
                return "/common/var/log";
            case CommonTemporaryDirectory:
                return "/common/var/tmp";
            case CommonRunDirectory:
                return "/common/run";
            case CommonLockDirectory:
                return "/common/run/lock";

            case UsersDirectory:
                return "/users";
            case UserHomeDirectory:
                return QDir::homePath();
            case UserSettingsDirectory:
                return QDir::homePath() + "/.config";
            case UserBundlesDirectory:
                return QDir::homePath() + "/.local/bundles";
            case UserApplicationsDirectory:
                return QDir::homePath() + "/.local/apps";
            case UserDesktopDirectory:
                return QDir::homePath() + "/Desktop";
            case UserDownloadsDirectory:
                return QDir::homePath() + "/Downloads";
            case UserDocumentsDirectory:
                return QDir::homePath() + "/Documents";
            case UserMusicDirectory:
                return QDir::homePath() + "/Music";
            case UserMoviesDirectory:
                return QDir::homePath() + "/Movies";
            case UserPicturesDirectory:
                return QDir::homePath() + "/Pictures";
            case UserTemporaryDirectory:
                return QDir::homePath() + "/.local/tmp";
            case UserDataDirectory:
                return QDir::homePath() + "/.local/data";
            case UserThemesDirectory:
                return QDir::homePath() + "/.local/data/themes";
            case UserFontsDirectory:
                return QDir::homePath() + "/.local/data/fonts";
            case UserCacheDirectory:
                return QDir::homePath() + "/.cache";

            case VolumesDirectory:
                return "/volumes";
            default:
                break;
        }

        return QString();
    }
}
