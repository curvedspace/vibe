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
    VPageModel
    VPageView
    VPageWidget
    VPageWidgetModel
    VPreferencesModule
    VPreferencesModuleInfo
    VSelectableWidget
    VStyle
    VStyleOptionBreadCrumbBar
    VTitleWidget
)

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeGui COMPONENT Devel)
