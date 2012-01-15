set(HEADERS
    VAbstractCompressionFilter
    VApplicationInfo
    VCommandOptions
    VCompressionFilter
    VDesktopFile
    VFileSupport
    VFileSystemWatcher
    VGlobal
    VMimeType
    VSaveFile
    VSettings
    VStandardDirectories
    VStringHandler
)

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeCore COMPONENT Devel)
