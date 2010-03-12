#ifndef QUBEWIDGETS_H
#define QUBEWIDGETS_H

#include <QtDesigner/QDesignerCustomWidgetCollectionInterface>

class QubeWidgets : public QObject, QDesignerCustomWidgetCollectionInterface
{
Q_OBJECT
Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)
public:
    explicit QubeWidgets(QObject *parent = 0);

    virtual QList<QDesignerCustomWidgetInterface *> customWidgets() const;

private:
    QList<QDesignerCustomWidgetInterface *> m_plugins;
};

#endif // QUBEWIDGETS_H
