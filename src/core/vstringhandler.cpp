/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2010-2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
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

#include "vstringhandler.h"

/*!
    \namespace VStringHandler
    \inmodule core
    \target Vibe Namespace

    \brief Namespace for manipulating words and sentences in strings.

    The functions here are intended to provide an easy way to
    cut/slice/splice words inside sentences in whatever order desired.
    While the main focus of StringHandler is words (ie characters
    separated by spaces/tabs), the two core functions here (split()
    and join()) will allow you to use any character as a separator
    This will make it easy to redefine what a 'word' means in the
    future if needed.

    The function names and calling styles are based on python and mIRC's
    scripting support.

    The ranges are a fairly powerful way of getting/stripping words from
    a string. These ranges function, for the large part, as they would in
    Python. See the word(const QString&, int) and remword(const QString&, int)
    functions for more detail.

    The methods here are completely stateless.  All strings are cut
    on the fly and returned as new qstrings/qstringlists.

    \author Ian Zepp <icszepp@islc.net>
    \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
*/
namespace VStringHandler
{
    /*!
        Substitute characters at the beginning, at the middle or at the end of
        a string by an ellipsis ("...").

        \param str is the string to modify
        \param mode is where the ellipsis appears
        \param maxlen is the maximum length the modified string will have
        If the original string is shorter than \a maxlen, it is returned verbatim

        \return the modified string or the same string if \a mode is Qt::ElideNone or
        the \a str string length is more than \a maxlen.
    */
    QString elide(const QString &str, Qt::TextElideMode mode, int maxlen)
    {
        switch (mode) {
            case Qt::ElideLeft:
                if (str.length() > maxlen) {
                    int part = maxlen - 3;
                    return QString::fromLatin1("...") + str.right(part);
                }
                break;
            case Qt::ElideRight:
                if (str.length() > maxlen) {
                    int part = maxlen - 3;
                    return str.left(part) + QLatin1String("...");
                }
                break;
            case Qt::ElideMiddle:
                if (str.length() > maxlen) {
                    const int part = (maxlen - 3) / 2;
                    return str.left(part) + QLatin1String("...") + str.right(part);
                }
                break;
            default:
                break;
        }

        return str;
    }

    /*!
        Does a natural comparing of the strings. A negative value is returned if \a a
        is smaller than \a b. A positive value is returned if \a a is greater than \a b. 0
        is returned if both values are equal.  Whether to use case sensitive compare or
        not can be specified with \a caseSensitivity.
    */
    int naturalCompare(const QString &_a, const QString &_b, Qt::CaseSensitivity caseSensitivity)
    {
        // This method chops the input a and b into pieces of
        // digits and non-digits (a1.05 becomes a | 1 | . | 05)
        // and compares these pieces of a and b to each other
        // (first with first, second with second, ...).
        //
        // This is based on the natural sort order code code by Martin Pool
        // http://sourcefrog.net/projects/natsort/
        // Martin Pool agreed to license this under LGPL or GPL.

        // FIXME: Using toLower() to implement case insensitive comparison is
        // sub-optimal, but is needed because we compare strings with
        // localeAwareCompare(), which does not know about case sensitivity.
        // A task has been filled for this in Qt Task Tracker with ID 205990.
        // http://trolltech.com/developer/task-tracker/index_html?method=entry&id=205990
        QString a;
        QString b;
        if (caseSensitivity == Qt::CaseSensitive) {
            a = _a;
            b = _b;
        } else {
            a = _a.toLower();
            b = _b.toLower();
        }

        const QChar *currA = a.unicode(); // iterator over a
        const QChar *currB = b.unicode(); // iterator over b

        if (currA == currB) {
            return 0;
        }

        while (!currA->isNull() && !currB->isNull()) {
            const QChar *begSeqA = currA; // beginning of a new character sequence of a
            const QChar *begSeqB = currB;
            if (currA->unicode() == QChar::ObjectReplacementCharacter) {
                return 1;
            }

            if (currB->unicode() == QChar::ObjectReplacementCharacter) {
                return -1;
            }

            if (currA->unicode() == QChar::ReplacementCharacter) {
                return 1;
            }

            if (currB->unicode() == QChar::ReplacementCharacter) {
                return -1;
            }

            // find sequence of characters ending at the first non-character
            while (!currA->isNull() && !currA->isDigit() && !currA->isPunct() && !currA->isSpace()) {
                ++currA;
            }

            while (!currB->isNull() && !currB->isDigit() && !currB->isPunct() && !currB->isSpace()) {
                ++currB;
            }

            // compare these sequences
            const QStringRef &subA(a.midRef(begSeqA - a.unicode(), currA - begSeqA));
            const QStringRef &subB(b.midRef(begSeqB - b.unicode(), currB - begSeqB));
            const int cmp = QStringRef::localeAwareCompare(subA, subB);
            if (cmp != 0) {
                return cmp < 0 ? -1 : +1;
            }

            if (currA->isNull() || currB->isNull()) {
                break;
            }

            // find sequence of characters ending at the first non-character
            while ((currA->isPunct() || currA->isSpace()) && (currB->isPunct() || currB->isSpace())) {
                if (*currA != *currB) {
                    return (*currA < *currB) ? -1 : +1;
                }
                ++currA;
                ++currB;
                if (currA->isNull() || currB->isNull()) {
                    break;
                }
            }

            // now some digits follow...
            if ((*currA == QLatin1Char('0')) || (*currB == QLatin1Char('0'))) {
                // one digit-sequence starts with 0 -> assume we are in a fraction part
                // do left aligned comparison (numbers are considered left aligned)
                while (1) {
                    if (!currA->isDigit() && !currB->isDigit()) {
                        break;
                    } else if (!currA->isDigit()) {
                        return +1;
                    } else if (!currB->isDigit()) {
                        return -1;
                    } else if (*currA < *currB) {
                        return -1;
                    } else if (*currA > *currB) {
                        return + 1;
                    }
                    ++currA;
                    ++currB;
                }
            } else {
                // No digit-sequence starts with 0 -> assume we are looking at some integer
                // do right aligned comparison.
                //
                // The longest run of digits wins. That aside, the greatest
                // value wins, but we can't know that it will until we've scanned
                // both numbers to know that they have the same magnitude.

                bool isFirstRun = true;
                int weight = 0;
                while (1) {
                    if (!currA->isDigit() && !currB->isDigit()) {
                        if (weight != 0) {
                            return weight;
                        }
                        break;
                    } else if (!currA->isDigit()) {
                        if (isFirstRun) {
                            return *currA < *currB ? -1 : +1;
                        } else {
                            return -1;
                        }
                    } else if (!currB->isDigit()) {
                        if (isFirstRun) {
                            return *currA < *currB ? -1 : +1;
                        } else {
                            return +1;
                        }
                    } else if ((*currA < *currB) && (weight == 0)) {
                        weight = -1;
                    } else if ((*currA > *currB) && (weight == 0)) {
                        weight = + 1;
                    }
                    ++currA;
                    ++currB;
                    isFirstRun = false;
                }
            }
        }

        if (currA->isNull() && currB->isNull()) {
            return 0;
        }

        return currA->isNull() ? -1 : + 1;
    }
}
