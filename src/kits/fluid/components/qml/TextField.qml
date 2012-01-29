/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2011 Daker Fernandes Pinheiro
 * Copyright (c) 2012 Pier Luigi Fiorini
 * Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies)
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *	Daker Fernandes Pinheiro <dakerfp@gmail.com>
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

/*!
Documented API

Inherits:
        Item

Imports:
        QtQuick 2.0

Description:
        Creates a simple plasma theme based text field widget.

Properties:
      * font font:
        This property holds the font used in the text field.
    The default font value is the font from plasma desktop theme.

      * Qt::InputMethodHints inputMethodHints:
        This property holds the the currently supported input method hints
     for the text field.
     The default values is Qt.ImhNone.

      * bool errorHighlight:
        This property holds if the text field is highlighted or not
    If it is true then the problematic lines will be highlighted.
    This feature is defined in the Common API but is unimplemented in plasma components.

      * int cursorPosition:
        This property holds the current cursor position.

      * bool readOnly:
        This property holds if the text field can be modified by the user interaction.
    The default value is false.

      * string selectedText:
        This property holds the text selected by the user.
    If no text is selected it holds an empty string.
    This property is read-only.

      * int selectionEnd:
        This property holds the cursor position after the last character in the current selection.
    This property is read-only.

      * int selectionStart:
        This property holds the cursor position before the first character in the current selection.
    This property is read-only.

      * string text:
        This property holds the entire text in the text field.

      * string placeholderText:
        This property holds the text displayed in when the text is empty.
    The default value is empty string, meaning no placeholderText shown.

      * enumeration echoMode:
        This property specifies how the text should be displayed in the TextField.
    The acceptable values are:
        - TextInput.Normal - Displays the text as it is. (Default)
        - TextInput.Password - Displays asterixes instead of characters.
        - TextInput.NoEcho - Displays nothing.
        - TextInput.PasswordEchoOnEdit - Displays all but the current character as asterixes.
    The default value is TextInput.Normal

      * string inputMask:
        Allows you to set an input mask on the TextField, restricting the allowable text inputs.
    See QLineEdit::inputMask for further details, as the exact same mask strings are used by TextInput.

      * Validator validator:
        Allows you to set a validator on the TextField. When a validator is set the TextField
    will only accept input which leaves the text property in an acceptable or intermediate state.
    The accepted signal will only be sent if the text is in an acceptable state when enter is pressed.
    Currently supported validators are IntValidator, DoubleValidator and RegExpValidator.
    An example of using validators is shown below, which allows input of integers
    between 11 and 31 into the text input:
    <code>
    import QtQuick 2.0
    TextInput {
        validator: IntValidator{bottom: 11; top: 31;}
        focus: true
    }
    </code>

      * int maximumLength:
        The maximum permitted length of the text in the TextField.
    If the text is too long, it is truncated at the limit.
    By default, this property contains a value of 32767.

       * bool acceptableInput:
         This property is always true unless a validator or input mask has been set.
    If a validator or input mask has been set, this property will only be true if the current
    text is acceptable to the validator or input mask as a final string (not as an intermediate string).
    This property is always true unless a validator has been set.
    If a validator has been set, this property will only be true if the current text is acceptable to the
    validator as a final string (not as an intermediate string).
    This property is read-only.

       * bool clearButtonShown:
         Holds if the button to clear the text from TextField is visible.

Methods:
       * void copy():
         Copies the currently selected text to the system clipboard.

       * void cut():
         Moves the currently selected text to the system clipboard.

       * void deselect():
         Removes active text selection.

       * void paste():
         Replaces the currently selected text by the contents of the system clipboard.

       * void select(int start, int end):
         Causes the text from start to end to be selected.
     If either start or end is out of range, the selection is not changed.
     After calling this, selectionStart will become the lesser and selectionEnd will become the greater
     (regardless of the order passed to this method).

       * void selectAll():
         Causes all text to be selected.

       * void selectWord():
         Causes the word closest to the current cursor position to be selected.

       * void positionAt(int position):
         This function returns the character position at x pixels from the left of the TextField.
     Position 0 is before the first character, position 1 is after the first character but before the second,
     and so on until position text.length, which is after all characters.
     This means that for all x values before the first character this function returns 0,
     and for all x values after the last character this function returns text.length.

       * rectangle positionToRectangle(position):
         Returns the rectangle at the given position in the text.
     The x, y, and height properties correspond to the cursor that would describe that position.
*/

import QtQuick 2.0

FocusScope {
    id: focusScope
    width: 250; height: 28

    // Common API
    property string placeholderText

    SystemPalette {
        id: palette
    }

    BorderImage {
        source: "image://theme/lineedit-bg"
        width: parent.width; height: parent.height
        border { left: 4; top: 4; right: 4; bottom: 4 }
    }

    BorderImage {
        source: "image://theme/lineedit-bg-focus"
        width: parent.width; height: parent.height
        border { left: 4; top: 4; right: 4; bottom: 4 }
        visible: parent.activeFocus ? true : false
    }

    Text {
        id: typeSomething
        anchors.fill: parent; anchors.leftMargin: 8
        verticalAlignment: Text.AlignVCenter
        text: placeholderText
        color: palette.windowText
        opacity: 0.5
        elide: Text.ElideRight
        clip: true
/*
        font.capitalization: theme.defaultFont.capitalization
        font.family: theme.defaultFont.family
        font.italic: true
        font.letterSpacing: theme.defaultFont.letterSpacing
        font.pointSize: theme.defaultFont.pointSize
        font.strikeout: theme.defaultFont.strikeout
        font.underline: theme.defaultFont.underline
        font.weight: theme.defaultFont.weight
        font.wordSpacing: theme.defaultFont.wordSpacing
*/
        font.italic: true
    }

    MouseArea { 
        anchors.fill: parent
        onClicked: { focusScope.focus = true; textInput.openSoftwareInputPanel(); } 
    }

    TextInput {
        id: textInput
        anchors { left: parent.left; leftMargin: 8; right: clear.left; rightMargin: 8; verticalCenter: parent.verticalCenter }
        focus: true
        selectByMouse: true
    }

    Image {
        id: clear
        anchors { right: parent.right; rightMargin: 8; verticalCenter: parent.verticalCenter }
        source: "image://icon/edit-clear-locationbar-" + (Qt.application.layoutDirection == Qt.LeftToRight ? "ltr" : "rtl")
        opacity: 0

        MouseArea { 
            anchors.fill: parent
            onClicked: { textInput.text = ''; focusScope.focus = true; textInput.openSoftwareInputPanel(); }
        }
    }

    states: State {
        name: "hasText"; when: textInput.text != ''
        PropertyChanges { target: typeSomething; opacity: 0 }
        PropertyChanges { target: clear; opacity: 1 }
    }

    transitions: [
        Transition {
            from: ""; to: "hasText"
            NumberAnimation { exclude: typeSomething; properties: "opacity" }
        },
        Transition {
            from: "hasText"; to: ""
            NumberAnimation { properties: "opacity" }
        }
    ]
}
