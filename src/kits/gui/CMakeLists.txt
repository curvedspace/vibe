add_definitions(-DQUBE_GUI_LIB)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/headers)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SOURCES
    colorscheme.cpp
    clickablelabel.cpp
    navbar.cpp
    navbargroup.cpp
    navbaritem.cpp
    selectablewidget.cpp
    theme.cpp
)

set(HEADERS
    colorscheme.h
    clickablelabel.h
    navbar.h
    navbargroup.h
    navbaritem.h
    selectablewidget.h
    theme.h
)

qt4_automoc(${SOURCES})

add_library(QubeGui SHARED ${SOURCES})

target_link_libraries(QubeGui
	LINK_INTERFACE_LIBRARIES ${QT_QTGUI_LIBRARY} QubeCore)
set_target_properties(QubeGui PROPERTIES
	VERSION ${QUBE_VERSION}
	SOVERSION 0.0.0)

install(FILES ${HEADERS}
    DESTINATION include/Qube/QubeGui COMPONENT Devel)
install(TARGETS QubeGui
    LIBRARY DESTINATION lib COMPONENT Framework)
