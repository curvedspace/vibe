#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QtCore/QUrl>
#include <QtGui/QDialog>

namespace QubeGui
{
    class AboutDialogPrivate;

    class AboutDialog : public QDialog
    {
        Q_OBJECT
        Q_PROPERTY(QPixmap logo READ logo WRITE setLogo)
        Q_PROPERTY(QString description READ description WRITE setDescription)
        Q_PROPERTY(QString copyright READ copyright WRITE setCopyright)
        Q_PROPERTY(QString license READ licenseText WRITE setLicenseText)
        Q_PROPERTY(QStringList authors READ authors WRITE setAuthors)
        Q_PROPERTY(QUrl link READ link WRITE setLink)
    public:
        explicit AboutDialog(QWidget *parent = 0);
        virtual ~AboutDialog();

        QPixmap *logo() const;
        void setLogo(const QPixmap &pixmap);

        QString description() const;
        void setDescription(const QString &descr);

        QString copyright() const;
        void setCopyright(const QString &text);

        QString licenseText() const;
        void setLicenseText(const QString &text);

        QStringList authors() const;
        void setAuthors(const QStringList &authors);

        QUrl link() const;
        void setLink(const QUrl &url);

    protected:
        AboutDialogPrivate * const d_ptr;

    private:
        Q_DECLARE_PRIVATE(AboutDialog)
    };
}

#endif // ABOUTDIALOG_H
