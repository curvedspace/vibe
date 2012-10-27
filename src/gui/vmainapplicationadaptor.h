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

#ifndef VMAINAPPLICATIONADAPTOR_H
#define VMAINAPPLICATIONADAPTOR_H

#include <QDBusAbstractAdaptor>
#include <QGuiApplication>

#include <VibeGui/VibeGuiExport>

class VMainApplicationAdaptorPrivate;

class VIBEGUI_EXPORT VMainApplicationAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.hawaii.DBus.MainApplication")
    Q_PROPERTY(QString identifier READ identifier)
    Q_PROPERTY(QString applicationName READ applicationName)
    Q_PROPERTY(QString applicationVersion READ applicationVersion)
    Q_PROPERTY(qint64 applicationPid READ applicationPid)
    Q_PROPERTY(QString applicationDirPath READ applicationDirPath)
    Q_PROPERTY(QString applicationFilePath READ applicationFilePath)
    Q_PROPERTY(QString organizationName READ organizationName)
    Q_PROPERTY(QString organizationDomain READ organizationDomain)
public:
    explicit VMainApplicationAdaptor(const QString &identifier, QGuiApplication *app);

    QString identifier() const;

    QString applicationName() const;
    QString applicationVersion() const;
    qint64 applicationPid() const;
    QString applicationDirPath() const;
    QString applicationFilePath() const;

    QString organizationName() const;
    QString organizationDomain() const;

    void emitDemandsAttention();

signals:
    void Activate(uint);
    void Deactivate(uint);
    void DemandsAttention(uint);
    void LastWindowClosed(uint);
    void Quit(uint);

public slots:
    QString GetDesktopFileName();

    Q_NOREPLY void ReloadSettings();

    Q_NOREPLY void CloseAllWindows();
    Q_NOREPLY void Quit();

protected:
    bool eventFilter(QObject *obj, QEvent *e);

private:
    Q_DECLARE_PRIVATE(VMainApplicationAdaptor)
    Q_PRIVATE_SLOT(d_ptr, void _q_lastWindowClosed())
    Q_PRIVATE_SLOT(d_ptr, void _q_aboutToQuit())

    VMainApplicationAdaptorPrivate *const d_ptr;
};

#endif // VMAINAPPLICATIONADAPTOR_H
