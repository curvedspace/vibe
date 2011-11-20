set(HEADERS
    AboutDialog
    BreadCrumbBar
    BreadCrumbDirModel
    ClickableLabel
    ColorScheme
    InterfaceMacros
    NavBar
    NavBarGroup
    NavBarItem
    SelectableWidget
    QStyleOptionBreadCrumbBar
)

install(FILES ${HEADERS}
    DESTINATION include/Qube/Gui COMPONENT Devel)
