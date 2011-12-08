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
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeGui COMPONENT Devel)
