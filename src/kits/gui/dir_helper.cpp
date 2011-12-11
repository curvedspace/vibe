/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (C) 2009 Dzimi Mve Alexandre
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Dzimi Mve Alexandre <dzimiwine@gmail.com>
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

#include <QStringList>
#include <QMap>
#include <QDir>
#include <QObject>
#include <QSysInfo>

#include "dir_helper.h"

#if defined(Q_OS_WIN) && !defined(Q_OS_WINCE)
#include <QSettings>
#include <QLibrary>
typedef BOOL (WINAPI *PtrGetVolumeInformation)(LPCTSTR lpRootPathName,
                                               LPTSTR lpVolumeNameBuffer,
                                               DWORD nVolumeNameSize,
                                               LPDWORD lpVolumeSerialNumber,
                                               LPDWORD lpMaximumComponentLength,
                                               LPDWORD lpFileSystemFlags,
                                               LPTSTR lpFileSystemNameBuffer,
                                               DWORD nFileSystemNameSize);

static PtrGetVolumeInformation pGetVolumeInformation = 0;
#endif

QMap< DirHelper::SpecialFolder, QString > DirHelper::m_specialFolderMap = QMap< DirHelper::SpecialFolder, QString >();
bool DirHelper::m_initialized = false;

void DirHelper::init()
{
    QString homePath = QDir::homePath(), folder;

    m_specialFolderMap.insert(DirHelper::Home, homePath);
#if defined(Q_OS_WIN) && !defined(Q_OS_WINCE)
    QMap< SpecialFolder, QString > registerKeys;

    registerKeys.insert(DirHelper::Desktop, "Desktop");
    registerKeys.insert(DirHelper::Documents, "Personal");
    registerKeys.insert(DirHelper::Music, "My Music");
    registerKeys.insert(DirHelper::Pictures, "My Pictures");
    registerKeys.insert(DirHelper::Video, "My Video");

    QSettings settings(QSettings::UserScope, "Microsoft", "Windows");

    foreach(DirHelper::SpecialFolder key, registerKeys.keys()) {
        folder = registerKeys.value(key);
        folder = settings.value(QString("CurrentVersion/Explorer/User Shell Folders/") + folder).toString().replace("%USERPROFILE%", homePath);
        m_specialFolderMap.insert(key, folder);
    }
#else
    m_specialFolderMap.insert(DirHelper::Desktop, homePath + QDir::separator() + "Desktop");
    m_specialFolderMap.insert(DirHelper::Documents, homePath + QDir::separator() + "Documents");
    m_specialFolderMap.insert(DirHelper::Music, homePath + QDir::separator() + "Music");
    m_specialFolderMap.insert(DirHelper::Pictures, homePath + QDir::separator() + "Pictures");
#if defined(Q_OS_MAC)
    m_specialFolderMap.insert(DirHelper::Video, homePath + QDir::separator() + "Movies");
#else
    m_specialFolderMap.insert(DirHelper::Video, homePath + QDir::separator() + "Videos");
#endif // Q_OS_MAC
#endif // Q_OS_WIN

    m_initialized = true;
}

QStringList DirHelper::splitPath(const QString &path)
{
    QString pathCopy = QDir::toNativeSeparators(path);
    QString sep = QDir::separator();

#if defined(Q_OS_WIN) && !defined(Q_OS_WINCE)
    if (pathCopy == QLatin1String("\\") || pathCopy == QLatin1String("\\\\"))
        return QStringList(pathCopy);
    QString doubleSlash(QLatin1String("\\\\"));
    if (pathCopy.startsWith(doubleSlash))
        pathCopy = pathCopy.mid(2);
    else
        doubleSlash.clear();
#endif

    QRegExp re(QLatin1String("[") + QRegExp::escape(sep) + QLatin1String("]"));
    QStringList parts = pathCopy.split(re);

#if defined(Q_OS_WIN) && !defined(Q_OS_WINCE)
    if (!doubleSlash.isEmpty())
        parts[0].prepend(doubleSlash);
#else
    if (pathCopy[0] == sep[0]) // read the "/" at the beginning as the split removed it
        parts[0] = QDir::fromNativeSeparators(QString(sep[0]));
#endif
    parts.removeAll("");
    return parts;
}

QString DirHelper::setupPath(const QStringList &list, int index)
{
    QString str;
    int startIndex = 0;

    if (list.isEmpty())
        return QString();
    str = list[0];
    startIndex = 1;
    for (int i = startIndex; i <= index; i++) {
        str += QDir::separator() + list[i];
    }
    return str;
}

QString DirHelper::specialFolder(DirHelper::SpecialFolder folder)
{
    if (folder < DirHelper::Home || folder > DirHelper::Video) {
        qWarning("DirHelper::specialDirPath(): Invalid special directory.");
        return QString();
    }
    if (!m_initialized)
        init();
    return m_specialFolderMap[folder];
}
QString DirHelper::myComputer()
{
#if defined(Q_OS_WIN)
    if (QSysInfo::windowsVersion() < QSysInfo::WV_VISTA)
        return QObject::tr("My Computer");
    else
        return QObject::tr("Computer");
#else
    return QObject::tr("Computer");
#endif
}

QString DirHelper::driveLabel(const QString &drive)
{
#if defined(Q_OS_WIN) && !(Q_OS_WINCE)
    QString name;
    TCHAR volumeName[MAX_PATH + 1] = { 0 };
    TCHAR fileSystemName[MAX_PATH + 1] = { 0 };
    DWORD serialNumber = 0;
    DWORD maxComponentLen = 0;
    DWORD fileSystemFlags = 0;
    QString  d = QDir::toNativeSeparators(drive) + QDir::separator();
    LPCTSTR dr = (const TCHAR *) QStringToTCHAR(d);
    static bool tried = false;

    if (!tried) {
        tried = true;
        QLibrary lib(QString::fromAscii("Kernel32"));

        if (!lib.isLoaded()) {
            lib.load();
        }
        pGetVolumeInformation = (PtrGetVolumeInformation)lib.resolve("GetVolumeInformationW");
    }
    if (pGetVolumeInformation) {
        pGetVolumeInformation(dr,
                              volumeName,
                              MAX_PATH + 1,
                              &serialNumber,
                              &maxComponentLen,
                              &fileSystemFlags,
                              fileSystemName,
                              MAX_PATH + 1);
        name = TCHARToQString(volumeName);
    }
    d = d.remove(QDir::separator());
    if (name.isEmpty())
        name = d;
    else
        name = name + QString(" (%1)").arg(d);

    return name;
#else
    return QObject::tr("File System (%1)").arg(drive);
#endif
}
