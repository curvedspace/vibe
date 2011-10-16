
#ifndef ABOUTDIALOG_P_H
#define ABOUTDIALOG_P_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QUrl>

class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class QPushButton;
class QSpacerItem;

namespace QubeGui
{
    class AboutDialog;

    class AboutDialogPrivate : public QObject
    {
        Q_OBJECT
    public:
        AboutDialogPrivate(AboutDialog *parent);

        QString licenseText() const { return m_licenseText; }
        void setLicenseText(const QString &text);

        QStringList authors() const { return m_authors; }
        void setAuthors(const QStringList &authors);

        QUrl link() const { return m_link; }
        void setLink(const QUrl &url);

        AboutDialog * const q_ptr;
        Q_DECLARE_PUBLIC(AboutDialog)

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

    private:
        QString m_licenseText;
        QStringList m_authors;
        QUrl m_link;

    private Q_SLOTS:
        void slotMoreInformation();

        void slotClose();
    };
}

#endif // ABOUTDIALOG_P_H
