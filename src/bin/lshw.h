/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2006 Kevin Ottens
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *	Kevin Ottens <ervin@kde.org>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Vibe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Vibe.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef LIST_HARDWARE_H
#define LIST_HARDWARE_H

#include <QCoreApplication>
#include <QEventLoop>

#include <VibeHardware/VStorageAccess>
#include <VibeHardware/VOpticalDrive>

namespace VHardware
{
    namespace Control
    {
        class Authentication;
    }
}

class HardwareConsole : public QCoreApplication
{
    Q_OBJECT
public:
    enum VolumeCallType {
        Mount,
        Unmount,
        Eject
    };

    HardwareConsole(int &argc, char *argv[]);

    bool hwList(bool interfaces, bool system);
    bool hwCapabilities(const QString &udi);
    bool hwProperties(const QString &udi);
    bool hwQuery(const QString &parentUdi, const QString &query);
    bool hwVolumeCall(VolumeCallType type, const QString &udi);

private:
    QEventLoop m_loop;
    int m_error;
    QString m_errorString;

private slots:
    void slotStorageResult(VHardware::ErrorType error, const QVariant &errorData);
};

Q_DECLARE_METATYPE(QList<int>)

#endif // LIST_HARDWARE_H
