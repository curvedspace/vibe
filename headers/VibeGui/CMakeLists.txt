set(HEADERS
    VAboutDialog
    VColorScheme
    VLineEdit
    VMainApplicationAdaptor
    VNavigationBar
    VPageModel
    VPageView
    VPageWidget
    VPageWidgetModel
    VPreferencesModule
    VPreferencesModulePlugin
    VTitleWidget
)

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeGui COMPONENT Devel)
