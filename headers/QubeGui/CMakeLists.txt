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
    DESTINATION include/Qube/QubeGui COMPONENT Devel)
