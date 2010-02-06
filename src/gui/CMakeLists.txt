project(QubeGui)

add_definitions(-DQUBE_GUI_LIBRARY)

set(QubeGuiSources
	qclickablelabel.cpp
	qnavbar.cpp
	qnavbargroup.cpp
	qnavbaritem.cpp
	qselectablewidget.cpp
)

add_library(QubeGui SHARED ${QubeGuiSources})

set(libraries ${QT_QTGUI_LIBRARY})
target_link_libraries(QubeGui
	LINK_INTERFACE_LIBRARIES ${libraries})
set_target_properties(QubeGui PROPERTIES
	VERSION 0.0.0
	SOVERSION 0.0.0)

install(FILES
	qclickablelabel.h
	qnavbar.h
	qnavbargroup.h
	qnavbaritem.h
	qselectablewidget.h

	QClickableLabel
	QNavBar
	QNavBarGroup
	QNavBarItem
	QSelectableWidget
DESTINATION ${INCLUDE_INSTALL_DIR}/QubeGui COMPONENT Devel)
