project(QubeGui)

add_definitions(-DQUBE_GUI_LIB)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/core ${CMAKE_SOURCE_DIR}/headers/QubeCore)
include_directories(${CMAKE_SOURCE_DIR}/src/gui ${CMAKE_SOURCE_DIR}/headers/QubeGui)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(QubeGuiSources
    colorscheme.cpp
    clickablelabel.cpp
    navbar.cpp
    navbargroup.cpp
    navbaritem.cpp
    selectablewidget.cpp
)

set(QubeGuiHeaders
    colorscheme.h
    clickablelabel.h
    navbar.h
    navbargroup.h
    navbaritem.h
    selectablewidget.h
)

qt4_automoc(${QubeGuiSources})

add_library(QubeGui SHARED ${QubeGuiSources})

target_link_libraries(QubeGui
	LINK_INTERFACE_LIBRARIES ${QT_QTGUI_LIBRARY} QubeCore)
set_target_properties(QubeGui PROPERTIES
	VERSION ${QUBE_VERSION}
	SOVERSION 0.0.0)

install(FILES ${QubeGuiHeaders} DESTINATION ${QT_INCLUDE_DIR}/Qube/QubeGui COMPONENT Devel)
install(TARGETS QubeGui LIBRARY DESTINATION ${QT_LIBRARY_DIR} COMPONENT Framework)
