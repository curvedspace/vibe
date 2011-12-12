set(HEADERS
    VApplicationInfo
    VCommandOptions
    VDesktopFile
    VGlobal
    VMimeType
    VSaveFile
    VStringHandler
)

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeCore COMPONENT Devel)
