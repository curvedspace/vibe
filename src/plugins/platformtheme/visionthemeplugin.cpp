
#include "visionthemeplugin.h"
#include "visiontheme.h"

VisionThemePlugin::VisionThemePlugin(QObject *parent) :
    QPlatformThemePlugin(parent)
{
}

QStringList VisionThemePlugin::keys() const
{
    return QStringList() << "Vision";
}

QPlatformTheme *VisionThemePlugin::create(const QString &key, const QStringList &paramList)
{
	QStringList keys;
	keys << "vision" << "wayland" << "xcb";

    if (keys.contains(key.toLower()))
        return new VisionTheme();
    return 0;
}
