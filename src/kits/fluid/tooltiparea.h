#ifndef QTOOLTIPAREA_H
#define QTOOLTIPAREA_H

#include <QTimer>
#include <QQuickItem>

class TooltipArea : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(bool containsMouse READ containsMouse NOTIFY containsMouseChanged)

public:
    explicit TooltipArea(QQuickItem *parent = 0);

    void setText(const QString &t);
    QString text() const {
        return m_text;
    }
    bool containsMouse() const {
        return m_containsMouse;
    }
    void showToolTip(const QString &str) const;
    void hoverEnterEvent(QHoverEvent *event);
    void hoverLeaveEvent(QHoverEvent *event);

public slots:
    void timeout();

signals:
    void textChanged();
    void containsMouseChanged();

private:

    QTimer m_tiptimer;
    QString m_text;
    bool m_containsMouse;
};

#endif // TOOLTIPAREA_H
