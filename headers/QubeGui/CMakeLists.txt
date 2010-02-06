project(QubeGuiHeaders)

install(FILES
	QClickableLabel
	QNavBar
	QNavBarGroup
	QNavBarItem
	QSelectableWidget
DESTINATION ${INCLUDE_INSTALL_DIR}/QubeGui COMPONENT Devel)
