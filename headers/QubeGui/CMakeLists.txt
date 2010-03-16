project(QubeGuiHeaders)

set(QubeGuiHeaderFiles
    ClickableLabel
    NavBar
    NavBarGroup
    NavBarItem
    SelectableWidget
)

install(FILES ${QubeGuiHeaderFiles} DESTINATION ${QT_INCLUDE_DIR}/Qube/QubeGui COMPONENT Devel)
