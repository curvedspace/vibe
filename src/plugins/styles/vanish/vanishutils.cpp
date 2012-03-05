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

#include <QPixmapCache>

#include "vanishutils.h"
#include "qstylehelper_p.h"

Q_GUI_EXPORT int qt_defaultDpiX();

qreal dpiScaled(qreal value)
{
    static const qreal scale = qreal(qt_defaultDpiX()) / 96.0;
    return value * scale;
}

QLinearGradient qMapGradientToRect(const QLinearGradient &gradient, const QRectF &rect)
{
    QLinearGradient tmpGrad(rect.center().x(), rect.top(),
                            rect.center().x(), rect.bottom());
    tmpGrad.setStops(gradient.stops());
    return tmpGrad;
}

QBrush qMapBrushToRect(const QBrush &brush, const QRectF &rect)
{
    if (!brush.gradient())
        return brush;

    // ### Ugly assumption that it's a linear gradient
    QBrush tmp(qMapGradientToRect(*static_cast<const QLinearGradient *>(brush.gradient()), rect));
    return tmp;
}

void qBrushSetAlphaF(QBrush *brush, qreal alpha)
{
    if (const QGradient *gradient = brush->gradient()) {
        // Use the gradient. Call QColor::setAlphaF() on all color stops.
        QGradientStops stops = gradient->stops();
        QMutableVectorIterator<QGradientStop> it(stops);
        QColor tmpColor;
        while (it.hasNext()) {
            it.next();
            tmpColor = it.value().second;
            tmpColor.setAlphaF(alpha * tmpColor.alphaF());
            it.setValue(QPair<qreal, QColor>(it.value().first, tmpColor));
        }

        switch (gradient->type()) {
            case QGradient::RadialGradient: {
                QRadialGradient grad = *static_cast<const QRadialGradient *>(gradient);
                grad.setStops(stops);
                *brush = QBrush(grad);
                break;
            }
            case QGradient::ConicalGradient: {
                QConicalGradient grad = *static_cast<const QConicalGradient *>(gradient);
                grad.setStops(stops);
                *brush = QBrush(grad);
                break;
            }
            default:
                qWarning("VanishStyle::qBrushLight() - unknown gradient type"
                         " - falling back to QLinearGradient");
            case QGradient::LinearGradient: {
                QLinearGradient grad = *static_cast<const QLinearGradient *>(gradient);
                grad.setStops(stops);
                *brush = QBrush(grad);
                break;
            }
        }
    } else if (!brush->texture().isNull()) {
        // Modify the texture - ridiculously expensive.
        QPixmap texture = brush->texture();
        QPixmap pixmap;
        QString name = QLatin1Literal("qbrushtexture-alpha")
                       % HexString<qreal>(alpha)
                       % HexString<qint64>(texture.cacheKey());
        if (!QPixmapCache::find(name, pixmap)) {
            QImage image = texture.toImage();
            QRgb *rgb = reinterpret_cast<QRgb *>(image.bits());
            int pixels = image.width() * image.height();
            QColor tmpColor;
            while (pixels--) {
                tmpColor.setRgb(*rgb);
                tmpColor.setAlphaF(alpha * tmpColor.alphaF());
                *rgb++ = tmpColor.rgba();
            }
            pixmap = QPixmap::fromImage(image);
            QPixmapCache::insert(name, pixmap);
        }
        brush->setTexture(pixmap);
    } else {
        // Use the color
        QColor tmpColor = brush->color();
        tmpColor.setAlphaF(alpha * tmpColor.alphaF());
        brush->setColor(tmpColor);
    }
}

QBrush qBrushLight(QBrush brush, int light)
{
    if (const QGradient *gradient = brush.gradient()) {
        // Use the gradient. Call QColor::lighter() on all color stops.
        QGradientStops stops = gradient->stops();
        QMutableVectorIterator<QGradientStop> it(stops);
        while (it.hasNext()) {
            it.next();
            it.setValue(QPair<qreal, QColor>(it.value().first, it.value().second.lighter(light)));
        }

        switch (gradient->type()) {
            case QGradient::RadialGradient: {
                QRadialGradient grad = *static_cast<const QRadialGradient *>(gradient);
                grad.setStops(stops);
                brush = QBrush(grad);
                break;
            }
            case QGradient::ConicalGradient: {
                QConicalGradient grad = *static_cast<const QConicalGradient *>(gradient);
                grad.setStops(stops);
                brush = QBrush(grad);
                break;
            }
            default:
                qWarning("VanishStyle::qBrushLight() - unknown gradient type"
                         " - falling back to QLinearGradient");
            case QGradient::LinearGradient: {
                QLinearGradient grad = *static_cast<const QLinearGradient *>(gradient);
                grad.setStops(stops);
                brush = QBrush(grad);
                break;
            }
        }
    } else if (!brush.texture().isNull()) {
        // Modify the texture - ridiculously expensive.
        QPixmap texture = brush.texture();
        QPixmap pixmap;
        QString name = QLatin1Literal("qbrushtexture-light")
                       % HexString<int>(light)
                       % HexString<qint64>(texture.cacheKey());

        if (!QPixmapCache::find(name, pixmap)) {
            QImage image = texture.toImage();
            QRgb *rgb = reinterpret_cast<QRgb *>(image.bits());
            int pixels = image.width() * image.height();
            QColor tmpColor;
            while (pixels--) {
                tmpColor.setRgb(*rgb);
                *rgb++ = tmpColor.lighter(light).rgba();
            }
            pixmap = QPixmap::fromImage(image);
            QPixmapCache::insert(name, pixmap);
        }
        brush.setTexture(pixmap);
    } else {
        // Use the color
        brush.setColor(brush.color().lighter(light));
    }
    return brush;
}

QBrush qBrushDark(QBrush brush, int dark)
{
    if (const QGradient *gradient = brush.gradient()) {
        // Use the gradient. Call QColor::darker() on all color stops.
        QGradientStops stops = gradient->stops();
        QMutableVectorIterator<QGradientStop> it(stops);
        while (it.hasNext()) {
            it.next();
            it.setValue(QPair<qreal, QColor>(it.value().first, it.value().second.darker(dark)));
        }

        switch (gradient->type()) {
            case QGradient::RadialGradient: {
                QRadialGradient grad = *static_cast<const QRadialGradient *>(gradient);
                grad.setStops(stops);
                brush = QBrush(grad);
                break;
            }
            case QGradient::ConicalGradient: {
                QConicalGradient grad = *static_cast<const QConicalGradient *>(gradient);
                grad.setStops(stops);
                brush = QBrush(grad);
                break;
            }
            default:
                qWarning("VanishStyle::qBrushDark() - unknown gradient type"
                         " - falling back to QLinearGradient");
            case QGradient::LinearGradient: {
                QLinearGradient grad = *static_cast<const QLinearGradient *>(gradient);
                grad.setStops(stops);
                brush = QBrush(grad);
                break;
            }
        }
    } else if (!brush.texture().isNull()) {
        // Modify the texture - ridiculously expensive.
        QPixmap texture = brush.texture();
        QPixmap pixmap;
        QString name = QLatin1Literal("qbrushtexture-dark")
                       % HexString<int>(dark)
                       % HexString<qint64>(texture.cacheKey());

        if (!QPixmapCache::find(name, pixmap)) {
            QImage image = texture.toImage();
            QRgb *rgb = reinterpret_cast<QRgb *>(image.bits());
            int pixels = image.width() * image.height();
            QColor tmpColor;
            while (pixels--) {
                tmpColor.setRgb(*rgb);
                *rgb++ = tmpColor.darker(dark).rgba();
            }
            pixmap = QPixmap::fromImage(image);
            QPixmapCache::insert(name, pixmap);
        }
        brush.setTexture(pixmap);
    } else {
        // Use the color
        brush.setColor(brush.color().darker(dark));
    }
    return brush;
}
