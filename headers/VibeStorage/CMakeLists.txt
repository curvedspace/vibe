set(HEADERS
    VBookmark
    VBookmarkManager
    VFilePlacesModel
    VFileSystemModel
    VFindDirectory
)

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeStorage COMPONENT Devel)
