project(QubeGui)

add_definitions(-DQUBE_GUI_LIB)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/core ${CMAKE_SOURCE_DIR}/headers/QubeCore)
include_directories(${CMAKE_SOURCE_DIR}/src/gui ${CMAKE_SOURCE_DIR}/headers/QubeGui)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(QubeGuiSources
	qclickablelabel.cpp
	qnavbar.cpp
	qnavbargroup.cpp
	qnavbaritem.cpp
	qselectablewidget.cpp
)

set(QubeGuiHeaders
    qclickablelabel.h
    qnavbar.h
    qnavbargroup.h
    qnavbaritem.h
    qselectablewidget.h
)

qt4_automoc(${QubeGuiSources})

add_library(QubeGui SHARED ${QubeGuiSources})

target_link_libraries(QubeGui
        LINK_INTERFACE_LIBRARIES ${QT_QTGUI_LIBRARY})
set_target_properties(QubeGui PROPERTIES
	VERSION 0.0.0
	SOVERSION 0.0.0)

install(FILES ${QubeGuiHeaders} DESTINATION ${INCLUDE_INSTALL_DIR}/QubeGui COMPONENT Devel)
