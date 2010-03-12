project(Terminal)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/core ${CMAKE_SOURCE_DIR}/headers/QubeCore)
include_directories(${CMAKE_SOURCE_DIR}/src/gui ${CMAKE_SOURCE_DIR}/headers/QubeGui)
include_directories(${CMAKE_SOURCE_DIR}/headers)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(TerminalSources
    mainwindow.cpp
)

set(TerminalHeaders
)

set(TerminalUis
    mainwindow.ui
)

set(TerminalResources
    terminal.qrc
)

qt4_automoc(${TerminalSources})
qt4_wrap_ui(TerminalUiFiles ${TrackerUis})
qt4_add_resources(TerminalResourceFiles ${TrackerResources})

add_executable(terminal ${TerminalSources} ${TrackerUiFiles} ${TrackerResourceFiles})

target_link_libraries(terminal QubeGui TermWidget)
