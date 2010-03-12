project(TerminalTermWidget)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/core ${CMAKE_SOURCE_DIR}/headers/QubeCore)
include_directories(${CMAKE_SOURCE_DIR}/src/gui ${CMAKE_SOURCE_DIR}/headers/QubeGui)
include_directories(${CMAKE_SOURCE_DIR}/headers)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_definitions(-DHAVE_POSIX_OPENPT)
#add_definitions(-DHAVE_GETPT)

set(TermWidgetSources
    TerminalCharacterDecoder.cpp
    KeyboardTranslator.cpp
    Screen.cpp
    History.cpp
    BlockArray.cpp
    konsole_wcwidth.cpp
    ScreenWindow.cpp
    Emulation.cpp
    Vt102Emulation.cpp
    TerminalDisplay.cpp
    Filter.cpp
    Pty.cpp
    kpty.cpp
    k3process.cpp
    k3processcontroller.cpp
    Session.cpp
    ShellCommand.cpp
    qtermwidget.cpp
)

set(TermWidgetHeaders
    TerminalCharacterDecoder.h
    Character.h
    CharacterColor.h
    KeyboardTranslator.h
    ExtendedDefaultTranslator.h
    Screen.h
    History.h
    BlockArray.h
    konsole_wcwidth.h
    ScreenWindow.h
    Emulation.h
    Vt102Emulation.h
    TerminalDisplay.h
    Filter.h
    LineFont.h
    Pty.h
    kpty.h
    kpty_p.h
    k3process.h
    k3processcontroller.h
    Session.h
    ShellCommand.h
    qtermwidget.h
)

qt4_automoc(${TermWidgetSources})

add_library(TermWidget STATIC ${TermWidgetSources})

target_link_libraries(TermWidget LINK_INTERFACE_LIBRARIES ${QT_QTGUI_LIBRARY})
