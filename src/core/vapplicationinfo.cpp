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

#include <QDir>
#include <QUrl>
#include <QVariant>
#include <QProcess>

#include "vapplicationinfo.h"
#include "vapplicationinfo_p.h"
#include "vdesktopfile.h"

/*
 * VApplicationInfoPrivate
 */

VApplicationInfoPrivate::VApplicationInfoPrivate(VApplicationInfo *q, const QString &fileName) :
    q_ptr(q)
{
    desktopFile = new VDesktopFile(fileName);
    isValid = desktopFile->isValid();

    // Cache information
    name = desktopFile->name();
    genericName = desktopFile->genericName();
    comment = desktopFile->comment();
    iconName = desktopFile->iconName();
}

VApplicationInfoPrivate::~VApplicationInfoPrivate()
{
    delete desktopFile;
}

QStringList VApplicationInfoPrivate::extractExecArgs(const QStringList &args)
{
    QStringList result;
    // TODO: Need to handle quoting see http://standards.freedesktop.org/desktop-entry-spec/latest/ar01s06.html
    QStringList tokens = desktopFile->executeCommand().split(QLatin1Char(' '));

    result << tokens.at(0);

    foreach(QString token, tokens) {
        /*
         * The specification says:
         *
         * A single file name, even if multiple files are selected. The system
         * reading the desktop entry should recognize that the program in
         * question cannot handle multiple file arguments, and it should
         * should probably spawn and execute multiple copies of a program
         * for each selected file if the program is not able to handle
         * additional file arguments. If files are not on the local file
         * system (i.e. are on HTTP or FTP locations), the files will be
         * copied to the local file system and %f will be expanded to point
         * at the temporary file. Used for programs that do not understand the URL syntax.
         */
        if (token == QLatin1String("%f")) {
            if (!args.isEmpty())
                result << expandEnvVariables(args.at(0));
            continue;
        }

        /*
         * The specification says:
         *
         * A list of files. Use for apps that can open several local files at once. Each file is passed as a separate argument to the executable program.
         */
        if (token == QLatin1String("%F")) {
            result << expandEnvVariables(args.join(QLatin1Char(' ')));
            continue;
        }

        /*
         * The specification says:
         *
         * A single URL. Local files may either be passed as file: URLs or as file path.
         */
        if (token == QLatin1String("%u")) {
            if (!args.isEmpty()) {
                QUrl url(expandEnvVariables(args.at(0)));
                result << (!url.toLocalFile().isEmpty() ? url.toLocalFile() : url.toEncoded());
            }
            continue;
        }

        /*
         * The specification says:
         *
         * A list of URLs. Each URL is passed as a separate argument to the
         * executable program. Local files may either be passed as file: URLs or as file path.
         */
        if (token == QLatin1String("%U")) {
            foreach(QString u, args) {
                QUrl url(expandEnvVariables(u));
                result << (!url.toLocalFile().isEmpty() ? url.toLocalFile() : url.toEncoded());
            }
            continue;
        }

        /*
         * The specification says:
         *
         * The Icon key of the desktop entry expanded as two arguments,
         * first --icon and then the value of the Icon key. Should not expand
         * to any arguments if the Icon key is empty or missing.
         */
        if (token == QLatin1String("%i")) {
            QString icon = desktopFile->iconName();
            if (!icon.isEmpty())
                result << QLatin1String("-icon") << icon.replace(QLatin1Char('%'), QLatin1String("%%"));
            continue;
        }

        /*
         * The specification says:
         *
         * The translated name of the application as listed in the appropriate
         * Name key in the desktop entry.
         */
        if (token == QLatin1String("%c")) {
            result << desktopFile->name().replace(QLatin1Char('%'), QLatin1String("%%"));
            continue;
        }

        /*
         * The specification says:
         *
         * The location of the desktop file as either a URI (if for example
         * gotten from the vfolder system) or a local filename or empty if no
         * location is known.
         */
        if (token == QLatin1String("%k"))
            result << desktopFile->fileName().replace(QLatin1Char('%'), QLatin1String("%%"));
    }

    return result;
}

bool VApplicationInfoPrivate::launch(const QStringList &_args)
{
    QStringList args = extractExecArgs(_args);
    if (args.isEmpty())
        return false;

    // Run in a terminal if requested
    if (desktopFile->runInTerminal()) {
        // TODO: We need to handle application association here and run the default terminal
        QString term = QString::fromLocal8Bit(qgetenv("TERM"));
        if (term.isEmpty())
            term = "terminal";

        args.prepend("-e");
        args.prepend(term);
    }

    QString cmd = args.takeFirst();
    QString wd = desktopFile->workingDirectory().absolutePath();
    return QProcess::startDetached(cmd, args, wd);
}

void VApplicationInfoPrivate::_q_desktopFileChanged()
{
    // Desktop file has changed, let's what fields were exactly changed
    if (isValid != desktopFile->isValid()) {
        isValid = desktopFile->isValid();
        emit q_ptr->validChanged(isValid);
    }
    if (isValid) {
        if (identifier != desktopFile->value("X-Hawaii-Identifier").toString()) {
            identifier = desktopFile->value("X-Hawaii-Identifier").toString();
            emit q_ptr->identifierChanged(identifier);
        }
        if (version != desktopFile->value("X-Hawaii-Version").toString()) {
            version = desktopFile->value("X-Hawaii-Version").toString();
            emit q_ptr->versionChanged(version);
        }
        if (name != desktopFile->name()) {
            name = desktopFile->name();
            emit q_ptr->nameChanged(name);
        }
        if (genericName != desktopFile->genericName()) {
            genericName = desktopFile->genericName();
            emit q_ptr->genericNameChanged(name);
        }
        if (comment != desktopFile->comment()) {
            comment = desktopFile->comment();
            emit q_ptr->commentChanged(comment);
        }
        if (iconName != desktopFile->iconName()) {
            iconName = desktopFile->iconName();
            emit q_ptr->iconNameChanged(iconName);
        }
    }
}

void VApplicationInfoPrivate::expandEnvVariable(QString &str, const QString &varName, const QString &after)
{
    // Replace variables with the nation $NAME or ${NAME}
    str.replace(QRegExp(QString("\\$%1(?!\\w)").arg(varName)), after);
    str.replace(QRegExp(QString("\\$\\{%1\\}").arg(varName)), after);
}

QString VApplicationInfoPrivate::expandEnvVariables(const QString &str)
{
    // Any non-local URI won't be handled
    if (QUrl(str).scheme() != QLatin1String("file"))
        return str;

    QString res = str;

    // TODO: There's might be more environment variables
    expandEnvVariable(res, "HOME", QString::fromLocal8Bit(qgetenv("HOME")));
    expandEnvVariable(res, "USER", QString::fromLocal8Bit(qgetenv("USER")));

    return res;
}

/*!
    \class VApplicationInfo
    \brief The VApplicationInfo class reads application information from the
    the specified file.

    \ingroup core

    \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
*/

/*!
    Constructs an application information object from \a fileName.
*/
VApplicationInfo::VApplicationInfo(const QString &fileName) :
    d_ptr(new VApplicationInfoPrivate(this, fileName))
{
    connect(d_ptr->desktopFile, SIGNAL(changed(QString)),
            this, SLOT(_q_desktopFileChanged()));
}

/*!
    Destroys the object.
*/
VApplicationInfo::~VApplicationInfo()
{
    delete d_ptr;
}

/*!
    Returns the desktop file name with absolute path.
*/
QString VApplicationInfo::fileName() const
{
    Q_D(const VApplicationInfo);
    return d->desktopFile->fileName();
}

/*!
    Returns whether the information is valid or not.
*/
bool VApplicationInfo::isValid() const
{
    Q_D(const VApplicationInfo);
    return d->desktopFile->isValid() &&
           d->desktopFile->type() == VDesktopFile::ApplicationType;
}

/*!
    Returns application's identifier in reverse domain form, for example for an
    imaginary "App" application from Maui it whould be "org.maui.App".
*/
QString VApplicationInfo::identifier() const
{
    Q_D(const VApplicationInfo);
    return d->desktopFile->value("X-Hawaii-Identifier").toString();
}

/*!
    Returns application's version.
    \sa QApplication::applicationVersion()
*/
QString VApplicationInfo::version() const
{
    Q_D(const VApplicationInfo);
    return d->desktopFile->value("X-Hawaii-Version").toString();
}

/*!
    Returns the application's name.
*/
QString VApplicationInfo::name() const
{
    Q_D(const VApplicationInfo);
    return d->desktopFile->name();
}

/*!
    Returns a generic name that describes what the application does.
*/
QString VApplicationInfo::genericName() const
{
    Q_D(const VApplicationInfo);
    return d->desktopFile->genericName();
}

/*!
    Returns a verbose description of what the application does.
*/
QString VApplicationInfo::comment() const
{
    Q_D(const VApplicationInfo);
    return d->desktopFile->comment();
}

/*!
    Returns the icon name for the application.
    The result of this method can be passed to QIcon::fromTheme() in order to
    retrieve the icon from the theme.

    \sa QIcon::fromTheme()
*/
QString VApplicationInfo::iconName() const
{
    Q_D(const VApplicationInfo);
    return d->desktopFile->iconName();
}

/*!
    Returns whether this application should be hidden from application
    launchers.
*/
bool VApplicationInfo::isHidden() const
{
    Q_D(const VApplicationInfo);
    return d->desktopFile->isHidden() || d->desktopFile->isHiddenFor("Hawaii");
}

QStringList VApplicationInfo::onlyShowIn() const
{
    Q_D(const VApplicationInfo);
    return d->desktopFile->onlyShowIn();
}

QStringList VApplicationInfo::notShowIn() const
{
    Q_D(const VApplicationInfo);
    return d->desktopFile->notShowIn();
}

QString VApplicationInfo::tryExecutePath() const
{
    Q_D(const VApplicationInfo);
    return d->desktopFile->tryExecutePath();
}

bool VApplicationInfo::isExecutable() const
{
    Q_D(const VApplicationInfo);
    return d->desktopFile->isExecutable();
}

QString VApplicationInfo::executeCommand() const
{
    Q_D(const VApplicationInfo);
    return d->desktopFile->executeCommand();
}

/*!
    Returns the working directory to be set when launching this application.
*/
QDir VApplicationInfo::workingDirectory() const
{
    Q_D(const VApplicationInfo);
    return d->desktopFile->workingDirectory();
}

/*!
    Returns whether the application should bu run in a terminal window.
*/
bool VApplicationInfo::runInTerminal() const
{
    Q_D(const VApplicationInfo);
    return d->desktopFile->runInTerminal();
}

/*!
    Returns a list of MIME types supported by this application.
*/
QStringList VApplicationInfo::supportedMimeTypes() const
{
    Q_D(const VApplicationInfo);
    return d->desktopFile->supportedMimeTypes();
}

/*!
    Returns the categories to which the application belongs.
*/
QStringList VApplicationInfo::categories() const
{
    Q_D(const VApplicationInfo);
    return d->desktopFile->categories();
}

/*!
    Launch the application with \a args.
    Returns whether the application was launched successfully or not.
*/
bool VApplicationInfo::launch(const QStringList &args)
{
    Q_D(VApplicationInfo);
    return d->launch(args);
}

#include "moc_vapplicationinfo.cpp"
