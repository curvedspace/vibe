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

#ifndef VAPPLICATIONINFO_H
#define VAPPLICATIONINFO_H

#include <QObject>

#include <VibeCore/VibeCoreExport>

class QDir;
class QStringList;

class VApplicationInfoPrivate;

class VIBECORE_EXPORT VApplicationInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName)
    Q_PROPERTY(bool valid READ isValid NOTIFY validChanged)
    Q_PROPERTY(QString identifier READ identifier NOTIFY identifierChanged)
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString genericName READ genericName NOTIFY genericNameChanged)
    Q_PROPERTY(QString comment READ comment NOTIFY commentChanged)
    Q_PROPERTY(QString iconName READ iconName NOTIFY iconNameChanged)
    Q_PROPERTY(bool hidden READ isHidden)
    Q_PROPERTY(QStringList onlyShowIn READ onlyShowIn)
    Q_PROPERTY(QStringList notShowIn READ notShowIn)
    Q_PROPERTY(QString tryExecutePath READ tryExecutePath)
    Q_PROPERTY(bool executable READ isExecutable)
    Q_PROPERTY(QString executeCommand READ executeCommand)
    Q_PROPERTY(QDir workingDirectory READ workingDirectory)
    Q_PROPERTY(bool runInTerminal READ runInTerminal)
    Q_PROPERTY(QStringList supportedMimeTypes READ supportedMimeTypes)
    Q_PROPERTY(QStringList categories READ categories)
public:
    explicit VApplicationInfo(const QString &fileName);
    ~VApplicationInfo();

    QString fileName() const;

    bool isValid() const;

    QString identifier() const;
    QString version() const;

    QString name() const;
    QString genericName() const;
    QString comment() const;
    QString iconName() const;
    bool isHidden() const;
    QStringList onlyShowIn() const;
    QStringList notShowIn() const;
    QString tryExecutePath() const;
    bool isExecutable() const;
    QString executeCommand() const;
    QDir workingDirectory() const;
    bool runInTerminal() const;
    QStringList supportedMimeTypes() const;
    QStringList categories() const;

    bool launch(const QStringList &args);

signals:
    void validChanged(bool valid);
    void identifierChanged(const QString &identifier);
    void versionChanged(const QString &version);
    void nameChanged(const QString &name);
    void genericNameChanged(const QString &genericName);
    void commentChanged(const QString &comment);
    void iconNameChanged(const QString &iconName);

private:
    Q_DECLARE_PRIVATE(VApplicationInfo)
    Q_PRIVATE_SLOT(d_ptr, void _q_desktopFileChanged())

    class VApplicationInfoPrivate *const d_ptr;
};

#endif // VAPPLICATIONINFO_H
