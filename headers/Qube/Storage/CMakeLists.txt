set(HEADERS
    Bookmark
    BookmarkManager
    FilePlacesModel
)

install(FILES ${HEADERS}
    DESTINATION include/Qube/Storage COMPONENT Devel)
