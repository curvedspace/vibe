/****************************************************************************
 *
 * Copyright (c) 2010 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * All rights reserved.
 * Contact: Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * GNU General Public License Usage
 * This file may be used under the terms of the GNU General Public
 * License version 2 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging
 * of this file.  Please review the following information to
 * ensure the GNU General Public License version 2 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/gpl-2.0.html.
 *
 ***************************************************************************/

#ifndef NAVBARPLUGIN_H
#define NAVBARPLUGIN_H

#include <QtDesigner/QDesignerCustomWidgetInterface>

class NavBarPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    explicit NavBarPlugin(const QIcon &icon, QObject *parent = 0);

    void initialize(QDesignerFormEditorInterface *formEditor);
    bool isInitialized() const;
    bool isContainer() const;

    QWidget *createWidget(QWidget *parent);

    QIcon icon() const;
    QString domXml() const;
    QString group() const;
    QString includeFile() const;
    QString name() const;
    QString toolTip() const;
    QString whatsThis() const;

private:
    bool m_initialized;
    QIcon m_icon;
};

#endif // NAVBARPLUGIN_H
