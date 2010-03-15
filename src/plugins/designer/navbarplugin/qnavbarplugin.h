#ifndef QNAVBARPLUGIN_H
#define QNAVBARPLUGIN_H

#include <QtDesigner/QDesignerCustomWidgetInterface>

class QNavBarPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    explicit QNavBarPlugin(const QIcon &icon, QObject *parent = 0);

    void initialize(QDesignerFormEditorInterface *formEditor);
    bool isInitialized() const;
    bool isContainer() const;

    QWidget *createWidget(QWidget *parent);

    QIcon icon() const;
    QString domXml() const;
    QString group() const;
    QString includeFile() const;
    QString name() const;
    QString toolTip() const;
    QString whatsThis() const;

private:
    bool m_initialized;
    QIcon m_icon;
};

#endif // QNAVBARPLUGIN_H
