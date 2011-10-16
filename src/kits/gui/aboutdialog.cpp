
#include <QtGui/QApplication>
#include <QtGui/QLabel>

#include "aboutdialog.h"
#include "aboutdialog_p.h"

namespace QubeGui
{
    AboutDialog::AboutDialog(QWidget *parent) :
        QDialog(parent),
        d_ptr(new AboutDialogPrivate(this))
    {
        setWindowTitle(QApplication::translate("AboutDialog", "About", 0, QApplication::UnicodeUTF8));
    }

    AboutDialog::~AboutDialog()
    {
        delete d_ptr;
    }

    QPixmap *AboutDialog::logo() const
    {
        Q_D(const AboutDialog);
        return const_cast<QPixmap *>(d->logoLabel->pixmap());
    }

    void AboutDialog::setLogo(const QPixmap &pixmap)
    {
        Q_D(AboutDialog);
        d->logoLabel->setPixmap(pixmap);
    }

    QString AboutDialog::description() const
    {
        Q_D(const AboutDialog);
        return d->descrLabel->text();
    }

    void AboutDialog::setDescription(const QString &descr)
    {
        Q_D(AboutDialog);
        d->descrLabel->setText(descr);
    }

    QString AboutDialog::copyright() const
    {
        Q_D(const AboutDialog);
        return d->copyrightLabel->text();
    }

    void AboutDialog::setCopyright(const QString &text)
    {
        Q_D(AboutDialog);
        d->copyrightLabel->setText(text);
    }

    QString AboutDialog::licenseText() const
    {
        Q_D(const AboutDialog);
        return d->licenseText();
    }

    void AboutDialog::setLicenseText(const QString &text)
    {
        Q_D(AboutDialog);
        d->setLicenseText(text);
    }

    QStringList AboutDialog::authors() const
    {
        Q_D(const AboutDialog);
        return d->authors();
    }

    void AboutDialog::setAuthors(const QStringList &authors)
    {
        Q_D(AboutDialog);
        d->setAuthors(authors);
    }

    QUrl AboutDialog::link() const
    {
        Q_D(const AboutDialog);
        return d->link();
    }

    void AboutDialog::setLink(const QUrl &url)
    {
        Q_D(AboutDialog);
        d->setLink(url);
    }
}
