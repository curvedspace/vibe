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
    lib/terminalcharacterdecoder.cpp
    lib/keyboardtranslator.cpp
    lib/screen.cpp
    lib/history.cpp
    lib/blockarray.cpp
    lib/wcwidth.cpp
    lib/screenwindow.cpp
    lib/emulation.cpp
    lib/vt102emulation.cpp
    lib/terminaldisplay.cpp
    lib/filter.cpp
    lib/pty.cpp
    lib/kpty.cpp
    lib/k3process.cpp
    lib/k3processcontroller.cpp
    lib/session.cpp
    lib/shellcommand.cpp
    lib/termwidget.cpp
)

set(TermWidgetHeaders
    lib/terminalcharacterdecoder.h
    lib/character.h
    lib/charactercolor.h
    lib/keyboardtranslator.h
    lib/extendeddefaulttranslator.h
    lib/screen.h
    lib/history.h
    lib/blockarray.h
    lib/wcwidth.h
    lib/screenwindow.h
    lib/emulation.h
    lib/vt102emulation.h
    lib/terminaldisplay.h
    lib/filter.h
    lib/linefont.h
    lib/pty.h
    lib/kpty.h
    lib/kpty_p.h
    lib/k3process.h
    lib/k3processcontroller.h
    lib/session.h
    lib/shellcommand.h
    lib/termwidget.h
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
