
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
    }

    QPixmap *AboutDialog::logo() const
    {
        Q_D(const AboutDialog);
        return const_cast<QPixmap *>(d->m_logoLabel->pixmap());
    }

    void AboutDialog::setLogo(const QPixmap &pixmap)
    {
        Q_D(AboutDialog);
        d->m_logoLabel->setPixmap(pixmap);
    }

    QString AboutDialog::description() const
    {
        Q_D(const AboutDialog);
        return d->m_descrLabel->text();
    }

    void AboutDialog::setDescription(const QString &descr)
    {
        Q_D(AboutDialog);
        d->m_descrLabel->setText(descr);
    }

    QString AboutDialog::copyright() const
    {
        Q_D(const AboutDialog);
        return d->m_copyrightLabel->text();
    }

    void AboutDialog::setCopyright(const QString &text)
    {
        Q_D(AboutDialog);
        d->m_copyrightLabel->setText(text);
    }

    QString AboutDialog::licenseText() const
    {
    }

    void AboutDialog::setLicenseText(const QString &text)
    {
    }

    QStringList AboutDialog::authors() const
    {
    }

    void AboutDialog::setAuthors(const QStringList &authors)
    {
    }
}
