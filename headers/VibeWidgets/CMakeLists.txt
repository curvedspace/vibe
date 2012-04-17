set(HEADERS
    VAboutDialog
    VBreadcrumbView
    VFilePlacesModel
    VFilePlacesView
    VFileSystemModel
    VLineEdit
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
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeWidgets COMPONENT Devel)
