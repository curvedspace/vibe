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

#include <QtCore/QLocale>
#include <QtCore/QSettings>

#include "vcolorscheme.h"
#include "vcolorscheme_p.h"

/*
 * VColorSchemePrivate
 */

VColorSchemePrivate::VColorSchemePrivate(VColorScheme *q, const QString &name) :
    q_ptr(q)
{
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

        // Append to colors list
        QRegExp regExp("(?:Active|Disabled|Inactive)(.+)");
        if (regExp.exactMatch(key)) {
            VColorSchemePrivate::Entry entry;
            VColorSchemePrivate::Color color;
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

QColor VColorSchemePrivate::value(QPalette::ColorGroup group, const QString &name) const
{
    for (int i = 0; i < colors.size(); i++) {
        VColorSchemePrivate::Color color = colors.at(i);
        VColorSchemePrivate::Entry entry = color.first;
        if (entry.first == group && entry.second == name)
            return color.second;
    }
    return QColor();
}

/*
 * VColorScheme
 */

VColorScheme::VColorScheme(const QString &name)
    : d_ptr(new VColorSchemePrivate(this, name))
{
}

QString VColorScheme::name() const
{
    Q_D(const VColorScheme);
    QLocale locale;
    return d->colorScheme->value("Name[" + locale.name().split('_')[0] + "]",
                                 d->colorScheme->value("Name")).toString();
}

QString VColorScheme::comment() const
{
    Q_D(const VColorScheme);
    QLocale locale;
    return d->colorScheme->value("Comment[" + locale.name().split('_')[0] + "]",
                                 d->colorScheme->value("Comment")).toString();
}

QPalette VColorScheme::palette() const
{
    Q_D(const VColorScheme);
    return d->palette;
}

QColor VColorScheme::background(QPalette::ColorGroup group) const
{
    Q_D(const VColorScheme);
    return d->value(group, "Background");
}

QColor VColorScheme::foreground(QPalette::ColorGroup group) const
{
    Q_D(const VColorScheme);
    return d->value(group, "Foreground");
}

QColor VColorScheme::base(QPalette::ColorGroup group) const
{
    Q_D(const VColorScheme);
    return d->value(group, "Base");
}

QColor VColorScheme::text(QPalette::ColorGroup group) const
{
    Q_D(const VColorScheme);
    return d->value(group, "Text");
}

QColor VColorScheme::button(QPalette::ColorGroup group) const
{
    Q_D(const VColorScheme);
    return d->value(group, "Button");
}

QColor VColorScheme::buttonText(QPalette::ColorGroup group) const
{
    Q_D(const VColorScheme);
    return d->value(group, "ButtonText");
}

QColor VColorScheme::link(QPalette::ColorGroup group) const
{
    Q_D(const VColorScheme);
    return d->value(group, "Link");
}

QColor VColorScheme::linkVisited(QPalette::ColorGroup group) const
{
    Q_D(const VColorScheme);
    return d->value(group, "LinkVisited");
}

QColor VColorScheme::highlight(QPalette::ColorGroup group) const
{
    Q_D(const VColorScheme);
    return d->value(group, "Highlight");
}

QColor VColorScheme::highlightedText(QPalette::ColorGroup group) const
{
    Q_D(const VColorScheme);
    return d->value(group, "HighlightedText");
}
