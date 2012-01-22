/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
 * Copyright (c) 2006-2011 the QxtLib project
 *
 * See the QXT_AUTHORS file for a list of authors and copyright holders
 * of the original QxtLib code.
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * Vibe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Vibe.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef VCOMMANDOPTIONS_H
#define VCOMMANDOPTIONS_H

#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtCore/QCoreApplication>
#include <QtCore/QMultiHash>
#include <QtCore/QFlags>

#include <VibeCore/VGlobal>

QT_FORWARD_DECLARE_CLASS(QTextStream)
QT_FORWARD_DECLARE_CLASS(QIODevice)

class VCommandOptionsPrivate;

/*!
    \addtogroup core Core Kit
    @{

    \class VCommandOptions <vcommandoptions.h> <VibeCore/VCommandOptions>

    \brief The VCommandOptions class is a parser for command-line options

    This class is used by applications that need to accept command-line arguments.  It can
    also automatically generate help text, which keeps it from accidentally falling out of
    sync with the supported options, and it can produce warnings for common errors.

    It accepts Windows-style ("/option"), UNIX-style ("-option"), and GNU-style
    ("--option") options. By default, VCommandOptions uses Windows-style options
    (VCommandOptions::Slash) on Windows and GNU-style options (VCommandOptions::DoubleDash)
    on UNIX and Mac. When using GNU-style options, single-character option names only
    require a single leading dash and can be grouped together, for example, "-abc".

    Any parameter that does not start with the option prefix is considered a positional
    parameter. Most applications treat positional parameters as filenames. When using
    GNU- or UNIX-style options, use a double dash ("--") alone to force the remaining
    parameters to be interpreted as positional parameters.

    To use VCommandOptions, first add the supported options using the add() and alias()
    methods, then process the command line using the parse() method, and finally retrieve
    the processed options using the positional(), count(), value() and/or parameters()
    methods.

    Mutually-exclusive options can be specified by using the \a group parameter to add().
    Only one option in a group will be accepted on the command line; if multiple options
    are provided, only the last one takes effect.

    Some options may accept an optional or required parameter. Depending on the value
    of the paramStyle() property, the parameter may be separated from the option by
    an equals sign ("/option=value") or by a space ("-option value"). By default,
    Windows uses an equals sign and UNIX and Mac accept both an equals sign and a
    space. Optional parameters always require an equals sign. Note that, when using
    GNU-style options, single-character options cannot have optional parameters.

    A simple archiving application might use code similar to this:
    \code
        VCommandOptions options;
        options.add("compress", "create a new archive");
        options.alias("compress", "c");
        options.add("extract", "extract files from an archive");
        options.alias("extract", "x");
        options.add("level", "set the compression level (0-9)", VCommandOptions::Required);
        options.alias("level", "l");
        options.add("verbose", "show more information about the process; specify twice for more detail", VCommandOptions::AllowMultiple);
        options.alias("verbose", "v");
        options.add("help", "show this help text");
        options.alias("help", "h");
        options.parse(QCoreApplication::arguments());
        if (options.count("help") || options.showUnrecognizedWarning()) {
            options.showUsage();
            return -1;
        }
        bool verbose = options.count("verbose");
        int level = 5;
        if (options.count("level"))
            level = options.value("level").toInt();
    \endcode

    \sa QCoreApplication::arguments()
*/
class VIBE_EXPORT VCommandOptions
{
    Q_DECLARE_TR_FUNCTIONS(VCommandOptions)
public:
    /*!
        \enum VCommandOptions::FlagStyle
        This enum type defines which type of option prefix is used.
         Slash is the default on Windows.
        DoubleDash is the default on all other platforms.
    */
    enum FlagStyle {
        DoubleDash,         /*!< Two dashes (GNU-style) */
        SingleDash,         /*!< One dash (UNIX-style) */
        Slash               /*!< Forward slash (Windows-style) */
    };

    /*!
        \enum VCommandOptions::ParamStyle
        This enum type defines what syntax is used for options that
        require parameters. Equals is the default on Windows.
        SpaceAndEquals is the default on all other platforms.
    */
    enum ParamStyle {
        Space = 1,          /*!< Space ("-option value") */
        Equals = 2,         /*!< Equals sign ("/option=value") */
        SpaceAndEquals = 3  /*!< Accept either */
    };

    /*!
        \enum VCommandOptions::ParamType
        \flags VCommandOptions::ParamTypes
        This enum type is used to specify flags that control the
        interpretation of an option.

        The ParamTypes type is a typedef for QFlags<ParamType>. It stores
        an OR combination of ParamType values.
    */
    enum ParamType {
        NoValue = 0,                /*!< The option does not accept a value. */
        ValueOptional = 1,          /*!< The option may accept a value. */
        ValueRequired = 2,          /*!< The option requires a value. */
        Optional = ValueOptional,   /*!< The option may accept a value. Deprecated in favor of ValueOptional. */
        Required = ValueRequired,   /*!< The option requires a value. Deprecated in favor of ValueRequired. */
        AllowMultiple = 4,          /*!< The option may be passed multiple times. */
        Undocumented = 8            /*!< The option is not output in the help text. */
    };
    Q_DECLARE_FLAGS(ParamTypes, ParamType)

    VCommandOptions();

    void setFlagStyle(FlagStyle style);
    FlagStyle flagStyle() const;
    void setParamStyle(ParamStyle style);
    ParamStyle paramStyle() const;
    void setScreenWidth(quint16 width);
    quint16 screenWidth() const;

    void addSection(const QString &name);
    void add(const QString &name, const QString &desc = QString(), ParamTypes paramType = NoValue, int group = -1);
    void alias(const QString &from, const QString &to);

    QStringList positional() const;
    QStringList unrecognized() const;
    int count(const QString &name) const;
    QVariant value(const QString &name) const;
    QMultiHash<QString, QVariant> parameters() const;

    void parse(int argc, char **argv);
    void parse(QStringList params);

    void showUsage(bool showQtOptions = false, QIODevice *device = 0) const;
    void showUsage(bool showQtOptions, QTextStream &stream) const;
    QString getUsage(bool showQtOptions = false) const;

    bool showUnrecognizedWarning(QIODevice *device = 0) const;
    bool showUnrecognizedWarning(QTextStream &stream) const;
    QString getUnrecognizedWarning() const;

private:
    Q_DECLARE_PRIVATE(VCommandOptions)

    VCommandOptionsPrivate *const d_ptr;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(VCommandOptions::ParamTypes)

/*!
    @}
*/

#endif // VCOMMANDOPTIONS_H
