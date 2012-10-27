/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPL2$
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef VNAVIGATIONBAR_H
#define VNAVIGATIONBAR_H

#include <QTreeView>

#include <VibeWidgets/VibeWidgetsExport>

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
class VIBEWIDGETS_EXPORT VNavigationBar : public QTreeView
{
    Q_OBJECT
public:
    explicit VNavigationBar(QWidget *parent = 0);

protected:
    virtual void drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const;
};

/** @}*/

#endif // VNAVIGATIONBAR_H
