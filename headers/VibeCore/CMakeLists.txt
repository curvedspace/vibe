set(HEADERS
    VCommandOptions
    VGlobal
    VMimeType
    VSaveFile
    VStringHandler
)

install(FILES ${HEADERS}
    DESTINATION include/VibeCore COMPONENT Devel)
