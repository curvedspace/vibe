set(HEADERS
    VBookmark
    VBookmarkManager
    VFileManagerNavigationModel
    VFilePlacesModel
    VFileSystemModel
)

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeStorage COMPONENT Devel)
