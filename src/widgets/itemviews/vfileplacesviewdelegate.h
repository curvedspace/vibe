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

#ifndef PRIVATE_FILEPLACESVIEWDELEGATE_H
#define PRIVATE_FILEPLACESVIEWDELEGATE_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Vibe API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QAbstractItemDelegate>

class QTimeLine;

class VFilePlacesView;

namespace VPrivate
{
    class FilePlacesViewDelegate : public QAbstractItemDelegate
    {
        Q_OBJECT
    public:
        explicit FilePlacesViewDelegate(VFilePlacesView *parent);
        virtual ~FilePlacesViewDelegate();

        virtual QSize sizeHint(const QStyleOptionViewItem &option,
                               const QModelIndex &index) const;

        virtual void paint(QPainter *painter,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;

        int iconSize() const;
        void setIconSize(int newSize);

        void addAppearingItem(const QModelIndex &index);
        void setAppearingItemProgress(qreal value);

        void addDisappearingItem(const QModelIndex &index);
        void setDisappearingItemProgress(qreal value);

        void addFadeAnimation(const QModelIndex &index, QTimeLine *timeLine);
        void removeFadeAnimation(const QModelIndex &index);

        QModelIndex indexForFadeAnimation(QTimeLine *timeLine) const;
        QTimeLine *fadeAnimationForIndex(const QModelIndex &index) const;

        qreal contentsOpacity(const QModelIndex &index) const;

    private:
        VFilePlacesView *m_view;
        int m_iconSize;

        QList<QPersistentModelIndex> m_appearingItems;
        int m_appearingIconSize;
        qreal m_appearingOpacity;

        QList<QPersistentModelIndex> m_disappearingItems;
        int m_disappearingIconSize;
        qreal m_disappearingOpacity;

        QMap<QPersistentModelIndex, QTimeLine *> m_timeLineMap;
        QMap<QTimeLine *, QPersistentModelIndex> m_timeLineInverseMap;
    };
}

#endif // PRIVATE_FILEPLACESVIEWDELEGATE_H
