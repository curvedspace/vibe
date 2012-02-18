
#ifndef VISIONTHEMEPLUGIN_H
#define VISIONTHEMEPLUGIN_H

#include <QtGui/qplatformthemeplugin_qpa.h>

class Q_GUI_EXPORT VisionThemePlugin : public QPlatformThemePlugin
{
    Q_OBJECT
public:
    explicit VisionThemePlugin(QObject *parent = 0);

    QStringList keys() const;
    QPlatformTheme *create(const QString &key, const QStringList &paramList);
};

#endif // VISIONTHEMEPLUGIN_H
