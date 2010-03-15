
#include <QtGui/QApplication>

#include "qnavbar.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QNavBar *navbar = new QNavBar();
    QNavBarGroup *group = navbar->addGroup("APPLICATIONS");
    group->addItem(QPixmap(":/icons/z_allApps.png"), "All Applications");
    group->addItem(QPixmap(":/icons/z_updates.png"), "Updates");
    group->addItem(QPixmap(":/icons/z_upToDate.png"), "Up To Date");
    group->addItem(QPixmap(":/icons/z_unknown.png"), "Unknown");
    group = navbar->addGroup("BY KIND");
    group->setStatic(true);
    group->addItem(QPixmap(":/icons/z_apps.png"), "Applications");
    group->addItem(QPixmap(":/icons/z_plugins.png"), "Plugins");
    group->addItem(QPixmap(":/icons/z_widgets.png"), "Widgets");
    group->addItem(QPixmap(":/icons/z_prefApp.png"), "Preference Panes");
    group->addItem(QPixmap(":/icons/z_systemUpdates.png"), "System Updates");
    group = navbar->addGroup("VENDORS");
    group->addItem(QPixmap(":/icons/z_apple.png"), "Apple");
    navbar->show();
    return app.exec();
}
