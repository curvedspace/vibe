/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2010-2011 Pier Luigi Fiorini
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

#ifndef VSTRINGHANDLER_H
#define VSTRINGHANDLER_H

#include <QtCore/QString>

#include <VibeCore/VGlobal>

/**
 * This namespace contains utility functions for handling strings.
 *
 * The functions here are intended to provide an easy way to
 * cut/slice/splice words inside sentences in whatever order desired.
 * While the main focus of StringHandler is words (ie characters
 * separated by spaces/tabs), the two core functions here (split()
 * and join()) will allow you to use any character as a separator
 * This will make it easy to redefine what a 'word' means in the
 * future if needed.
 *
 * The function names and calling styles are based on python and mIRC's
 * scripting support.
 *
 * The ranges are a fairly powerful way of getting/stripping words from
 * a string. These ranges function, for the large part, as they would in
 * python. See the word(const QString&, int) and remword(const QString&, int)
 * functions for more detail.
 *
 * The methods here are completely stateless.  All strings are cut
 * on the fly and returned as new qstrings/qstringlists.
 *
 * @short Namespace for manipulating words and sentences in strings
 * @author Ian Zepp <icszepp@islc.net>
 * @author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
namespace VStringHandler
{
    /** Substitute characters at the beginning, at the middle or at the end of
     * a string by an ellipsis ("...").
     * @param str is the string to modify
     * @param mode is where the ellipsis appears
     * @param maxlen is the maximum length the modified string will have
     * If the original string is shorter than "maxlen", it is returned verbatim
     * @return the modified string or the same string if @mode is Qt::ElideNone or
     * the @str string length is more than @maxlen.
     */
    VIBE_EXPORT QString elide(const QString &str, Qt::TextElideMode mode, int maxlen = 40);
}

#endif // VSTRINGHANDLER_H