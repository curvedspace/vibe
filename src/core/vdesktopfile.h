/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPL2$
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef VDESKTOPFILE_H
#define VDESKTOPFILE_H

#include <QObject>
#include <QVariant>

#include <VibeCore/VibeCoreExport>

class QStringList;
class QDir;
class QUrl;

class VDesktopFilePrivate;

/** \addtogroup core Core Kit
 *  @{
 */

/**
 * \class VDesktopFile vdesktopfile.h <VDesktopFile>
 *
 * @brief Desktop Entry Specification implementation.
 *
 * The Desktop Entry Specification describes desktop entries: files
 * describing information about an application such as the name, icon,
 * and description. These files are used for application launchers
 * and for creating menus of applications that can be launched.
 *
 * A desktop entry is a file with a name ending in the ".desktop" extension
 * which conforms to the Desktop Entry Specification with Type=Application.
 * It describes a menu item, including a name, an icon, and what to do
 * when the item is selected.
 *
 * Latest version of the specification can be read online at:
 *     http://standards.freedesktop.org/desktop-entry-spec/latest/
 *
 * This class implements version 1.0 of the standard.
 *
 * @author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 */
class VIBECORE_EXPORT VDesktopFile : public QObject
{
    Q_OBJECT
    Q_ENUMS(Type)
    Q_PROPERTY(QString fileName READ fileName)
    Q_PROPERTY(bool valid READ isValid)
    Q_PROPERTY(Type type READ type)
    Q_PROPERTY(QString typeName READ typeName)
    Q_PROPERTY(QString version READ version)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString genericName READ genericName)
    Q_PROPERTY(QString comment READ comment)
    Q_PROPERTY(QString iconName READ iconName)
    Q_PROPERTY(bool hidden READ isHidden)
    Q_PROPERTY(QStringList onlyShowIn READ onlyShowIn)
    Q_PROPERTY(QStringList notShowIn READ notShowIn)
    Q_PROPERTY(QString tryExecutePath READ tryExecutePath)
    Q_PROPERTY(bool executable READ isExecutable)
    Q_PROPERTY(QString executeCommand READ executeCommand)
    Q_PROPERTY(QDir workingDirectory READ workingDirectory)
    Q_PROPERTY(bool runInTerminal READ runInTerminal)
    Q_PROPERTY(QStringList supportedMimeTypes READ supportedMimeTypes)
    Q_PROPERTY(QStringList categories READ categories)
    Q_PROPERTY(QUrl url READ url)
public:
    /**
     * The Desktop Entry Specification defines three types of entries: Application,
     * Link and Directory.
     */
    enum Type {
        //! desktop entry for an application
        ApplicationType,
        //! desktop entry for a Web page
        LinkType,
        //! desktop entry for a directory
        DirectoryType,
        /**
         * none of the previous types. Call VDesktopFile::typeName() to get a
         * string representation of the type.
         *
         * \sa typeName()
         */
        UnknownType
    };

    /**
     * Constructs a VDesktopFile object.
     * \param fileName Desktop file name. It can be either its basename,
     *                 its name with the extension or the full path.
     *                 If a full path is not provided, the standard common
     *                 applications path is assumed.
     *
     * \sa QStandardPaths
     */
    explicit VDesktopFile(const QString &fileName);

    //! Destructs the VDesktopFile object.
    ~VDesktopFile();

    /**
     * Loads a desktop file.
     * \param fileName Desktop file name. It can be either its basename,
     *                 its name with the extension or the full path.
     *                 If a full path is not provided, the standard common
     *                 applications path is assumed.
     * \returns true if desktop file was loaded successfully,
     *          otherwise false.
     *
     * \sa QStandardPaths
     */
    bool load(const QString &fileName);

    //! Full path of the desktop file currently in use.
    QString fileName() const;

    /**
     * Returns whether desktop file is valid or not.
     * To be valid it has to respect the Desktop Entry Specification
     * standard.
     */
    bool isValid() const;

    /**
     * Returns the value of a desktop entry key.
     * \param key Desktop entry key.
     * \param defaultValue Optional default value.
     * \returns The corresponding value.
     */
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;

    //! Returns the type of desktop entry.
    Type type() const;

    //! Returns the type of desktop entry, in string format.
    QString typeName() const;

    //! Version of the specification that the desktop entry conforms with.
    QString version() const;

    //! Specific name of the entry (for example "Colombo").
    QString name() const;

    //! Generic name of the entry (for example "Web Browser").
    QString genericName() const;

    //! Tooltip for the entry.
    QString comment() const;

    //! Name of the entry's icon as defined by the Icon Theme Specification.
    QString iconName() const;

    /**
     * Returns whether the desktop entry:
     *   - Is hidden for all desktop environments
     *   - Deleted by the user at his level
     */
    bool isHidden() const;

    /**
     * Returns whether the desktop entry should be ignored for a specific
     * environment or not.
     * \param environment Case-sensitive environment name.
     */
    bool isHiddenFor(const QString &environment) const;

    //! Environments where the desktop entry should be considered only.
    QStringList onlyShowIn() const;

    //! Environments where the desktop entry should not be considered.
    QStringList notShowIn() const;

    /**
     * Path to an executable file on disk used to determine if
     * the application is actually installed.
     * Only relevant if type() is @ref ApplicationType.
     */
    QString tryExecutePath() const;

    /**
     * Returns whether the application is executable or not.
     * Only relevant if type() is @ref ApplicationType.
     */
    bool isExecutable() const;

    /**
     * Program to execute, possibly with arguments.
     * Only relevant if type() is @ref ApplicationType.
     */
    QString executeCommand() const;

    /**
     * The working directory to run the application in.
     * Only relevant if type() is @ref ApplicationType.
     */
    QDir workingDirectory() const;

    /**
     * Returns whether the application runs in a terminal window or not.
     * Only relevant if type() is @ref ApplicationType.
     */
    bool runInTerminal() const;

    /**
     * List of supported MIME types.
     * Only relevant if type() is @ref ApplicationType.
     */
    QStringList supportedMimeTypes() const;

    /**
     * Categories in which the entry should be shown in a menu.
     * Only relevant if type() is @ref ApplicationType.
     */
    QStringList categories() const;

    // TODO StartupNotify and StartupWMClass

    /**
     * URL.
     * Only relevant if type() is @ref LinkType.
     */
    QUrl url() const;

signals:
    void changed(const QString &);

private:
    Q_DECLARE_PRIVATE(VDesktopFile)
    Q_PRIVATE_SLOT(d_ptr, void _q_fileChanged(const QString &))

    VDesktopFilePrivate *const d_ptr;
};

/** @}*/

#endif // VDESKTOPFILE_H
