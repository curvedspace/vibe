/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2008 Matteo Bertozzi
 * Copyright (c) 2010-2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Matteo Bertozzi <theo.bertozzi@gmail.com>
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

#ifndef VMIMETYPE_H
#define VMIMETYPE_H

#include <QtCore/QObject>
#include <QtCore/QFile>

#include <VibeCore/VGlobal>

class QStringList;
class QDomElement;
class QDomNodeList;
class QUrl;

class VMimeTypePrivate;

class VIBE_EXPORT VMimeType : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(VMimeType)
public:
    explicit VMimeType(const QString &mimeType = QString::null, QObject *parent = 0);

    bool fromFileName(const QString &fileName);
    bool fromFile(QFile *file);

    QString iconName() const;
    static QString iconNameForUrl(const QUrl &url);

    QString mimeType() const;
    QString subClassOf() const;

    QString comment() const;
    QString genericIconName() const;

    QString acronym() const;
    QString expandedAcronym() const;

    QStringList aliases() const;

    QStringList globs() const;

protected:
    bool getMimeNode();
    bool checkMagic(const QDomElement &magicNode, QFile *file);
    bool checkMagic(const QDomNodeList &matchList, QFile *file);
    bool checkSubMagic(const QDomNodeList &matchList, QFile *file);
    bool checkMatch(const QDomElement &matchNode, QFile *file);

private:
    VMimeTypePrivate *d_ptr;

    bool checkMagicString(const QString &offset, const QByteArray &value,
                          const QByteArray &mask, QFile *file);
};

#endif // VMIMETYPE_H
