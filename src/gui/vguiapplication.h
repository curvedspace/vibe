/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
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

#ifndef VGUIAPPLICATION_H
#define VGUIAPPLICATION_H

#include <QGuiApplication>

#include <VibeGui/VibeGuiExport>

class VGuiApplicationPrivate;

class VIBEGUI_EXPORT VGuiApplication : public QGuiApplication
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
