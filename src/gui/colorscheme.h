/****************************************************************************
 *
 * Copyright (c) 2010 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef COLORSCHEME_H
#define COLORSCHEME_H

#include <QtGui/QPalette>

namespace QubeGui
{
    class ColorSchemePrivate;

    class ColorScheme
    {
	Q_DECLARE_PRIVATE(ColorScheme)
    public:
	ColorScheme(const QString &name);

	QString name() const;
	QString comment() const;

	QPalette palette() const;

	QColor background(QPalette::ColorGroup group) const;
	QColor foreground(QPalette::ColorGroup group) const;
	QColor base(QPalette::ColorGroup group) const;
	QColor text(QPalette::ColorGroup group) const;
	QColor button(QPalette::ColorGroup group) const;
	QColor buttonText(QPalette::ColorGroup group) const;
	QColor link(QPalette::ColorGroup group) const;
	QColor linkVisited(QPalette::ColorGroup group) const;
	QColor highlight(QPalette::ColorGroup group) const;
	QColor highlightedText(QPalette::ColorGroup group) const;

    private:
	ColorSchemePrivate *d_ptr;
    };
}

#endif // COLORSCHEME_H
