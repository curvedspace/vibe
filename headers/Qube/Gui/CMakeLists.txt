set(HEADERS
    AboutDialog
    ClickableLabel
    ColorScheme
    NavBar
    NavBarGroup
    NavBarItem
    SelectableWidget
)

install(FILES ${HEADERS}
    DESTINATION include/Qube/Gui COMPONENT Devel)
