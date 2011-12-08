#ifndef QNAVBARPLUGINDIALOG_H
#define QNAVBARPLUGINDIALOG_H

#include <QtGui/QDialog>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
QT_END_NAMESPACE

class QNavBar;

class QNavBarPluginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QNavBarPluginDialog(QNavBar *plugin = 0, QWidget *parent = 0);

    QSize sizeHint() const;

private slots:
    void resetGroups();
    void saveGroups();

private:
    QNavBar *m_editor;
    QNavBar *m_navBar;
    QDialogButtonBox *m_buttonBox;
};

#endif // QNAVBARPLUGINDIALOG_H
