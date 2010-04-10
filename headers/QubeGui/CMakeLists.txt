project(QubeGuiHeaders)

set(QubeGuiHeaderFiles
    ClickableLabel
    NavBar
    NavBarGroup
    NavBarItem
    SelectableWidget
)

install(FILES ${QubeGuiHeaderFiles}
    DESTINATION ${QUBE_DEVELOPMENT_INCLUDE_DIR}/Qube/QubeGui COMPONENT Devel)
