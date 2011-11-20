/**********************************************************************
**
** This file is part of on Toolkit.
**
** Copyright (C) 2009-2020 Dzimi Mve Alexandre <dzimiwine@gmail.com>
**
** Contact: dzimiwine@gmail.com
**
** on is a free toolkit developed in Qt by Dzimi Mve A.; you can redistribute
** sources and libraries of this library and/or modify them under the terms of
** the GNU Library General Public License version 3.0 as published by the
** Free Software Foundation and appearing in the file LICENSE.txt included in
** the packaging of this file.
** Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** This SDK is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
**
**********************************************************************/

#ifndef DIR_HELPER_H
#define DIR_HELPER_H

class QString;
class QStringList;

template< class U, class V >
class QMap;

//
//  W A R N I N G
//  -------------
//
// This file is not part of the on API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

namespace Qube
{
namespace Gui
{
class DirHelper
{
public:
    enum SpecialFolder {
        Home,
        Desktop,
        Documents,
        Music,
        Pictures,
        Video
    };
    static QStringList splitPath( const QString & );
    static QString setupPath(const QStringList &, int index);
    static QString specialFolder( DirHelper::SpecialFolder );
    static QString driveLabel( const QString & drive );

    static void init();

    static QMap< SpecialFolder, QString > m_specialFolderMap;
    static bool m_initialized;

    static QString myComputer();
};
}
}

#endif // BREADCRUMBDIRMODEL_HELPER_H
