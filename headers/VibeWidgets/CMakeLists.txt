set(HEADERS
    VAboutDialog
    VCategorizedSortFilterProxyModel
    VCategorizedView
    VCategoryDrawer
    VColorButton
    VBreadcrumbView
    VFilePlacesModel
    VFilePlacesView
    VFileSystemModel
    VibeWidgetsExport
    VLineEdit
    VPageModel
    VPageView
    VPageWidget
    VPageWidgetModel
    VPreferencesModule
    VPreferencesModulePlugin
    VTitleWidget
)

install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeWidgets)
