add_definitions(-DQUBE_GUI_LIB)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/headers)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(PUBLIC_SOURCES
    aboutdialog.cpp
    abstractbreadcrumbmodel.cpp
    breadcrumbbar.cpp
    breadcrumbdirmodel.cpp
    colorscheme.cpp
    clickablelabel.cpp
    dir_helper.cpp
    navbar.cpp
    navbargroup.cpp
    navbaritem.cpp
    selectablewidget.cpp
    qstyleoptionbreadcrumbbar.cpp
    theme.cpp
)

set(PUBLIC_HEADERS
    aboutdialog.h
    breadcrumbbar.h
    breadcrumbdirmodel.h
    clickablelabel.h
    interfacemacros.h
    navbar.h
    navbargroup.h
    navbaritem.h
    selectablewidget.h
)

set(NON_MOC_HEADERS
    abstractbreadcrumbmodel.h
    colorscheme.h
    theme.h
    qstyleoptionbreadcrumbbar.h
)

set(SOURCES
    ${PUBLIC_SOURCES}
    aboutdialog_p.cpp
)

set(HEADERS
    ${PUBLIC_HEADERS}
    aboutdialog_p.h
    breadcrumbbar_ui.h
)

qt4_wrap_cpp(HEADERS_MOC ${HEADERS})

add_library(QubeGui SHARED ${SOURCES} ${HEADERS_MOC})

target_link_libraries(QubeGui
	LINK_INTERFACE_LIBRARIES ${QT_QTGUI_LIBRARY} QubeCore)
set_target_properties(QubeGui PROPERTIES
	VERSION ${QUBE_VERSION}
	SOVERSION 0.0.0)

install(FILES ${PUBLIC_HEADERS} ${NON_MOC_HEADERS}
    DESTINATION include/Qube/Gui COMPONENT Devel)
install(TARGETS QubeGui
    LIBRARY DESTINATION lib COMPONENT Framework)
