/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2001 Waldo Bastian
 * Copyright (c) 2004 Jarosław Staniek
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *    Waldo Bastian <bastian@kde.org>
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Jarosław Staniek <staniek@kde.org>
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

#ifndef VFILESUPPORT_H
#define VFILESUPPORT_H

/**
 * \file vfilesupport.h
 * \brief This file provides portable defines for file support.
 *
 * Use the Vibe_xxx defines instead of the normal C
 * functions and structures.
 */

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>

#include <VibeCore/VGlobal>

#if (defined _LFS64_LARGEFILE) && (defined _LARGEFILE64_SOURCE) && (!defined _GNU_SOURCE)
/*
 * This section provides portable defines for large file support.
 * To use this you must compile your code with _LARGEFILE64_SOURCE
 * defined and use the Vibe_xxx defines instead of the normal
 * C functions and structures.
 *
 * Please note that not every platform supports 64 bit file structures,
 * in that case the normal 32 bit functions will be used.
 *
 * @see http://www.suse.de/~aj/linux_lfs.html
 * @see http://ftp.sas.com/standards/large.file/xopen/x_open.05Mar96.html
 *
 * Vibe makes use of the "Transitional Extensions" since we can not ensure
 * that all modules and libraries used by KDE will be compiled with
 * 64-bit support.
 * (A.3.2.3 Mixed API and Compile Environments within a Single Process)
 */
#  define Vibe_stat		::stat64
#  define Vibe_lstat		::lstat64
#  define Vibe_fstat		::fstat64
#  define Vibe_open		::open64
#  define Vibe_lseek		::lseek64
#  define Vibe_fseek		::fseek64
#  define Vibe_ftell		::ftell64
#  define Vibe_fgetpos		::fgetpos64
#  define Vibe_fsetpos		::fsetpos64
#  define Vibe_readdir		::readdir64
#  define Vibe_sendfile		::sendfile64
#  define Vibe_struct_stat 	struct stat64
#  define Vibe_struct_dirent	struct dirent64
#  define Vibe_rename		::rename
#  define Vibe_mkdir		::mkdir
#else /* !_LFS64_LARGEFILE */
/*
 * This section defines portable defines for standard file support.
 */
#  define Vibe_stat		::stat
#  define Vibe_lstat		::lstat
#  define Vibe_fstat		::fstat
#  define Vibe_open		::open
#  define Vibe_lseek		::lseek
#  define Vibe_fseek		::fseek
#  define Vibe_ftell		::ftell
#  define Vibe_fgetpos		::fgetpos
#  define Vibe_fsetpos		::fsetpos
#  define Vibe_readdir		::readdir
#  define Vibe_sendfile		::sendfile
#  define Vibe_struct_stat 	struct stat
#  define Vibe_struct_dirent	struct dirent
#  define Vibe_rename		::rename
#  define Vibe_mkdir		::mkdir

#ifdef _LFS64_STDIO
#  define Vibe_fopen		::fopen64
#  define Vibe_freopen		::freopen64
/* TODO: define for win32 */
#else
#  define Vibe_fopen		::fopen
#endif
#endif

// Functions without 64-bit version but wrapped for compatibility reasons
#define Vibe_fdopen		::fdopen
#define Vibe_signal		::signal

#include <QFile>

class QString;

namespace Vibe
{
    /** Replacement for ::access() to handle filenames in a platform independent way */
    VIBE_EXPORT int access(const QString &path, int mode);

    /** Replacement for ::chmod() to handle filenames in a platform independent way */
    VIBE_EXPORT int chmod(const QString &path, mode_t mode);

    /** Replacement for ::fopen()/::fopen64() to handle filenames in a platform independent way */
    VIBE_EXPORT FILE *fopen(const QString &pathname, const char *mode);

    /** Replacement for ::lstat()/::lstat64() to handle filenames in a platform independent way */
    VIBE_EXPORT int lstat(const QString &path, Vibe_struct_stat *buf);

    /** Replacement for ::mkdir() to handle pathnames in a platform independent way */
    VIBE_EXPORT int mkdir(const QString &pathname, mode_t mode);

    /** Replacement for ::open()/::open64() to handle filenames in a platform independent way */
    VIBE_EXPORT int open(const QString &pathname, int flags, mode_t mode = 0);

    /** Replacement for ::rename() to handle pathnames in a platform independent way */
    VIBE_EXPORT int rename(const QString &in, const QString &out);

    /** Replacement for ::stat()/::stat64() to handle filenames in a platform independent way */
    VIBE_EXPORT int stat(const QString &path, Vibe_struct_stat *buf);

    /** Replacement for ::utime() to handle filenames in a platform independent way */
    VIBE_EXPORT int utime(const QString &filename, struct utimbuf *buf);

#ifndef Q_WS_WIN
    inline int access(const QString &path, int mode)
    {
        return ::access(QFile::encodeName(path).constData(), mode);
    }

    inline int chmod(const QString &path, mode_t mode)
    {
        return ::chmod(QFile::encodeName(path).constData(), mode);
    }

    inline FILE *fopen(const QString &pathname, const char *mode)
    {
        return Vibe_fopen(QFile::encodeName(pathname).constData(), mode);
    }

    inline int lstat(const QString &path, Vibe_struct_stat *buf)
    {
        return Vibe_lstat(QFile::encodeName(path).constData(), buf);
    }

    inline int mkdir(const QString &pathname, mode_t mode)
    {
        return Vibe_mkdir(QFile::encodeName(pathname).constData(), mode);
    }

    inline int open(const QString &pathname, int flags, mode_t mode)
    {
        return Vibe_open(QFile::encodeName(pathname).constData(), flags, mode);
    }

    inline int rename(const QString &in, const QString &out)
    {
        return Vibe_rename(QFile::encodeName(in).constData(), QFile::encodeName(out).constData());
    }

    inline int stat(const QString &path, Vibe_struct_stat *buf)
    {
        return Vibe_stat(QFile::encodeName(path).constData(), buf);
    }

    inline int utime(const QString &filename, struct utimbuf *buf)
    {
        return ::utime(QFile::encodeName(filename).constData(), buf);
    }
#endif
}

#if defined _WIN32 || defined _WIN64
#  define V_PATH_SEPARATOR ';'
#  define V_DIR_SEPARATOR '\\' // Faster than QDir::separator()
#else
#  ifndef O_BINARY
#    define O_BINARY 0        // For open()
#  endif
#  define V_PATH_SEPARATOR ':'
#  define V_DIR_SEPARATOR '/' // Faster than QDir::separator()
#endif

#endif // VFILESUPPORT_H
