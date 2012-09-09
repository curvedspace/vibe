/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2007 Kevin Ottens
 * Copyright (c) 2008 Rafael Fernández López
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Rafael Fernández López <ereslibre@kde.org>
 *    Kevin Ottens <ervin@kde.org>
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
#include <QPainter>
#include <QStyle>
#include <QTimeLine>

#include <VibeWidgets/VFilePlacesModel>
#include <VibeWidgets/VFilePlacesView>

#include <VibeHardware/VDevice>

#include "vfileplacesviewdelegate.h"

const quint32 kLateralMargin = 4;

namespace VPrivate
{
    FilePlacesViewDelegate::FilePlacesViewDelegate(VFilePlacesView *parent) :
        QAbstractItemDelegate(parent),
        m_view(parent),
        m_iconSize(24),
        m_appearingIconSize(0),
        m_appearingOpacity(0.0),
        m_disappearingIconSize(0),
        m_disappearingOpacity(0.0)
    {
    }

    FilePlacesViewDelegate::~FilePlacesViewDelegate()
    {
    }

    QSize FilePlacesViewDelegate::sizeHint(const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const
    {
        int iconSize = m_iconSize;
        if (m_appearingItems.contains(index))
            iconSize = m_appearingIconSize;
        else
            iconSize = m_disappearingIconSize;

        const VFilePlacesModel *model = static_cast<const VFilePlacesModel *>(index.model());
        VDevice device = model->deviceForIndex(index);

        return QSize(option.rect.width(), option.fontMetrics.height() / 2
                     + qMax(iconSize, option.fontMetrics.height()));
    }

    void FilePlacesViewDelegate::paint(QPainter *painter,
                                       const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
    {
        painter->save();

#if 0
        // Set opacity for appearing and disappearing items
        if (m_appearingItems.contains(index))
            painter->setOpacity(m_appearingOpacity);
        else
            painter->setOpacity(m_disappearingOpacity);
#endif

        const bool isChild = index.model()->hasChildren(index);
        bool isLTR = option.direction == Qt::LeftToRight;
        const VFilePlacesModel *model = static_cast<const VFilePlacesModel *>(index.model());

        // Draw primitive element
        QStyleOptionViewItemV4 opt = option;
        QStyle *style = opt.widget ? opt.widget->style() : QApplication::style();
        style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, opt.widget);

        // Draw icon
        QIcon icon = model->icon(index);
        QPixmap pm = icon.pixmap(m_iconSize, m_iconSize);
        QPoint point(isLTR ? option.rect.left() + kLateralMargin
                     : option.rect.right() - kLateralMargin - m_iconSize, option.rect.top() + (option.rect.height() - m_iconSize) / 2);
        painter->drawPixmap(point, pm);

        // Text color when the item is selected
        if (option.state & QStyle::State_Selected) {
            QPalette::ColorGroup cg = QPalette::Active;
            if (!(option.state & QStyle::State_Enabled))
                cg = QPalette::Disabled;
            else if (!(option.state & QStyle::State_Active))
                cg = QPalette::Inactive;
            painter->setPen(option.palette.color(cg, QPalette::HighlightedText));
        }

        // Draw top-level item's text with bold font
        QFont textFont;
        if (!index.parent().isValid())
            textFont.setBold(true);
        textFont.setPointSize(textFont.pointSize() * 0.80);
        painter->setFont(textFont);

        QRect rectText;

        // TODO: capacity bar

        if (!index.parent().isValid())
            rectText = option.rect.adjusted(15, 0, 0, 0);
        else
            rectText = QRect(isLTR ? m_iconSize + kLateralMargin * 2 + option.rect.left()
                             : 0, option.rect.top(), option.rect.width() - m_iconSize - kLateralMargin * 2, option.rect.height());
        painter->setFont(textFont);
        painter->drawText(rectText, Qt::AlignLeft | Qt::AlignVCenter,
                          option.fontMetrics.elidedText(index.model()->data(index).toString(), Qt::ElideRight, rectText.width()));

        painter->restore();
    }

    int FilePlacesViewDelegate::iconSize() const
    {
        return m_iconSize;
    }

    void FilePlacesViewDelegate::setIconSize(int newSize)
    {
        m_iconSize = newSize;
    }

    void FilePlacesViewDelegate::addAppearingItem(const QModelIndex &index)
    {
        m_appearingItems << index;
    }

    void FilePlacesViewDelegate::setAppearingItemProgress(qreal value)
    {
        if (value <= 0.25) {
            m_appearingOpacity = 0.0;
            m_appearingIconSize = iconSize() * value * 4;

            // Appearing icon size can't exceed maximum size
            if (m_appearingIconSize >= m_iconSize)
                m_appearingIconSize = m_iconSize;
        } else {
            m_appearingOpacity = 0.0;
            m_appearingIconSize = (value - 0.25) * 4 / 3;

            // If progress is complete remove all items
            if (value >= 1.0)
                m_appearingItems.clear();
        }
    }

    void FilePlacesViewDelegate::addDisappearingItem(const QModelIndex &index)
    {
        m_disappearingItems << index;
    }

    void FilePlacesViewDelegate::setDisappearingItemProgress(qreal value)
    {
        value = 1.0 - value;

        if (value <= 0.25) {
            m_disappearingOpacity = 0.0;
            m_disappearingIconSize = iconSize() * value * 4;

            // Disappearing icon size can't exceed maximum size
            if (m_disappearingIconSize >= m_iconSize)
                m_disappearingIconSize = m_iconSize;

            // If progress is complete remove all items
            if (value <= 0.0)
                m_disappearingItems.clear();
        } else {
            m_disappearingIconSize = m_iconSize;
            m_disappearingOpacity = (value - 0.25) * 4 / 3;
        }
    }

    void FilePlacesViewDelegate::addFadeAnimation(const QModelIndex &index, QTimeLine *timeLine)
    {
        m_timeLineMap.insert(index, timeLine);
        m_timeLineInverseMap.insert(timeLine, index);
    }

    void FilePlacesViewDelegate::removeFadeAnimation(const QModelIndex &index)
    {
        QTimeLine *timeLine = m_timeLineMap.value(index, 0);
        m_timeLineMap.remove(index);
        m_timeLineInverseMap.remove(timeLine);
    }

    QModelIndex FilePlacesViewDelegate::indexForFadeAnimation(QTimeLine *timeLine) const
    {
        return m_timeLineInverseMap.value(timeLine, QModelIndex());
    }

    QTimeLine *FilePlacesViewDelegate::fadeAnimationForIndex(const QModelIndex &index) const
    {
        return m_timeLineMap.value(index, 0);
    }

    qreal FilePlacesViewDelegate::contentsOpacity(const QModelIndex &index) const
    {
        QTimeLine *timeLine = fadeAnimationForIndex(index);
        if (timeLine)
            return timeLine->currentValue();
        return 0;
    }
}

#include "moc_vfileplacesviewdelegate.cpp"
