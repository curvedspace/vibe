
#include <QGraphicsView>
#include <QToolTip>
#include <QApplication>
#include <QGraphicsSceneEvent>

#include "tooltiparea.h"

TooltipArea::TooltipArea(QQuickItem *parent) :
    QQuickItem(parent),
    m_containsMouse(false)
{
    setAcceptHoverEvents(true);
    connect(&m_tiptimer, SIGNAL(timeout()), this, SLOT(timeout()));
    m_tiptimer.setSingleShot(true);
}

void TooltipArea::setText(const QString &t)
{
    if (t != m_text) {
        m_text = t;
        emit textChanged();
    }
}

void TooltipArea::showToolTip(const QString &str) const
{
    QToolTip::showText(QPoint(pos().x(), pos().y()), str);
}

void TooltipArea::hoverEnterEvent(QHoverEvent *event)
{
    m_tiptimer.start(1000);

    m_containsMouse = true;
    emit containsMouseChanged();
    QQuickItem::hoverEnterEvent(event);
}

void TooltipArea::hoverLeaveEvent(QHoverEvent *event)
{
    m_tiptimer.stop();
    m_containsMouse = false;
    emit containsMouseChanged();
    QQuickItem::hoverLeaveEvent(event);
}

void TooltipArea::timeout()
{
    showToolTip(m_text);
}

#include "moc_tooltiparea.cpp"
