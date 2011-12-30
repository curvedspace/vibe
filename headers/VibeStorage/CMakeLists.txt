set(HEADERS
    VBookmark
    VBookmarkManager
    VFilePlacesModel
    VFilePlacesView
    VFileSystemModel
)

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeStorage COMPONENT Devel)
