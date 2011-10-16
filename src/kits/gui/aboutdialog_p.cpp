
#include <QtGui/QApplication>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

#include "aboutdialog_p.h"
#include "aboutdialog.h"

namespace QubeGui
{
    AboutDialogPrivate::AboutDialogPrivate(AboutDialog *parent) :
        q_ptr(parent)
    {
        m_verticalLayout = new QVBoxLayout(parent);
        m_logoLabel = new QLabel(parent);
        m_logoLabel->setAlignment(Qt::AlignCenter);
        m_verticalLayout->addWidget(m_logoLabel);

        m_appVerLabel = new QLabel(parent);
        QFont font;
        font.setPointSize(16);
        m_appVerLabel->setFont(font);
        m_appVerLabel->setAlignment(Qt::AlignCenter);
        m_appVerLabel->setText(QString("<strong>%1 %2</strong>")
                               .arg(qApp->applicationName())
                               .arg(qApp->applicationVersion()));
        m_verticalLayout->addWidget(m_appVerLabel);

        m_descrLabel = new QLabel(parent);
        m_descrLabel->setAlignment(Qt::AlignCenter);
        m_verticalLayout->addWidget(m_descrLabel);

        m_copyrightLabel = new QLabel(parent);
        m_copyrightLabel->setAlignment(Qt::AlignCenter);
        m_copyrightLabel->setWordWrap(true);
        m_verticalLayout->addWidget(m_copyrightLabel);

        m_websiteLabel = new QLabel(parent);
        m_websiteLabel->setAlignment(Qt::AlignCenter);
        m_verticalLayout->addWidget(m_websiteLabel);

        m_verticalSpacer = new QSpacerItem(20, 299, QSizePolicy::Minimum, QSizePolicy::Expanding);
        m_verticalLayout->addItem(m_verticalSpacer);

        m_horizontalLayout = new QHBoxLayout();
        m_horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        m_horizontalLayout->addItem(m_horizontalSpacer);

        m_moreInfoButton = new QPushButton(parent);
        m_moreInfoButton->setText(QApplication::translate("AboutDialog", "More Information....", 0, QApplication::UnicodeUTF8));
        m_horizontalLayout->addWidget(m_moreInfoButton);

        m_closeButton = new QPushButton(parent);
        m_closeButton->setDefault(true);
        m_closeButton->setText(QApplication::translate("AboutDialog", "Close", 0, QApplication::UnicodeUTF8));
        m_horizontalLayout->addWidget(m_closeButton);

        m_verticalLayout->addLayout(m_horizontalLayout);

        connect(m_moreInfoButton, SIGNAL(clicked()),
                this, SLOT(slotMoreInformation()));
        connect(m_closeButton, SIGNAL(clicked()),
                this, SLOT(slotClose()));
    }

    void AboutDialogPrivate::slotMoreInformation()
    {
    }

    void AboutDialogPrivate::slotClose()
    {
        Q_Q(AboutDialog);
        q->close();
    }
}
