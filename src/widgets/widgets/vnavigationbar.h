/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef VNAVIGATIONBAR_H
#define VNAVIGATIONBAR_H

#include <QTreeView>

#include <VibeCore/VGlobal>

/** \addtogroup gui Gui Kit
 *  @{
 */

/**
 * \class VNavigationBar vnavigationbar.h <VNavigationBar>
 *
 * \brief Navigation bar.
 *
 * This class provides a nice navigation bar.
 */
class VIBE_EXPORT VNavigationBar : public QTreeView
{
    Q_OBJECT
public:
    explicit VNavigationBar(QWidget *parent = 0);

protected:
    virtual void drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const;
};

/** @}*/

#endif // VNAVIGATIONBAR_H
