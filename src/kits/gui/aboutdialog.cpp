#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    ui->logoLabel->setText(QString::null);
    ui->appVerLabel->setText(QString("<strong>%1 %2</strong>")
                             .arg(qApp->applicationName())
                             .arg(qApp->applicationVersion()));
    ui->copyrightLabel->setText(QString::null);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

QIcon AboutDialog::logoIcon() const
{
    return QIcon(ui->logoLabel->pixmap());
}

void AboutDialog::setLogoIcon(const QIcon &icon)
{
    ui->logoLabel->setPixmap(icon);
}

QString AboutDialog::copyright() const
{
    return ui->copyrightLabel->text();
}

void AboutDialog::setCopyright(const QString &text)
{
    ui->copyrightLabel->setText(text);
}
