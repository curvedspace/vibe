project(QubeGui)

add_definitions(-DQUBE_GUI_LIB)

set(QT_USE_QTGUI, TRUE)
include(${QT_USE_FILE})
include_directories(../core ${QT_QTGUI_INCLUDE_DIR})

set(QubeGuiSources
	qclickablelabel.cpp
	qnavbar.cpp
	qnavbargroup.cpp
	qnavbaritem.cpp
	qselectablewidget.cpp
)

qt4_automoc(${QubeGuiSources})

add_library(QubeGui SHARED ${QubeGuiSources})

target_link_libraries(QubeGui
        LINK_INTERFACE_LIBRARIES ${QT_QTGUI_LIBRARY})
set_target_properties(QubeGui PROPERTIES
	VERSION 0.0.0
	SOVERSION 0.0.0)

install(FILES
	qclickablelabel.h
	qnavbar.h
	qnavbargroup.h
	qnavbaritem.h
	qselectablewidget.h
DESTINATION ${INCLUDE_INSTALL_DIR}/QubeGui COMPONENT Devel)
