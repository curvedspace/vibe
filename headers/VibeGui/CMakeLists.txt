set(HEADERS
    VAboutDialog
    VBreadCrumbBar
    VBreadCrumbDirModel
    VClickableLabel
    VColorScheme
    VCommonStyle
    VInterfaceMacros
    VNavBar
    VNavBarGroup
    VNavBarItem
    VSelectableWidget
    VStyle
    VStyleOptionBreadCrumbBar
)

install(FILES ${HEADERS}
    DESTINATION include/VibeGui COMPONENT Devel)
