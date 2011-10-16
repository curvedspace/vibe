#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
    class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();

    QIcon logoIcon() const;
    void setLogoIcon(const QIcon &icon);

    QString copyright() const;
    void setCopyright(const QString &text);

private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
