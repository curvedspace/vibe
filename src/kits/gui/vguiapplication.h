/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
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

#ifndef VGUIAPPLICATION_H
#define VGUIAPPLICATION_H

#include <QGuiApplication>

#include <VibeCore/VGlobal>

class VGuiApplicationPrivate;

class VIBE_EXPORT VGuiApplication : public QGuiApplication
{
    Q_OBJECT
    Q_PROPERTY(QString identifier READ identifier WRITE setIdentifier)
public:
    explicit VGuiApplication(int &argc, char **argv);

    QString identifier() const;
    void setIdentifier(const QString &identifier);

    void demandsAttention();

    virtual void reloadSettings();
    virtual void closeAllWindows();

private:
    Q_DECLARE_PRIVATE(VGuiApplication)

    VGuiApplicationPrivate *const d_ptr;
};

#endif // VGUIAPPLICATION_H
