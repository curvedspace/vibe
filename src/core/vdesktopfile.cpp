/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPL2$
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QTextStream>
#include <QVariant>
#include <QStandardPaths>
#include <QStringList>
#include <QUrl>
#include <QLocale>
#include <QSettings>

#include "vdesktopfile.h"
#include "vdesktopfile_p.h"

/*
 * VDesktopFilePrivate
 */

VDesktopFilePrivate::VDesktopFilePrivate(const QString &_fileName, VDesktopFile *self)
    : fileName(_fileName)
    , q_ptr(self)
{
    // Read desktop file
    file = new QSettings(fileName, QSettings::IniFormat);
    file->beginGroup("Desktop Entry");
    isValid = file->allKeys().size() > 0;

    // Watch for changes
    watcher = new QFileSystemWatcher(q_ptr);
    watcher->addPath(fileName);
}

VDesktopFilePrivate::~VDesktopFilePrivate()
{
    delete watcher;
}

void VDesktopFilePrivate::read()
{
    // Read desktop file
    delete file;
    file = new QSettings(fileName, QSettings::IniFormat);
    file->beginGroup("Desktop Entry");
    isValid = file->allKeys().size() > 0;
}

bool VDesktopFilePrivate::contains(const QString &key) const
{
    return file->contains(key);
}

QVariant VDesktopFilePrivate::value(const QString &key, const QVariant &defaultValue) const
{
    QVariant val = file->value(key, defaultValue);
    return val.toString().replace(QLatin1Char('&'), QLatin1String("&&"));
}

QVariant VDesktopFilePrivate::localizedValue(const QString &key, const QVariant &defaultValue) const
{
    QLocale locale = QLocale::system();

    QString k = QString("%1[%2]").arg(key, locale.name());
    if (contains(k))
        return value(k, defaultValue);

    return value(key, defaultValue);
}

bool VDesktopFilePrivate::checkTryExec(const QString &program) const
{
    // Check if a full path was provided
    QFileInfo info(program);
    if (info.exists())
        return QFileInfo(program).isExecutable();

    // Find the executable from the system paths
    info.setFile(QStandardPaths::findExecutable(program));
    if (info.exists())
            return info.isExecutable();

    return false;
}

void VDesktopFilePrivate::_q_fileChanged(const QString &_fileName)
{
    // Read the file again
    fileName = _fileName;
    read();

    // Notify that the desktop file has changed
    emit q_ptr->changed(fileName);
}

/*
 * VDesktopFile
 */

VDesktopFile::VDesktopFile(const QString &fileName)
    : QObject()
    , d_ptr(new VDesktopFilePrivate(fileName, this))
{
    connect(d_ptr->watcher, SIGNAL(fileChanged(QString)),
            this, SLOT(_q_fileChanged(QString)));
}

VDesktopFile::~VDesktopFile()
{
    delete d_ptr;
}

bool VDesktopFile::load(const QString &fileName)
{
    Q_D(VDesktopFile);

    if (QFile::exists(fileName))
        d->fileName = fileName;
    else
        d->fileName = QStandardPaths::locate(QStandardPaths::GenericDataLocation,
                                             QStringLiteral("applications/%1").arg(fileName));
    d->read();
    return d->isValid;
}

QString VDesktopFile::fileName() const
{
    Q_D(const VDesktopFile);
    return d->fileName;
}

bool VDesktopFile::isValid() const
{
    Q_D(const VDesktopFile);
    return d->isValid;
}

QVariant VDesktopFile::value(const QString &key, const QVariant &defaultValue) const
{
    Q_D(const VDesktopFile);
    return d->value(key, defaultValue);
}

VDesktopFile::Type VDesktopFile::type() const
{
    QString typeStr = typeName();

    if (typeStr == "Application")
        return VDesktopFile::ApplicationType;

    if (typeStr == "Link")
        return VDesktopFile::LinkType;

    if (typeStr == "Directory")
        return VDesktopFile::DirectoryType;

    return VDesktopFile::UnknownType;
}

QString VDesktopFile::typeName() const
{
    Q_D(const VDesktopFile);
    return d->value("Type").toString();
}

QString VDesktopFile::version() const
{
    Q_D(const VDesktopFile);
    return d->value("Version", "1.0").toString();
}

QString VDesktopFile::name() const
{
    Q_D(const VDesktopFile);
    return d->localizedValue("Name").toString();
}

QString VDesktopFile::genericName() const
{
    Q_D(const VDesktopFile);
    return d->localizedValue("GenericName").toString();
}

QString VDesktopFile::comment() const
{
    Q_D(const VDesktopFile);
    return d->localizedValue("Comment").toString();
}

QString VDesktopFile::iconName() const
{
    Q_D(const VDesktopFile);
    return d->localizedValue("Icon").toString();
}

bool VDesktopFile::isHidden() const
{
    Q_D(const VDesktopFile);

    // Application exists but should be hidden in the menu
    if (d->value("NoDisplay").toBool())
        return true;

    // User deleted this application at his level
    if (d->value("Hidden").toBool())
        return true;

    return false;
}

bool VDesktopFile::isHiddenFor(const QString &environment) const
{
    QStringList onlyShow = onlyShowIn();
    QStringList notShow = notShowIn();

    // Application is visible for other environments
    if (onlyShow.size() > 0 && !onlyShow.contains(environment))
        return true;

    // We are explicitly told the application is hidden in this environment
    if (notShow.size() > 0 && notShow.contains(environment))
        return true;

    return false;
}

QStringList VDesktopFile::onlyShowIn() const
{
    Q_D(const VDesktopFile);
    return d->value("OnlyShowIn").toString().split(';', QString::SkipEmptyParts);
}

QStringList VDesktopFile::notShowIn() const
{
    Q_D(const VDesktopFile);
    return d->value("NotShowIn").toString().split(';', QString::SkipEmptyParts);
}

QString VDesktopFile::tryExecutePath() const
{
    Q_D(const VDesktopFile);
    return d->value("TryExec").toString();
}

bool VDesktopFile::isExecutable() const
{
    Q_D(const VDesktopFile);

    QString tryExec = tryExecutePath();

    // If TryExec is empty fallback to the first Exec argument
    if (tryExec.isEmpty()) {
        QStringList args = executeCommand().split(QLatin1Char(' '), QString::SkipEmptyParts);
        if (args.size() == 0)
            return false;
        tryExec = args.at(0);
    }

    return d->checkTryExec(tryExec);
}

QString VDesktopFile::executeCommand() const
{
    Q_D(const VDesktopFile);
    return d->value("Exec").toString();
}

QDir VDesktopFile::workingDirectory() const
{
    Q_D(const VDesktopFile);
    return QDir(d->value("Path").toString());
}

bool VDesktopFile::runInTerminal() const
{
    Q_D(const VDesktopFile);
    return d->value("Terminal").toBool();
}

QStringList VDesktopFile::supportedMimeTypes() const
{
    Q_D(const VDesktopFile);
    return d->value("MimeType").toString().split(';', QString::SkipEmptyParts);
}

QStringList VDesktopFile::categories() const
{
    Q_D(const VDesktopFile);
    return d->value("Categories").toString().split(';', QString::SkipEmptyParts);
}

// TODO StartupNotify and StartupWMClass

QUrl VDesktopFile::url() const
{
    Q_D(const VDesktopFile);
    return d->value("URL").toUrl();
}

#include "moc_vdesktopfile.cpp"
