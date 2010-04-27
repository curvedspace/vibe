project(QubeGui)

add_definitions(-DQUBE_GUI_LIB)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/kits/core ${CMAKE_SOURCE_DIR}/headers/QubeCore)
include_directories(${CMAKE_SOURCE_DIR}/headers/QubeGui ${CMAKE_CURRENT_BINARY_DIR})

set(QubeGuiSources
#    aboutdialog.cpp
    colorscheme.cpp
    clickablelabel.cpp
    navbar.cpp
    navbargroup.cpp
    navbaritem.cpp
    selectablewidget.cpp
    theme.cpp
)

set(QubeGuiHeaders
#    aboutdialog.h
    colorscheme.h
    clickablelabel.h
    navbar.h
    navbargroup.h
    navbaritem.h
    selectablewidget.h
    theme.h
)

set(QubeGuiUis
#    aboutdialog.ui
)

qt4_automoc(${QubeGuiSources})
qt4_wrap_ui(QubeGuiUiFiles ${QubeGuiUis})

add_library(QubeGui SHARED ${QubeGuiSources} ${QubeGuiUiFiles})

target_link_libraries(QubeGui
	LINK_INTERFACE_LIBRARIES ${QT_QTGUI_LIBRARY} QubeCore)
set_target_properties(QubeGui PROPERTIES
	VERSION ${QUBE_VERSION}
	SOVERSION 0.0.0)

install(FILES ${QubeGuiHeaders}
    DESTINATION ${QUBE_DEVELOPMENT_INCLUDE_DIR}/Qube/QubeGui COMPONENT Devel)
install(TARGETS QubeGui LIBRARY
    DESTINATION ${QUBE_SYSTEM_LIBRARY_DIR} COMPONENT Framework)
