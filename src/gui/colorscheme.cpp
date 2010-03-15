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

#include <QtCore/QLocale>
#include <QtCore/QSettings>

#include "colorscheme.h"

namespace QubeGui
{
    class ColorSchemePrivate
    {
    public:
        typedef QPair<QPalette::ColorGroup, QString> Entry;
        typedef QPair<Entry, QColor> Color;

        QSettings *colorScheme;
        QList<Color> colors;
        QPalette palette;

        ColorSchemePrivate(const QString &name) {
            // TODO: Look from user's directory then system-wide
            QString fileName = name;

            // Force to INI file format
            colorScheme = new QSettings(fileName, QSettings::IniFormat);
            colorScheme->beginGroup("Color Scheme");

            // Load the palette
            QStringList keys = colorScheme->childKeys();
            for (int i = 0; i < keys.size(); i++) {
                QString key = keys.at(i);
                QPalette::ColorGroup group;

                // Convert group name to its value
                if (key.startsWith("Active"))
                    group = QPalette::Active;
                else if (key.startsWith("Disabled"))
                    group = QPalette::Disabled;
                else if (key.startsWith("Inactive"))
                    group = QPalette::Inactive;

                // Appen to colors list
                QRegExp regExp("(?:Active|Disabled|Inactive)(.+)");
                if (regExp.exactMatch(key)) {
                    ColorSchemePrivate::Entry entry;
                    ColorSchemePrivate::Color color;

                    entry.first = group;
                    entry.second = regExp.capturedTexts()[0];
                    color.first = entry;
                    color.second = QColor(colorScheme->value(key, QColor()).toString());
                    colors.append(color);

                    // Now add this color to the palette
                    QPalette::ColorRole role;
                    if (key == "Background")
                        role = QPalette::Background;
                    else if (key == "Foreground")
                        role = QPalette::Foreground;
                    else if (key == "Base")
                        role = QPalette::Base;
                    else if (key == "Text")
                        role = QPalette::Text;
                    else if (key == "Button")
                        role = QPalette::Button;
                    else if (key == "ButtonText")
                        role = QPalette::ButtonText;
                    else if (key == "Link")
                        role = QPalette::Link;
                    else if (key == "LinkVisited")
                        role = QPalette::LinkVisited;
                    else if (key == "Highlight")
                        role = QPalette::Highlight;
                    else if (key == "HighlightedText")
                        role = QPalette::HighlightedText;
                    palette.setColor(entry.first, role, color.second);
                }
            }
        }

        QColor value(QPalette::ColorGroup group, const QString &name) const {
            for (int i = 0; i < colors.size(); i++) {
                ColorSchemePrivate::Color color = colors.at(i);
                ColorSchemePrivate::Entry entry = color.first;

                if (entry.first == group && entry.second == name)
                    return color.second;
            }

            return QColor();
        }
    };

    ColorScheme::ColorScheme(const QString &name)
        : d_ptr(new ColorSchemePrivate(name))
    {
    }

    QString ColorScheme::name() const
    {
        Q_D(const ColorScheme);

        QLocale locale;
        return d->colorScheme->value("Name[" + locale.name().split('_')[0] + "]",
                                     d->colorScheme->value("Name")).toString();
    }

    QString ColorScheme::comment() const
    {
        Q_D(const ColorScheme);

        QLocale locale;
        return d->colorScheme->value("Comment[" + locale.name().split('_')[0] + "]",
                                     d->colorScheme->value("Comment")).toString();
    }

    QPalette ColorScheme::palette() const
    {
        Q_D(const ColorScheme);
        return d->palette;
    }

    QColor ColorScheme::background(QPalette::ColorGroup group) const
    {
        Q_D(const ColorScheme);
        return d->value(group, "Background");
    }

    QColor ColorScheme::foreground(QPalette::ColorGroup group) const
    {
        Q_D(const ColorScheme);
        return d->value(group, "Foreground");
    }

    QColor ColorScheme::base(QPalette::ColorGroup group) const
    {
        Q_D(const ColorScheme);
        return d->value(group, "Base");
    }

    QColor ColorScheme::text(QPalette::ColorGroup group) const
    {
        Q_D(const ColorScheme);
        return d->value(group, "Text");
    }

    QColor ColorScheme::button(QPalette::ColorGroup group) const
    {
        Q_D(const ColorScheme);
        return d->value(group, "Button");
    }

    QColor ColorScheme::buttonText(QPalette::ColorGroup group) const
    {
        Q_D(const ColorScheme);
        return d->value(group, "ButtonText");
    }

    QColor ColorScheme::link(QPalette::ColorGroup group) const
    {
        Q_D(const ColorScheme);
        return d->value(group, "Link");
    }

    QColor ColorScheme::linkVisited(QPalette::ColorGroup group) const
    {
        Q_D(const ColorScheme);
        return d->value(group, "LinkVisited");
    }

    QColor ColorScheme::highlight(QPalette::ColorGroup group) const
    {
        Q_D(const ColorScheme);
        return d->value(group, "Highlight");
    }

    QColor ColorScheme::highlightedText(QPalette::ColorGroup group) const
    {
        Q_D(const ColorScheme);
        return d->value(group, "HighlightedText");
    }
}
