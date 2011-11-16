set(HEADERS
    Bookmark
    BookmarkManager
    FilePlacesModel
    FindDirectory
)

install(FILES ${HEADERS}
    DESTINATION include/Qube/Storage COMPONENT Devel)
