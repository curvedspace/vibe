/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2010-2011 Pier Luigi Fiorini
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

#include <QLocale>
#include <QSettings>

#include "vcolorscheme.h"
#include "vcolorscheme_p.h"

/*
 * VColorSchemePrivate
 */

VColorSchemePrivate::VColorSchemePrivate(VColorScheme *q, const QString &name) :
    q_ptr(q)
{
    // Create a new palette object
    palette = new QPalette();

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
            if (key == "Window")
                role = QPalette::Window;
            else if (key == "WindowText")
                role = QPalette::WindowText;
            else if (key == "Base")
                role = QPalette::Base;
            else if (key == "AlternateBase")
                role = QPalette::AlternateBase;
            else if (key == "ToolTipBase")
                role = QPalette::ToolTipBase;
            else if (key == "ToolTipText")
                role = QPalette::ToolTipText;
            else if (key == "Text")
                role = QPalette::Text;
            else if (key == "Button")
                role = QPalette::Button;
            else if (key == "ButtonText")
                role = QPalette::ButtonText;
            else if (key == "BrightText")
                role = QPalette::BrightText;
            else if (key == "Light")
                role = QPalette::Light;
            else if (key == "Midlight")
                role = QPalette::Midlight;
            else if (key == "Dark")
                role = QPalette::Dark;
            else if (key == "Mid")
                role = QPalette::Mid;
            else if (key == "Shadow")
                role = QPalette::Shadow;
            else if (key == "Highlight")
                role = QPalette::Highlight;
            else if (key == "HighlightedText")
                role = QPalette::HighlightedText;
            else if (key == "Link")
                role = QPalette::Link;
            else if (key == "LinkVisited")
                role = QPalette::LinkVisited;
            palette->setColor(entry.first, role, color.second);
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

const QPalette *VColorScheme::palette() const
{
    Q_D(const VColorScheme);
    return (const QPalette *)d->palette;
}

QColor VColorScheme::window(QPalette::ColorGroup group) const
{
    Q_D(const VColorScheme);
    return d->value(group, "Window");
}

QColor VColorScheme::windowText(QPalette::ColorGroup group) const
{
    Q_D(const VColorScheme);
    return d->value(group, "WindowText");
}

QColor VColorScheme::base(QPalette::ColorGroup group) const
{
    Q_D(const VColorScheme);
    return d->value(group, "Base");
}

QColor VColorScheme::alternateBase(QPalette::ColorGroup group) const
{
    Q_D(const VColorScheme);
    return d->value(group, "AlternateBase");
}

QColor VColorScheme::toolTipBase(QPalette::ColorGroup group) const
{
    Q_D(const VColorScheme);
    return d->value(group, "ToolTipBase");
}

QColor VColorScheme::toolTipText(QPalette::ColorGroup group) const
{
    Q_D(const VColorScheme);
    return d->value(group, "ToolTipText");
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

QColor VColorScheme::brightText(QPalette::ColorGroup group) const
{
    Q_D(const VColorScheme);
    return d->value(group, "BrightText");
}

QColor VColorScheme::light(QPalette::ColorGroup group) const
{
    Q_D(const VColorScheme);
    return d->value(group, "Light");
}

QColor VColorScheme::midlight(QPalette::ColorGroup group) const
{
    Q_D(const VColorScheme);
    return d->value(group, "Midlight");
}

QColor VColorScheme::dark(QPalette::ColorGroup group) const
{
    Q_D(const VColorScheme);
    return d->value(group, "Dark");
}

QColor VColorScheme::mid(QPalette::ColorGroup group) const
{
    Q_D(const VColorScheme);
    return d->value(group, "Mid");
}

QColor VColorScheme::shadow(QPalette::ColorGroup group) const
{
    Q_D(const VColorScheme);
    return d->value(group, "Shadow");
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
