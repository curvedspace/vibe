/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPL-ONLY$
 *
 * This file may be used under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation and
 * appearing in the file LICENSE.LGPL included in the packaging of
 * this file, either version 2.1 of the License, or (at your option) any
 * later version.  Please review the following information to ensure the
 * GNU Lesser General Public License version 2.1 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 * If you have questions regarding the use of this file, please contact
 * us via http://www.maui-project.org/.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef VMAINAPPLICATIONADAPTOR_H
#define VMAINAPPLICATIONADAPTOR_H

#include <QDBusAbstractAdaptor>
#include <QGuiApplication>

#include <VibeCore/VGlobal>

class VMainApplicationAdaptorPrivate;

class VIBE_EXPORT VMainApplicationAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.maui.DBus.MainApplication")
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
