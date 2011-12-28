set(HEADERS
    VAboutDialog
    VBreadCrumbBar
    VBreadCrumbDirModel
    VColorScheme
    VCommonStyle
    VInterfaceMacros
    VNavigationBar
    VPageModel
    VPageView
    VPageWidget
    VPageWidgetModel
    VPreferencesModule
    VPreferencesModulePlugin
    VStyle
    VStyleOptionBreadCrumbBar
    VTitleWidget
)

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeGui COMPONENT Devel)
