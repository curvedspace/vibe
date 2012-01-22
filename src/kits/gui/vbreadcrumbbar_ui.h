/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (C) 2009 Dzimi Mve Alexandre
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Dzimi Mve Alexandre <dzimiwine@gmail.com>
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef VBREADCRUMBBAR_UI_H
#define VBREADCRUMBBAR_UI_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the on API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QComboBox>

#include "vabstractbreadcrumbmodel.h"
#include "vbreadcrumbbar.h"

class QAction;
class QLabel;

class VBreadCrumbComboBox;
class VBreadCrumbComboBoxContainer;
class VBreadCrumbLabel;

class VIBE_EXPORT VBreadCrumbItem
{
public:
    enum Type {
        EmptyArea,
        Indicator,
        Label
    };

    VBreadCrumbItem(VBreadCrumbComboBoxContainer *, VBreadCrumbItem::Type);
    virtual ~VBreadCrumbItem() {}

    inline QRect rect() const {
        return m_rect;
    }
    inline void setRect(const QRect &rect) {
        m_rect = rect;
    }

    inline void setVisible(bool visible) {
        m_visible = visible;
    }
    inline bool isVisible() {
        return m_visible;
    }

    inline VBreadCrumbComboBoxContainer *container() const {
        return m_container;
    }
    inline Type type() const {
        return m_type;
    }

    virtual void clicked(const QPoint &pos) = 0;

private:
    bool m_visible;
    VBreadCrumbItem::Type m_type;
    VBreadCrumbComboBoxContainer *m_container;
    QRect m_rect;
};

class VIBE_EXPORT VBreadCrumbEmptyArea : public VBreadCrumbItem
{
public:
    VBreadCrumbEmptyArea(VBreadCrumbComboBoxContainer *);
    ~VBreadCrumbEmptyArea() {}

    void clicked(const QPoint &pos);
};

class VIBE_EXPORT VBreadCrumbIndicator : public VBreadCrumbItem
{
public:
    VBreadCrumbIndicator(VBreadCrumbLabel *, VBreadCrumbComboBoxContainer *);
    virtual ~VBreadCrumbIndicator();

    inline bool isTruncated() const {
        return m_trunc;
    }
    inline void setTruncated(bool trunc) {
        m_trunc = trunc;
    }

    inline VBreadCrumbLabel *label() const {
        return m_label;
    }

    void clicked(const QPoint &pos);

private:
    bool m_trunc;
    VBreadCrumbLabel *m_label;
};

class VIBE_EXPORT VBreadCrumbLabel : public VBreadCrumbItem
{
public:
    VBreadCrumbLabel(VBreadCrumbIndicator *, VBreadCrumbComboBoxContainer *, const VBreadCrumbModelNode &);
    virtual ~VBreadCrumbLabel() { }

    inline VBreadCrumbModelNode node() const {
        return m_node;
    }

    inline VBreadCrumbIndicator *indicator() {
        return m_indicator;
    }
    inline void setIndicator(VBreadCrumbIndicator *indicator)  {
        m_indicator = indicator;
    }

    void clicked(const QPoint &pos);

private:
    VBreadCrumbModelNode m_node;
    VBreadCrumbIndicator *m_indicator;
};

class VIBE_EXPORT VBreadCrumbComboBoxContainer : public QWidget
{
    Q_OBJECT
public:
    VBreadCrumbComboBoxContainer(VBreadCrumbComboBox *comboBox);
    ~VBreadCrumbComboBoxContainer();

    inline VBreadCrumbComboBox *comboBox() const {
        return m_comboBox;
    }

    void splitPath(const QString &);

    inline VBreadCrumbModelNodeList nodeList() const {
        return m_nodeList;
    }
    void refresh();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void leaveEvent(QEvent *);

private:
    friend class VBreadCrumbComboBox;

    void clear();
    void clearAll();
    void updateGeometries();
    int itemAt(const QPoint &pos);

    QList< VBreadCrumbItem * > m_items;
    VBreadCrumbModelNodeList m_nodeList;
    VBreadCrumbIndicator *m_rootIndicator;
    VBreadCrumbEmptyArea *m_emptyArea;

    int m_hoverItem;
    int m_downItem;
    VBreadCrumbComboBox *m_comboBox;
    bool m_clicked;
};

class VIBE_EXPORT VBreadCrumbIconWidget : public QWidget
{
    Q_OBJECT
public:
    VBreadCrumbIconWidget(VBreadCrumbComboBox *comboBox);

    inline QPixmap pixmap() const {
        return m_pixmap;
    }
    inline void setPixmap(const QPixmap &p) {
        m_pixmap = p;
        update();
    }

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

private slots:
    void slotCopyAddress();
    void slotEditAddress();

private:
    VBreadCrumbComboBox *m_comboBox;
    QPixmap m_pixmap;
    bool m_clicked;
};

class VIBE_EXPORT VBreadCrumbComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit VBreadCrumbComboBox(VBreadCrumbBar *parent);
    ~VBreadCrumbComboBox();

    inline bool isFlat() const {
        return m_flat;
    }
    void setFlat(bool f);
    inline VBreadCrumbBar *bar() const {
        return m_bar;
    }
    inline QString location() const {
        return m_location;
    }
    void setLocation(const QString &);

    void showPopup();

    void showVBreadCrumbs(bool popupError = true);
    void edit();

    inline VBreadCrumbComboBoxContainer *container() const {
        return m_container;
    }
    void updateGeometries();

private slots:
    void slotSetLocation(QAction *);
    void slotHandleEditTextChanged();
    void slotActivated();

protected:
    bool event(QEvent *);
    void resizeEvent(QResizeEvent *);
    void focusOutEvent(QFocusEvent *);

private:
    bool m_flat;
    QString m_location;
    VBreadCrumbIconWidget *m_iconLabel;
    VBreadCrumbComboBoxContainer *m_container;
    VBreadCrumbBar *m_bar;
    bool m_clicked;
};

#endif // VBREADCRUMBBAR_UI_H
