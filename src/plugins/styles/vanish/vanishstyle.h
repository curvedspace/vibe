/****************************************************************************
 * This file is part of Vanish.
 *
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
 * Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies)
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vanish is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1
 * as published by the Free Software Foundation.
 *
 * Vanish is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Vanish.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef VANISHSTYLE_H
#define VANISHSTYLE_H

#include <QtWidgets/QPlastiqueStyle>

class VanishStylePrivate;

class Q_WIDGETS_EXPORT VanishStyle : public QPlastiqueStyle
{
    Q_OBJECT
public:
    VanishStyle();
    ~VanishStyle();

    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget = 0) const;
    void drawControl(ControlElement element, const QStyleOption *option,
                     QPainter *painter, const QWidget *widget) const;
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                            QPainter *painter, const QWidget *widget) const;
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                           const QSize &size, const QWidget *widget) const;

    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt,
                         SubControl sc, const QWidget *widget) const;

    int styleHint(StyleHint hint, const QStyleOption *option = 0, const QWidget *widget = 0,
                  QStyleHintReturn *returnData = 0) const;
    SubControl hitTestComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                                     const QPoint &pos, const QWidget *widget = 0) const;

    int pixelMetric(PixelMetric metric, const QStyleOption *option = 0, const QWidget *widget = 0) const;

    QPixmap standardPixmap(StandardPixmap standardPixmap, const QStyleOption *opt,
                           const QWidget *widget = 0) const;

    void polish(QApplication *app);
    void polish(QWidget *w);

    QPalette standardPalette() const;

protected Q_SLOTS:
    QIcon standardIconImplementation(StandardPixmap standardIcon, const QStyleOption *opt = 0,
                                     const QWidget *widget = 0) const;
    int layoutSpacingImplementation(QSizePolicy::ControlType control1,
                                    QSizePolicy::ControlType control2,
                                    Qt::Orientation orientation,
                                    const QStyleOption *option = 0,
                                    const QWidget *widget = 0) const;

private:
    Q_DISABLE_COPY(VanishStyle)

    void drawPartialFrame(QPainter *painter, const QStyleOptionComplex *option,
                          const QRect &rect, const QWidget *widget) const;

    QRect insideMargin(const QRect &r, int main, int left = 0, int top = 0,
                       int right = 0, int bottom = 0) const {
        // Adjust rectangle based on provided margins
        return r.adjusted(main + left, main + top, -main - right, -main - bottom);
    }
};

#endif // VANISHSTYLE_H
