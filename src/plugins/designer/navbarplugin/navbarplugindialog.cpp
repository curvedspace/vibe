#include <QtGui/QtGui>
#include <QtDesigner/QtDesigner>

#include "qnavbar.h"
#include "qnavbarplugindialog.h"

QNavBarPluginDialog::QNavBarPluginDialog(QNavBar *navBar, QWidget *parent)
    : QDialog(parent)
{
    m_navBar = navBar;
    m_editor = new QNavBar();
    m_editor->addGroups(m_navBar->groups());
    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                       | QDialogButtonBox::Cancel
                                       | QDialogButtonBox::Reset);
    connect(m_buttonBox->button(QDialogButtonBox::Reset), SIGNAL(clicked()),
            this, SLOT(resetGroups()));
    connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(saveGroups()));
    connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(m_editor);
    mainLayout->addWidget(m_buttonBox);
    setLayout(mainLayout);
    setWindowTitle(tr("Edit Items"));
}

QSize QNavBarPluginDialog::sizeHint() const
{
    return QSize(250, 250);
}

void QNavBarPluginDialog::resetGroups()
{
    m_editor->clear();
}

void QNavBarPluginDialog::saveGroups()
{
#if 0
    if (QDesignerFormWindowInterface *formWindow
            = QDesignerFormWindowInterface::findFormWindow(m_navBar)) {
        formWindow->cursor()->setProperty("state", editor->groups());
    }
#endif
    accept();
}
