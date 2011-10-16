
#ifndef ABOUTDIALOG_P_H
#define ABOUTDIALOG_P_H

#include <QtCore/QObject>

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

        AboutDialog * const q_ptr;
        Q_DECLARE_PUBLIC(AboutDialog)

        QVBoxLayout *m_verticalLayout;
        QLabel *m_logoLabel;
        QLabel *m_appVerLabel;
        QLabel *m_descrLabel;
        QLabel *m_copyrightLabel;
        QLabel *m_websiteLabel;
        QSpacerItem *m_verticalSpacer;
        QHBoxLayout *m_horizontalLayout;
        QSpacerItem *m_horizontalSpacer;
        QPushButton *m_moreInfoButton;
        QPushButton *m_closeButton;

    private Q_SLOTS:
        void slotMoreInformation();

        void slotClose();
    };
}

#endif // ABOUTDIALOG_P_H
