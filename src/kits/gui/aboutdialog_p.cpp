
#include <QtGui/QApplication>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QTextBrowser>
#include <QtGui/QTableWidget>

#include "aboutdialog_p.h"
#include "aboutdialog.h"

namespace QubeGui
{
    AboutDialogPrivate::AboutDialogPrivate(AboutDialog *parent) :
        q_ptr(parent)
    {
        verticalLayout = new QVBoxLayout(parent);
        logoLabel = new QLabel(parent);
        logoLabel->setAlignment(Qt::AlignCenter);
        verticalLayout->addWidget(logoLabel);

        appVerLabel = new QLabel(parent);
        QFont font;
        font.setPointSize(16);
        appVerLabel->setFont(font);
        appVerLabel->setAlignment(Qt::AlignCenter);
        appVerLabel->setText(QString("<strong>%1 %2</strong>")
                               .arg(qApp->applicationName())
                               .arg(qApp->applicationVersion()));
        verticalLayout->addWidget(appVerLabel);

        descrLabel = new QLabel(parent);
        descrLabel->setAlignment(Qt::AlignCenter);
        verticalLayout->addWidget(descrLabel);

        copyrightLabel = new QLabel(parent);
        copyrightLabel->setAlignment(Qt::AlignCenter);
        copyrightLabel->setWordWrap(true);
        verticalLayout->addWidget(copyrightLabel);

        websiteLabel = new QLabel(parent);
        websiteLabel->setAlignment(Qt::AlignCenter);
        verticalLayout->addWidget(websiteLabel);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        horizontalLayout->addItem(horizontalSpacer);

        moreInfoButton = new QPushButton(parent);
        moreInfoButton->setText(QObject::tr("More Information.."));
        moreInfoButton->hide();
        horizontalLayout->addWidget(moreInfoButton);

        closeButton = new QPushButton(parent);
        closeButton->setDefault(true);
        closeButton->setText(QObject::tr("Close"));
        horizontalLayout->addWidget(closeButton);

        verticalLayout->addLayout(horizontalLayout);

        connect(moreInfoButton, SIGNAL(clicked()),
                this, SLOT(slotMoreInformation()));
        connect(closeButton, SIGNAL(clicked()),
                this, SLOT(slotClose()));
    }

    void AboutDialogPrivate::setLicenseText(const QString &text)
    {
        m_licenseText = text;
        if (m_licenseText == QString::null && m_authors.count() == 0)
            moreInfoButton->hide();
        else
            moreInfoButton->show();
    }

    void AboutDialogPrivate::setAuthors(const QStringList &authors)
    {
        m_authors = authors;
        if (m_licenseText == QString::null && m_authors.count() == 0)
            moreInfoButton->hide();
        else
            moreInfoButton->show();
    }

    void AboutDialogPrivate::setLink(const QUrl &url)
    {
        m_link = url;
        if (!url.isValid())
            websiteLabel->setText(QString::null);
        else
            websiteLabel->setText(QString("<a href=\"%1\">%1</a>").arg(url.toString()));
    }

    void AboutDialogPrivate::slotMoreInformation()
    {
        QDialog *dialog = new QDialog();
        dialog->setWindowTitle(QObject::tr("More Information"));

        QTabWidget *tab = new QTabWidget();

        if (m_authors.count() > 0) {
            QTextBrowser *authors = new QTextBrowser();
            authors->setText(m_authors.join("\n"));
            tab->addTab(authors, QObject::tr("Authors"));
        }

        if (m_licenseText.trimmed() != QString::null) {
            QTextBrowser *license = new QTextBrowser();
            license->setText(m_licenseText);

            tab->addTab(license, QObject::tr("License"));
        }

        QVBoxLayout *vbox = new QVBoxLayout(dialog);
        vbox->addWidget(tab);

        dialog->show();
    }

    void AboutDialogPrivate::slotClose()
    {
        Q_Q(AboutDialog);
        q->close();
    }
}
