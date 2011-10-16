set(HEADERS
    ClickableLabel
    NavBar
    NavBarGroup
    NavBarItem
    SelectableWidget
)

install(FILES ${HEADERS}
    DESTINATION include/Qube/QubeGui COMPONENT Devel)
