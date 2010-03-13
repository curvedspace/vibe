project(Terminal)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/core ${CMAKE_SOURCE_DIR}/headers/QubeCore)
include_directories(${CMAKE_SOURCE_DIR}/src/gui ${CMAKE_SOURCE_DIR}/headers/QubeGui)
include_directories(${CMAKE_SOURCE_DIR}/headers)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_definitions(-DHAVE_POSIX_OPENPT)
#add_definitions(-DHAVE_GETPT)

set(TermWidgetSources
    lib/TerminalCharacterDecoder.cpp
    lib/KeyboardTranslator.cpp
    lib/Screen.cpp
    lib/History.cpp
    lib/BlockArray.cpp
    lib/konsole_wcwidth.cpp
    lib/ScreenWindow.cpp
    lib/Emulation.cpp
    lib/Vt102Emulation.cpp
    lib/TerminalDisplay.cpp
    lib/Filter.cpp
    lib/Pty.cpp
    lib/kpty.cpp
    lib/k3process.cpp
    lib/k3processcontroller.cpp
    lib/Session.cpp
    lib/ShellCommand.cpp
    lib/qtermwidget.cpp
)

set(TermWidgetHeaders
    lib/TerminalCharacterDecoder.h
    lib/Character.h
    lib/CharacterColor.h
    lib/KeyboardTranslator.h
    lib/ExtendedDefaultTranslator.h
    lib/Screen.h
    lib/History.h
    lib/BlockArray.h
    lib/konsole_wcwidth.h
    lib/ScreenWindow.h
    lib/Emulation.h
    lib/Vt102Emulation.h
    lib/TerminalDisplay.h
    lib/Filter.h
    lib/LineFont.h
    lib/Pty.h
    lib/kpty.h
    lib/kpty_p.h
    lib/k3process.h
    lib/k3processcontroller.h
    lib/Session.h
    lib/ShellCommand.h
    lib/qtermwidget.h
)

set(TerminalSources
    mainwindow.cpp
    main.cpp
)

set(TerminalHeaders
    mainwindow.h
)

set(TerminalUis
    mainwindow.ui
)

set(TerminalResources
    terminal.qrc
)

qt4_automoc(${TermWidgetSources} ${TerminalSources})
qt4_wrap_ui(TerminalUiFiles ${TerminalUis})
qt4_add_resources(TerminalResourceFiles ${TrackerResources})

add_executable(terminal ${TermWidgetSources} ${TerminalSources} ${TerminalUiFiles} ${TrackerResourceFiles})

target_link_libraries(terminal QubeGui)
