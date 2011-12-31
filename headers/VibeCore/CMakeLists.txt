set(HEADERS
    VApplicationInfo
    VCommandOptions
    VDesktopFile
    VFileSupport
    VGlobal
    VMimeType
    VSaveFile
    VSettings
    VStandardDirectories
    VStringHandler
)

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeCore COMPONENT Devel)
