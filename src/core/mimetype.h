/****************************************************************************
 *
 * Copyright (c) 2010 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 * Copyright (c) 2008 Matteo Bertozzi <theo.bertozzi@gmail.com>
 *
 * All rights reserved.
 * Contact: Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * GNU Lesser General Public License Usage
 * This file may be used under the terms of the GNU Lesser
 * General Public License version 2.1 as published by the Free Software
 * Foundation and appearing in the file LICENSE.LGPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU Lesser General Public License version 2.1 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 ***************************************************************************/

#ifndef MIMETYPE_H
#define MIMETYPE_H

#include <QtCore/QObject>
#include <QtCore/QFile>

#include "qube_global.h"

class QStringList;
class QDomElement;
class QDomNodeList;

namespace QubeCore
{
    class MimeTypePrivate;

    class QUBESHARED_EXPORT MimeType : public QObject
    {
	Q_DECLARE_PRIVATE(MimeType)
    public:
	explicit MimeType(const QString &mimeType = QString::null, QObject *parent = 0);

	bool fromFileName(const QString& fileName);
	bool fromFile(QFile *file);

	QString iconName() const;

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
	MimeTypePrivate *d_ptr;

	bool checkMagicString(const QString &offset, const QByteArray &value,
			      const QByteArray &mask, QFile *file);
    };
}

#endif // MIMETYPE_H
