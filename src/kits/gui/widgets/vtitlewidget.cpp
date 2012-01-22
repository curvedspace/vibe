/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 * Copyright (C) 2007-2009 Urs Wolfer
 * Copyright (C) 2007 Michaël Larouche
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *	Michaël Larouche <larouche@kde.org>
 *	Urs Wolfer <uwolfer@kde.org>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
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

#include <QTimer>
#include <QMouseEvent>
#include <QFrame>
#include <QIcon>
#include <QLabel>
#include <QLayout>
#include <QTextDocument>

#include "vtitlewidget.h"

/*
 * VTitleWidget::Private
 */

class VTitleWidget::Private
{
public:
    Private(VTitleWidget *parent) :
        q(parent),
        autoHideTimeout(0),
        messageType(InfoMessage) {
    }

    QString textStyleSheet() const {
        return QString("QLabel { font-weight: bold; color: %1}").arg(q->palette().color(QPalette::WindowText).name());
    }

    QString commentStyleSheet() const {
        QString styleSheet;
        switch (messageType) {
                //FIXME: we need the usability color styles to implement different
                //       yet palette appropriate colours for the different use cases!
                //       also .. should we include an icon here,
                //       perhaps using the imageLabel?
            case InfoMessage:
            case WarningMessage:
            case ErrorMessage:
                styleSheet = QString("QLabel { color: palette(%1); background: palette(%2); }").arg(q->palette().color(QPalette::HighlightedText).name()).arg(q->palette().color(QPalette::Highlight).name());
                break;
            case PlainMessage:
            default:
                break;
        }
        return styleSheet;
    }

    VTitleWidget *q;
    QGridLayout *headerLayout;
    QLabel *imageLabel;
    QLabel *textLabel;
    QLabel *commentLabel;
    int autoHideTimeout;
    MessageType messageType;

    /**
     * @brief Get the icon name from the icon type
     * @param type icon type from the enum
     * @return named icon as QString
     */
    QString iconTypeToIconName(VTitleWidget::MessageType type);

    void _q_timeoutFinished() {
        q->setVisible(false);
    }
};

QString VTitleWidget::Private::iconTypeToIconName(VTitleWidget::MessageType type)
{
    switch (type) {
        case VTitleWidget::InfoMessage:
            return QLatin1String("dialog-information");
            break;
        case VTitleWidget::ErrorMessage:
            return QLatin1String("dialog-error");
            break;
        case VTitleWidget::WarningMessage:
            return QLatin1String("dialog-warning");
            break;
        case VTitleWidget::PlainMessage:
            break;
    }

    return QString();
}

/*
 * VTitleWidget
 */

VTitleWidget::VTitleWidget(QWidget *parent) :
    QWidget(parent),
    d(new Private(this))
{
    QFrame *titleFrame = new QFrame(this);
    titleFrame->setAutoFillBackground(true);
    titleFrame->setFrameShape(QFrame::StyledPanel);
    titleFrame->setFrameShadow(QFrame::Plain);
    titleFrame->setBackgroundRole(QPalette::Base);

    // default image / text part start
    d->headerLayout = new QGridLayout(titleFrame);
    d->headerLayout->setColumnStretch(0, 1);
    d->headerLayout->setMargin(6);

    d->textLabel = new QLabel(titleFrame);
    d->textLabel->setVisible(false);
    d->textLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);

    d->imageLabel = new QLabel(titleFrame);
    d->imageLabel->setVisible(false);

    d->headerLayout->addWidget(d->textLabel, 0, 0);
    d->headerLayout->addWidget(d->imageLabel, 0, 1, 1, 2);

    d->commentLabel = new QLabel(titleFrame);
    d->commentLabel->setVisible(false);
    d->commentLabel->setOpenExternalLinks(true);
    d->commentLabel->setWordWrap(true);
    d->commentLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
    d->headerLayout->addWidget(d->commentLabel, 1, 0);

    // default image / text part end

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(titleFrame);
    mainLayout->setMargin(0);
    setLayout(mainLayout);
}

VTitleWidget::~VTitleWidget()
{
    delete d;
}

bool VTitleWidget::eventFilter(QObject *object, QEvent *event)
{
    // Hide message label on click
    if (d->autoHideTimeout > 0 &&
            event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent && mouseEvent->button() == Qt::LeftButton) {
            setVisible(false);
            return true;
        }
    }

    return QWidget::eventFilter(object, event);
}

void VTitleWidget::setWidget(QWidget *widget)
{
    d->headerLayout->addWidget(widget, 2, 0, 1, 2);
}

QString VTitleWidget::text() const
{
    return d->textLabel->text();
}

QString VTitleWidget::comment() const
{
    return d->commentLabel->text();
}

const QPixmap *VTitleWidget::pixmap() const
{
    return d->imageLabel->pixmap();
}

void VTitleWidget::setBuddy(QWidget *buddy)
{
    d->textLabel->setBuddy(buddy);
}

void VTitleWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    if (e->type() == QEvent::PaletteChange) {
        d->textLabel->setStyleSheet(d->textStyleSheet());
        d->commentLabel->setStyleSheet(d->commentStyleSheet());
    }
}

void VTitleWidget::setText(const QString &text, Qt::Alignment alignment)
{
    d->textLabel->setVisible(!text.isNull());

    if (!Qt::mightBeRichText(text)) {
        d->textLabel->setStyleSheet(d->textStyleSheet());
    }

    d->textLabel->setText(text);
    d->textLabel->setAlignment(alignment);
    show();
}

void VTitleWidget::setText(const QString &text, MessageType type)
{
    setPixmap(type);
    setText(text);
}

void VTitleWidget::setComment(const QString &comment, MessageType type)
{
    d->commentLabel->setVisible(!comment.isNull());

    //TODO: should we override the current icon with the corresponding MessageType icon?
    d->messageType = type;
    d->commentLabel->setStyleSheet(d->commentStyleSheet());
    d->commentLabel->setText(comment);
    show();
}

void VTitleWidget::setPixmap(const QPixmap &pixmap, ImageAlignment alignment)
{
    d->imageLabel->setVisible(!pixmap.isNull());

    d->headerLayout->removeWidget(d->textLabel);
    d->headerLayout->removeWidget(d->commentLabel);
    d->headerLayout->removeWidget(d->imageLabel);

    if (alignment == ImageLeft) {
        // swap the text and image labels around
        d->headerLayout->addWidget(d->imageLabel, 0, 0, 2, 1);
        d->headerLayout->addWidget(d->textLabel, 0, 1);
        d->headerLayout->addWidget(d->commentLabel, 1, 1);
        d->headerLayout->setColumnStretch(0, 0);
        d->headerLayout->setColumnStretch(1, 1);
    } else {
        d->headerLayout->addWidget(d->textLabel, 0, 0);
        d->headerLayout->addWidget(d->commentLabel, 1, 0);
        d->headerLayout->addWidget(d->imageLabel, 0, 1, 2, 1);
        d->headerLayout->setColumnStretch(1, 0);
        d->headerLayout->setColumnStretch(0, 1);
    }

    d->imageLabel->setPixmap(pixmap);
}


void VTitleWidget::setPixmap(const QString &icon, ImageAlignment alignment)
{
    setPixmap(QIcon::fromTheme(icon), alignment);
}

void VTitleWidget::setPixmap(const QIcon &icon, ImageAlignment alignment)
{
    // TODO: We definitely need to take this from a settings singleton
    setPixmap(icon.pixmap(22, 22), alignment);
}

void VTitleWidget::setPixmap(MessageType type, ImageAlignment alignment)
{
    setPixmap(QIcon::fromTheme(d->iconTypeToIconName(type)), alignment);
}

int VTitleWidget::autoHideTimeout() const
{
    return d->autoHideTimeout;
}

void VTitleWidget::setAutoHideTimeout(int msecs)
{
    d->autoHideTimeout = msecs;

    if (msecs > 0)
        installEventFilter(this);
    else
        removeEventFilter(this);
}

void VTitleWidget::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)

    if (d->autoHideTimeout > 0)
        QTimer::singleShot(d->autoHideTimeout, this, SLOT(_q_timeoutFinished()));
}

#include "vtitlewidget.moc"
