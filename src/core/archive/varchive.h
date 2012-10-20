/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2000-2005 David Faure
 * Copyright (c) 2003 Leo Savernik
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    David Faure <faure@kde.org>
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Leo Savernik <l.savernik@aon.at>
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

#ifndef VARCHIVE_H
#define VARCHIVE_H

#include <sys/stat.h>
#include <sys/types.h>

#include <QDate>
#include <QString>
#include <QStringList>
#include <QHash>

#include <VibeCore/VibeCoreExport>

class VArchiveDirectory;
class VArchiveFile;

class VArchivePrivate;

class VIBECORE_EXPORT VArchive
{
public:
    explicit VArchive(const QString &fileName);
    VArchive(QIODevice *dev);

    ~VArchive();

    bool open(QIODevice::OpenMode mode);
    bool close();

    bool isOpen() const;

    QIODevice::OpenMode mode() const;
    QIODevice *device() const;

    QString fileName() const;

    const VArchiveDirectory *directory() const;

    bool addLocalFile(const QString &fileName, const QString &destName);

    bool addLocalDirectory(const QString &path, const QString &destName);

    enum { UnknownTime = static_cast<time_t>(-1) };

    bool writeDir(const QString &name, const QString &user, const QString &group,
                  mode_t perm = 040755, time_t atime = UnknownTime,
                  time_t mtime = UnknownTime, time_t ctime = UnknownTime);
    bool writeSymLink(const QString &name, const QString &target,
                      const QString &user, const QString &group,
                      mode_t perm = 0120755, time_t atime = UnknownTime,
                      time_t mtime = UnknownTime, time_t ctime = UnknownTime);
    bool writeFile(const QString &name, const QString &user, const QString &group,
                   const char *data, qint64 size,
                   mode_t perm = 0100644, time_t atime = UnknownTime,
                   time_t mtime = UnknownTime, time_t ctime = UnknownTime);

    bool prepareWriting(const QString &name, const QString &user,
                        const QString &group, qint64 size,
                        mode_t perm = 0100644, time_t atime = UnknownTime,
                        time_t mtime = UnknownTime, time_t ctime = UnknownTime);
    bool finishWriting(qint64 size);

private:
    Q_DECLARE_PRIVATE(VArchive)

    VArchivePrivate *const d_ptr;
};

class VArchiveEntryPrivate;

class VIBECORE_EXPORT VArchiveEntry
{
public:
    VArchiveEntry(VArchive *archive, const QString &name, int access, int date,
                  const QString &user, const QString &group,
                  const QString &symlink);
    virtual ~VArchiveEntry();

    QDateTime datetime() const;
    int date() const;

    QString name() const;

    mode_t permissions() const;

    QString user() const;
    QString group() const;

    QString symLinkTarget() const;

    virtual bool isFile() const;
    virtual bool isDirectory() const;

protected:
    VArchive *archive() const;

private:
    Q_DECLARE_PRIVATE(VArchiveEntry)

    VArchiveEntryPrivate *const d_ptr;
};

class VArchiveFilePrivate;

class VIBECORE_EXPORT VArchiveFile : public VArchiveEntry
{
public:
    VArchiveFile(VArchive *archive, const QString &name, int access, int date,
                 const QString &user, const QString &group, const QString &symlink,
                 qint64 pos, qint64 size);
    virtual ~VArchiveFile();

    qint64 position() const;

    qint64 size() const;
    void setSize(qint64 s);

    virtual QByteArray data() const;

    virtual QIODevice *createDevice() const;

    virtual bool isFile() const;

    void copyTo(const QString &dest) const;

private:
    Q_DECLARE_PRIVATE(VArchiveFile)

    VArchiveFilePrivate *const d_ptr;
};

class VArchiveDirectoryPrivate;

class VIBECORE_EXPORT VArchiveDirectory : public VArchiveEntry
{
public:
    VArchiveDirectory(VArchive *archive, const QString &name, int access, int date,
                      const QString &user, const QString &group,
                      const QString &symlink);
    virtual ~VArchiveDirectory();

    QStringList entries() const;

    const VArchiveEntry *entry(const QString &name) const;

    void addEntry(VArchiveEntry *);

    virtual bool isDirectory() const;

    void copyTo(const QString &dest, bool recursive = true) const;

private:
    Q_DECLARE_PRIVATE(VArchiveDirectory)

    VArchiveDirectoryPrivate *const d_ptr;
};

#endif // VARCHIVE_H
