#ifndef QNAVBARPLUGINTASKMENUFACTORY_H
#define QNAVBARPLUGINTASKMENUFACTORY_H

#include <QtDesigner/QExtensionFactory>

QT_BEGIN_NAMESPACE
class QExtensionManager;
QT_END_NAMESPACE

class QNavBar;

class QNavBarPluginTaskMenuFactory : public QExtensionFactory
{
Q_OBJECT
public:
    explicit QNavBarPluginTaskMenuFactory(QExtensionManager *parent = 0);

protected:
    QObject *createExtension(QObject *object, const QString &iid, QObject *parent) const;
};

#endif // QNAVBARPLUGINTASKMENUFACTORY_H
