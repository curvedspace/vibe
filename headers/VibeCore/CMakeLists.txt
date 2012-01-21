set(HEADERS
    VAbstractCompressionFilter
    VApplicationInfo
    VArchive
    VArchiveHandler
    VArchiveHandlerPlugin
    VCommandOptions
    VCompressionFilter
    VDesktopFile
    VFileString
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
