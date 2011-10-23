set(HEADERS
    Global
    Globals
    MimeType
    SaveFile
    Singleton
    StringHandler
)

install(FILES ${HEADERS}
    DESTINATION include/Qube/Core COMPONENT Devel)
