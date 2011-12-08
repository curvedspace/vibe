/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Vibe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Vibe.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QtGui/QApplication>
#include <QtGui/QTableWidget>
#include <QtGui/QTextBrowser>

#include "vaboutdialog.h"
#include "vaboutdialog_p.h"

/*
 * VAboutDialog::Private
 */

VAboutDialogPrivate::VAboutDialogPrivate(VAboutDialog *q) :
    q_ptr(q)
{
    verticalLayout = new QVBoxLayout(q);
    logoLabel = new QLabel(q);
    logoLabel->setAlignment(Qt::AlignCenter);
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

    connect(moreInfoButton, SIGNAL(clicked()),
            this, SLOT(slotMoreInformation()));
    connect(closeButton, SIGNAL(clicked()),
            this, SLOT(slotClose()));
}

void VAboutDialogPrivate::setLicenseText(const QString &text)
{
    m_licenseText = text;
    if (m_licenseText == QString::null && m_authors.count() == 0)
        moreInfoButton->hide();
    else
        moreInfoButton->show();
}

void VAboutDialogPrivate::setAuthors(const QStringList &authors)
{
    m_authors = authors;
    if (m_licenseText == QString::null && m_authors.count() == 0)
        moreInfoButton->hide();
    else
        moreInfoButton->show();
}

void VAboutDialogPrivate::setLink(const QUrl &url)
{
    m_link = url;
    if (!url.isValid())
        websiteLabel->setText(QString::null);
    else
        websiteLabel->setText(QString("<a href=\"%1\">%1</a>").arg(url.toString()));
}

void VAboutDialogPrivate::slotMoreInformation()
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(tr("More Information"));

    QTabWidget *tab = new QTabWidget();

    if (m_authors.count() > 0) {
        QTextBrowser *authors = new QTextBrowser();
        authors->setText(m_authors.join("\n"));
        tab->addTab(authors, tr("Authors"));
    }

    if (m_licenseText.trimmed() != QString::null) {
        QTextBrowser *license = new QTextBrowser();
        license->setText(m_licenseText);

        tab->addTab(license, tr("License"));
    }

    QVBoxLayout *vbox = new QVBoxLayout(dialog);
    vbox->addWidget(tab);

    dialog->show();
}

void VAboutDialogPrivate::slotClose()
{
    Q_Q(VAboutDialog);
    q->close();
}

/*
 * VAboutDialog
 */

VAboutDialog::VAboutDialog(QWidget *parent) :
    QDialog(parent),
    d_ptr(new VAboutDialogPrivate(this))
{
    setWindowTitle(tr("About"));
    setSizeGripEnabled(false);
}

VAboutDialog::~VAboutDialog()
{
    delete d_ptr;
}

QPixmap *VAboutDialog::logo() const
{
    Q_D(const VAboutDialog);
    return const_cast<QPixmap *>(d->logoLabel->pixmap());
}

void VAboutDialog::setLogo(const QPixmap &pixmap)
{
    Q_D(VAboutDialog);
    d->logoLabel->setPixmap(pixmap);
}

QString VAboutDialog::description() const
{
    Q_D(const VAboutDialog);
    return d->descrLabel->text();
}

void VAboutDialog::setDescription(const QString &descr)
{
    Q_D(VAboutDialog);
    d->descrLabel->setText(descr);
}

QString VAboutDialog::copyright() const
{
    Q_D(const VAboutDialog);
    return d->copyrightLabel->text();
}

void VAboutDialog::setCopyright(const QString &text)
{
    Q_D(VAboutDialog);
    d->copyrightLabel->setText(text);
}

QString VAboutDialog::licenseText() const
{
    Q_D(const VAboutDialog);
    return d->licenseText();
}

void VAboutDialog::setLicenseText(const QString &text)
{
    Q_D(VAboutDialog);
    d->setLicenseText(text);
}

QStringList VAboutDialog::authors() const
{
    Q_D(const VAboutDialog);
    return d->authors();
}

void VAboutDialog::setAuthors(const QStringList &authors)
{
    Q_D(VAboutDialog);
    d->setAuthors(authors);
}

QUrl VAboutDialog::link() const
{
    Q_D(const VAboutDialog);
    return d->link();
}

void VAboutDialog::setLink(const QUrl &url)
{
    Q_D(VAboutDialog);
    d->setLink(url);
}

#include "vaboutdialog.moc"
