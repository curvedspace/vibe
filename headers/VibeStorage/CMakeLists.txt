set(HEADERS
    VBookmark
    VBookmarkManager
    VFilePlacesModel
    VFindDirectory
)

install(FILES ${HEADERS}
    DESTINATION include/VibeStorage COMPONENT Devel)
