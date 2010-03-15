#ifndef QNAVBARPLUGINTASKMENU_H
#define QNAVBARPLUGINTASKMENU_H

#include <QtDesigner/QDesignerTaskMenuExtension>

QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE

class QNavBar;

class QNavBarPluginTaskMenu : public QObject, QDesignerTaskMenuExtension
{
    Q_OBJECT
    Q_INTERFACES(QDesignerTaskMenuExtension)
public:
    explicit QNavBarPluginTaskMenu(QNavBar *navBar, QObject *parent);

    QAction *preferredEditAction() const;
    QList<QAction *> taskActions() const;

private slots:
    void editItems();

private:
    QNavBar *m_navBar;
    QAction *m_editItemsAction;
};

#endif // QNAVBARPLUGINTASKMENU_H
