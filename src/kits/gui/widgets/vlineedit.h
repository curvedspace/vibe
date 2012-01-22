/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 * Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1
 * as published by the Free Software Foundation.
 *
 * Vibe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Vibe.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef VLINEEDIT_H
#define VLINEEDIT_H

#include <QLineEdit>

#include <VibeCore/VGlobal>

/** \addtogroup gui Gui Kit
 *  @{
 */

class VLineEditPrivate;

/**
 * \class VLineEdit vlineedit.h <VLineEdit>
 *
 * \brief A line edit with an embedded pixmap on one side that is connected to
 * a menu.
 *
 * \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VIBE_EXPORT VLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(VLineEdit)
    Q_ENUMS(Side)
public:
    enum Side {
        LeftSide = 0,
        RightSide
    };

    /**
     * Constructs a new line edit object.
     */
    explicit VLineEdit(QWidget *parent = 0);

    /**
     * @return pixmap for @p side.
     */
    QPixmap buttonPixmap(Side side) const;

    /**
     * Sets pixmap for button on @p side side.
     */
    void setButtonPixmap(Side side, const QPixmap &pixmap);

    /**
     * @return menu for the button on @p side side.
     */
    QMenu *buttonMenu(Side side) const;

    /**
     * Sets the menu for the button on @p side side.
     */
    void setButtonMenu(Side side, QMenu *menu);

    /**
     * @return whether the button on @p side side is visible.
     */
    bool isButtonVisible(Side side) const;

    /**
     * Sets the button on @p side side visible.
     */
    void setButtonVisible(Side side, bool visible);

    /**
     * Sets the tooltip for the button on @p side side to @p toolTip.
     */
    void setButtonToolTip(Side side, const QString &toolTip);

    /**
     * Sets the focus policy to @p policy for the button on @p side side.
     */
    void setButtonFocusPolicy(Side side, Qt::FocusPolicy policy);

    /**
     * @return whether menu triggers the focus when tab is pressed.
     */
    bool hasMenuTabFocusTrigger(Side side) const;

    /**
     * Sets whether the menu has tab focus.
     */
    void setMenuTabFocusTrigger(Side side, bool value);

    /**
     * @return whether the button on @p side side has auto-hide.
     */
    bool hasAutoHideButton(Side side) const;

    /**
     * Sets auto-hide for the button on @p side side.
     */
    void setAutoHideButton(Side side, bool value);

signals:
    void buttonClicked(VLineEdit::Side);
    void leftButtonClicked();
    void rightButtonClicked();

protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    VLineEditPrivate *const d_ptr;

private slots:
    void slotCheckButtons(const QString &text);
    void slotIconClicked();
};

/** @}*/

#endif // VLINEEDIT_H
