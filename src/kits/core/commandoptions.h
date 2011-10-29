/****************************************************************************
 * This file is part of Qube.
 *
 * Copyright (c) 2006-2011 the QxtLib project
 * See the QXT_AUTHORS file for a list of authors and copyright holders
 * of the original QxtLib code.
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Qube is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Qube is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Qube.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef QUBW_COMMANDOPTIONS_H
#define QUBE_COMMANDOPTIONS_H

#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtCore/QCoreApplication>     // for Q_DECLARE_TR_FUNCTIONS
#include <QtCore/QMultiHash>
#include <QtCore/QFlags>

QT_FORWARD_DECLARE_CLASS(QTextStream)
QT_FORWARD_DECLARE_CLASS(QIODevice)

namespace Qube
{
    namespace Core
    {
        class CommandOptionsPrivate;

        class CommandOptions
        {
            Q_DECLARE_PRIVATE(CommandOptions)
            Q_DECLARE_TR_FUNCTIONS(CommandOptions)
        public:
            /*!
             * \enum CommandOptions::FlagStyle
             * This enum type defines which type of option prefix is used.
             * Slash is the default on Windows.
             * DoubleDash is the default on all other platforms.
             */
            enum FlagStyle {
                DoubleDash,         /*!< Two dashes (GNU-style) */
                SingleDash,         /*!< One dash (UNIX-style) */
                Slash               /*!< Forward slash (Windows-style) */
            };

            /*!
             * \enum CommandOptions::ParamStyle
             * This enum type defines what syntax is used for options that
             * require parameters. Equals is the default on Windows.
             * SpaceAndEquals is the default on all other platforms.
             */
            enum ParamStyle {
                Space = 1,          /*!< Space ("-option value") */
                Equals = 2,         /*!< Equals sign ("/option=value") */
                SpaceAndEquals = 3  /*!< Accept either */
            };

            /*!
             * \enum CommandOptions::ParamType
             * \flags CommandOptions::ParamTypes
             * This enum type is used to specify flags that control the
             * interpretation of an option.
             *
             * The ParamTypes type is a typedef for QFlags<ParamType>. It stores
             * an OR combination of ParamType values.
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

            CommandOptions();

            void setFlagStyle(FlagStyle style);
            FlagStyle flagStyle() const;
            void setParamStyle(ParamStyle style);
            ParamStyle paramStyle() const;
            void setScreenWidth(quint16 width);
            quint16 screenWidth() const;

            void addSection(const QString& name);
            void add(const QString& name, const QString& desc = QString(), ParamTypes paramType = NoValue, int group = -1);
            void alias(const QString& from, const QString& to);

            QStringList positional() const;
            QStringList unrecognized() const;
            int count(const QString& name) const;
            QVariant value(const QString& name) const;
            QMultiHash<QString, QVariant> parameters() const;

            void parse(int argc, char** argv);
            void parse(QStringList params);

            void showUsage(bool showQtOptions = false, QIODevice* device = 0) const;
            void showUsage(bool showQtOptions, QTextStream& stream) const;
            QString getUsage(bool showQtOptions = false) const;

            bool showUnrecognizedWarning(QIODevice* device = 0) const;
            bool showUnrecognizedWarning(QTextStream& stream) const;
            QString getUnrecognizedWarning() const;

        private:
            CommandOptionsPrivate * const d_ptr;
        };

        Q_DECLARE_OPERATORS_FOR_FLAGS(CommandOptions::ParamTypes)
    }
}

#endif // QUBE_COMMANDOPTIONS_H
