set(HEADERS
    VBookmark
    VBookmarkManager
    VFilePlacesModel
    VFindDirectory
)

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeStorage COMPONENT Devel)
