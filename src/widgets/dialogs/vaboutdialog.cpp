/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
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

#include <QApplication>
#include <QTableWidget>
#include <QTextBrowser>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QVBoxLayout>

#include "vaboutdialog.h"

/*
 * VAboutDialog::Private
 */

class VAboutDialog::Private
{
public:
    Private(VAboutDialog *self) :
        q(self) {
        verticalLayout = new QVBoxLayout(q);
        logoLabel = new QLabel(q);
        logoLabel->setAlignment(Qt::AlignCenter);
        logoLabel->setPixmap(qApp->windowIcon().pixmap(128));
        verticalLayout->addWidget(logoLabel);

        appVerLabel = new QLabel(q);
        QFont font;
        font.setPointSize(16);
        appVerLabel->setFont(font);
        appVerLabel->setAlignment(Qt::AlignCenter);
        appVerLabel->setText(QString("<strong>%1 %2</strong>")
                             .arg(qApp->applicationName())
                             .arg(qApp->applicationVersion()));
        verticalLayout->addWidget(appVerLabel);

        descrLabel = new QLabel(q);
        descrLabel->setAlignment(Qt::AlignCenter);
        verticalLayout->addWidget(descrLabel);

        copyrightLabel = new QLabel(q);
        copyrightLabel->setAlignment(Qt::AlignCenter);
        copyrightLabel->setWordWrap(true);
        verticalLayout->addWidget(copyrightLabel);

        websiteLabel = new QLabel(q);
        websiteLabel->setAlignment(Qt::AlignCenter);
        verticalLayout->addWidget(websiteLabel);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        horizontalLayout->addItem(horizontalSpacer);

        moreInfoButton = new QPushButton(q);
        moreInfoButton->setText(tr("More Information.."));
        moreInfoButton->hide();
        horizontalLayout->addWidget(moreInfoButton);

        closeButton = new QPushButton(q);
        closeButton->setDefault(true);
        closeButton->setText(tr("Close"));
        horizontalLayout->addWidget(closeButton);

        verticalLayout->addLayout(horizontalLayout);
    }

    QString licenseText() const {
        return m_licenseText;
    }
    void setLicenseText(const QString &text);

    QStringList authors() const {
        return m_authors;
    }
    void setAuthors(const QStringList &authors);

    QUrl link() const {
        return m_link;
    }
    void setLink(const QUrl &url);

    VAboutDialog *q;

    QVBoxLayout *verticalLayout;
    QLabel *logoLabel;
    QLabel *appVerLabel;
    QLabel *descrLabel;
    QLabel *copyrightLabel;
    QLabel *websiteLabel;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *moreInfoButton;
    QPushButton *closeButton;

    QString m_licenseText;
    QStringList m_authors;
    QUrl m_link;

    void _q_moreInformation();
    void _q_close();
};

void VAboutDialog::Private::setLicenseText(const QString &text)
{
    m_licenseText = text;
    if (m_licenseText == QString::null && m_authors.count() == 0)
        moreInfoButton->hide();
    else
        moreInfoButton->show();
}

void VAboutDialog::Private::setAuthors(const QStringList &authors)
{
    m_authors = authors;
    if (m_licenseText == QString::null && m_authors.count() == 0)
        moreInfoButton->hide();
    else
        moreInfoButton->show();
}

void VAboutDialog::Private::setLink(const QUrl &url)
{
    m_link = url;
    if (!url.isValid())
        websiteLabel->setText(QString::null);
    else
        websiteLabel->setText(QString("<a href=\"%1\">%1</a>").arg(url.toString()));
}

void VAboutDialog::Private::_q_moreInformation()
{
    QDialog *dialog = new QDialog(q);
    dialog->setModal(true);
    dialog->setWindowTitle(tr("More Information"));

    QTabWidget *tab = new QTabWidget();
    QVBoxLayout *vbox = new QVBoxLayout(dialog);
    vbox->addWidget(tab);

    if (m_authors.count() > 0) {
        QTextBrowser *authors = new QTextBrowser();
        authors->setText(m_authors.join("\n"));

        QWidget *authorsPage = new QWidget(tab);
        QVBoxLayout *authorsVbox = new QVBoxLayout(authorsPage);
        authorsVbox->addWidget(authors);
        tab->addTab(authorsPage, tr("Authors"));
    }

    if (m_licenseText.trimmed() != QString::null) {
        QTextBrowser *license = new QTextBrowser();
        license->setText(m_licenseText);

        QWidget *licensePage = new QWidget(tab);
        QVBoxLayout *licenseVbox = new QVBoxLayout(licensePage);
        licenseVbox->addWidget(license);
        tab->addTab(licensePage, tr("License"));
    }

    dialog->exec();
}

void VAboutDialog::Private::_q_close()
{
    q->close();
}

/*
 * VAboutDialog
 */

VAboutDialog::VAboutDialog(QWidget *parent) :
    QDialog(parent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint),
    d(new Private(this))
{
    setWindowTitle(tr("About"));
    setModal(true);
    setSizeGripEnabled(false);
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    connect(d->moreInfoButton, SIGNAL(clicked()),
            this, SLOT(_q_moreInformation()));
    connect(d->closeButton, SIGNAL(clicked()),
            this, SLOT(_q_close()));
}

VAboutDialog::~VAboutDialog()
{
    delete d;
}

QPixmap *VAboutDialog::logo() const
{
    return const_cast<QPixmap *>(d->logoLabel->pixmap());
}

void VAboutDialog::setLogo(const QPixmap &pixmap)
{
    d->logoLabel->setPixmap(pixmap);
}

QString VAboutDialog::description() const
{
    return d->descrLabel->text();
}

void VAboutDialog::setDescription(const QString &descr)
{
    d->descrLabel->setText(descr);
}

QString VAboutDialog::copyright() const
{
    return d->copyrightLabel->text();
}

void VAboutDialog::setCopyright(const QString &text)
{
    d->copyrightLabel->setText(text);
}

QString VAboutDialog::licenseText() const
{
    return d->licenseText();
}

void VAboutDialog::setLicenseText(const QString &text)
{
    d->setLicenseText(text);
}

QStringList VAboutDialog::authors() const
{
    return d->authors();
}

void VAboutDialog::setAuthors(const QStringList &authors)
{
    d->setAuthors(authors);
}

QUrl VAboutDialog::link() const
{
    return d->link();
}

void VAboutDialog::setLink(const QUrl &url)
{
    d->setLink(url);
}

#include "moc_vaboutdialog.cpp"
