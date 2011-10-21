
#include "stringhandler.h"

namespace QubeCore
{
    namespace StringHandler
    {
        QString elide(const QString &str, Qt::TextElideMode mode, int maxlen)
        {
            switch (mode) {
            case Qt::ElideLeft:
                if (str.length() > maxlen) {
                    int part = maxlen - 3;
                    return QString::fromLatin1("...") + str.right(part);
                }
                break;
            case Qt::ElideRight:
                if (str.length() > maxlen) {
                    int part = maxlen - 3;
                    return str.left(part) + QLatin1String("...");
                }
                break;
            case Qt::ElideMiddle:
                if (str.length() > maxlen) {
                    const int part = (maxlen - 3) / 2;
                    return str.left(part) + QLatin1String("...") + str.right(part);
                }
                break;
            default:
                break;
            }

            return str;
        }
    }
}
