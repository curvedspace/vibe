#include <QtGui/QAction>
#include <QtDesigner/QtDesigner>

#include "qnavbar.h"
#include "qnavbarplugintaskmenu.h"
#include "qnavbarplugintaskmenufactory.h"
#include "qnavbarplugindialog.h"

QNavBarPluginTaskMenu::QNavBarPluginTaskMenu(QNavBar *navBar, QObject *parent) :
    QObject(parent),
    m_navBar(navBar),
    m_editItemsAction(new QAction(tr("Edit Items..."), this))
{
    connect(m_editItemsAction, SIGNAL(triggered()),
            this, SLOT(editItems()));
}

QAction *QNavBarPluginTaskMenu::preferredEditAction() const
{
    return m_editItemsAction;
}

QList<QAction *> QNavBarPluginTaskMenu::taskActions() const
{
    QList<QAction *> list;
    list.append(m_editItemsAction);
    return list;
}

void QNavBarPluginTaskMenu::editItems()
{
    QNavBarPluginDialog dialog(m_navBar);
    dialog.exec();
}
