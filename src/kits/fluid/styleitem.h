/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef STYLEITEM_H
#define STYLEITEM_H

#include <QEvent>
#include <QStyle>
#include <QQuickPaintedItem>

class StyleItem: public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(bool sunken READ sunken WRITE setSunken NOTIFY sunkenChanged)
    Q_PROPERTY(bool raised READ raised WRITE setRaised NOTIFY raisedChanged)
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedChanged)
    Q_PROPERTY(bool hasFocus READ hasFocus WRITE sethasFocus NOTIFY hasFocusChanged)
    Q_PROPERTY(bool on READ on WRITE setOn NOTIFY onChanged)
    Q_PROPERTY(bool hover READ hover WRITE setHover NOTIFY hoverChanged)
    Q_PROPERTY(bool horizontal READ horizontal WRITE setHorizontal NOTIFY horizontalChanged)

    Q_PROPERTY(QString elementType READ elementType WRITE setElementType NOTIFY elementTypeChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QString activeControl READ activeControl WRITE setActiveControl NOTIFY activeControlChanged)
    Q_PROPERTY(QString info READ info WRITE setInfo NOTIFY infoChanged)
    Q_PROPERTY(QString style READ style NOTIFY styleChanged)
    Q_PROPERTY(QString hint READ hint WRITE setHint NOTIFY hintChanged)

    // For range controls
    Q_PROPERTY(int minimum READ minimum WRITE setMinimum NOTIFY minimumChanged)
    Q_PROPERTY(int maximum READ maximum WRITE setMaximum NOTIFY maximumChanged)
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(int step READ step WRITE setStep NOTIFY stepChanged)
    Q_PROPERTY(int paintMargins READ paintMargins WRITE setPaintMargins NOTIFY paintMarginsChanged)

    Q_PROPERTY(int implicitWidth READ implicitWidth() NOTIFY implicitWidthChanged)
    Q_PROPERTY(int implicitHeight READ implicitHeight() NOTIFY implicitHeightChanged)
    Q_PROPERTY(int contentWidth READ contentWidth() WRITE setContentWidth NOTIFY contentWidthChanged)
    Q_PROPERTY(int contentHeight READ contentHeight() WRITE setContentHeight NOTIFY contentHeightChanged)

    Q_PROPERTY(QString fontFamily READ fontFamily NOTIFY fontHeightChanged)
    Q_PROPERTY(double fontPointSize READ fontPointSize NOTIFY fontHeightChanged)
    Q_PROPERTY(int fontHeight READ fontHeight NOTIFY fontHeightChanged)
public:
    enum Type {
        Undefined,
        Button,
        RadioButton,
        CheckBox,
        ComboBox,
        ComboBoxItem,
        Dial,
        ToolBar,
        ToolButton,
        Tab,
        TabFrame,
        Frame,
        FocusFrame,
        SpinBox,
        Slider,
        ScrollBar,
        ProgressBar,
        Edit,
        GroupBox,
        Header,
        Item,
        ItemRow,
        Splitter,
        Menu,
        MenuItem,
        Widget,
        StatusBar,
        ScrollAreaCorner
    };

    StyleItem(QQuickItem *parent = 0);
    ~StyleItem();

    virtual void paint(QPainter *painter);

    bool sunken() const {
        return m_sunken;
    }
    bool raised() const {
        return m_raised;
    }
    bool active() const {
        return m_active;
    }
    bool selected() const {
        return m_selected;
    }
    bool hasFocus() const {
        return m_focus;
    }
    bool on() const {
        return m_on;
    }
    bool hover() const {
        return m_hover;
    }
    bool horizontal() const {
        return m_horizontal;
    }

    int minimum() const {
        return m_minimum;
    }
    int maximum() const {
        return m_maximum;
    }
    int step() const {
        return m_step;
    }
    int value() const {
        return m_value;
    }
    int paintMargins() const {
        return m_paintMargins;
    }

    QString elementType() const {
        return m_type;
    }
    QString text() const {
        return m_text;
    }
    QString activeControl() const {
        return m_activeControl;
    }
    QString info() const {
        return m_info;
    }
    QString hint() const {
        return m_hint;
    }
    QString style() const;

    void setSunken(bool sunken) {
        if (m_sunken != sunken) {
            m_sunken = sunken;
            emit sunkenChanged();
        }
    }
    void setRaised(bool raised) {
        if (m_raised != raised) {
            m_raised = raised;
            emit raisedChanged();
        }
    }
    void setActive(bool active) {
        if (m_active != active) {
            m_active = active;
            emit activeChanged();
        }
    }
    void setSelected(bool selected) {
        if (m_selected != selected) {
            m_selected = selected;
            emit selectedChanged();
        }
    }
    void sethasFocus(bool focus) {
        if (m_focus != focus) {
            m_focus = focus;
            emit hasFocusChanged();
        }
    }
    void setOn(bool on) {
        if (m_on != on) {
            m_on = on ;
            emit onChanged();
        }
    }
    void setHover(bool hover) {
        if (m_hover != hover) {
            m_hover = hover ;
            emit hoverChanged();
        }
    }
    void setHorizontal(bool horizontal) {
        if (m_horizontal != horizontal) {
            m_horizontal = horizontal;
            emit horizontalChanged();
        }
    }
    void setMinimum(int minimum) {
        if (m_minimum != minimum) {
            m_minimum = minimum;
            emit minimumChanged();
        }
    }
    void setMaximum(int maximum) {
        if (m_maximum != maximum) {
            m_maximum = maximum;
            emit maximumChanged();
        }
    }
    void setValue(int value) {
        if (m_value != value) {
            m_value = value;
            emit valueChanged();
        }
    }
    void setStep(int step) {
        if (m_step != step) {
            m_step = step;
            emit stepChanged();
        }
    }
    void setPaintMargins(int value) {
        Q_UNUSED(value)
#ifdef Q_WS_WIN //only vista style needs this hack
        if (m_paintMargins != value)
            m_paintMargins = value;
#endif
    }
    void setElementType(const QString &str);
    void setText(const QString &str) {
        if (m_text != str) {
            m_text = str;
            emit textChanged();
        }
    }
    void setActiveControl(const QString &str) {
        if (m_activeControl != str) {
            m_activeControl = str;
            emit activeControlChanged();
        }
    }
    void setInfo(const QString &str) {
        if (m_info != str) {
            m_info = str;
            emit infoChanged();
        }
    }
    void setHint(const QString &str) {
        if (m_hint != str) {
            m_hint = str;
            emit hintChanged();
        }
    }

    bool eventFilter(QObject *, QEvent *);
    virtual void initStyleOption();
    QWidget *widget() {
        return m_dummywidget;
    }

    int fontHeight();
    QString fontFamily();
    double fontPointSize();

    int implicitHeight();
    int implicitWidth();

    int contentWidth() const {
        return m_contentWidth;
    }

    int contentHeight() const {
        return m_contentHeight;
    }

public slots:
    int pixelMetric(const QString &);
    QVariant styleHint(const QString &);
    void updateSizeHint();
    void updateItem() {
        update();
    }
    QString hitTest(int x, int y);
    QRect subControlRect(const QString &subcontrolString);
    QString elidedText(const QString &text, int elideMode, int width);
    int textWidth(const QString &);
    bool hasThemeIcon(const QString &) const;

    void setContentWidth(int arg) {
        if (m_contentWidth != arg) {
            m_contentWidth = arg;
            emit contentWidthChanged(arg);
        }
    }

    void setContentHeight(int arg) {
        if (m_contentHeight != arg) {
            m_contentHeight = arg;
            emit contentHeightChanged(arg);
        }
    }

signals:
    void elementTypeChanged();
    void textChanged();
    void sunkenChanged();
    void raisedChanged();
    void activeChanged();
    void selectedChanged();
    void hasFocusChanged();
    void onChanged();
    void hoverChanged();
    void horizontalChanged();
    void minimumChanged();
    void maximumChanged();
    void stepChanged();
    void valueChanged();
    void activeControlChanged();
    void infoChanged();
    void styleChanged();
    void paintMarginsChanged();
    void hintChanged();
    void fontHeightChanged();

    void implicitHeightChanged(int arg);
    void implicitWidthChanged(int arg);

    void contentWidthChanged(int arg);
    void contentHeightChanged(int arg);

private:
    QSize sizeFromContents(int width, int height);

protected:
    QWidget *m_dummywidget;
    QStyleOption *m_styleoption;
    Type m_itemType;

    QString m_type;
    QString m_text;
    QString m_activeControl;
    QString m_info;
    QString m_hint;

    bool m_sunken;
    bool m_raised;
    bool m_active;
    bool m_selected;
    bool m_focus;
    bool m_hover;
    bool m_on;
    bool m_horizontal;
    bool m_sharedWidget;

    int m_minimum;
    int m_maximum;
    int m_value;
    int m_step;
    int m_paintMargins;

    int m_implicitWidth;
    int m_implicitHeight;
    int m_contentWidth;
    int m_contentHeight;
};

#endif //STYLEITEM_H
