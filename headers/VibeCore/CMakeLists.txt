set(HEADERS
    VAbstractCompressionFilter
    VAccountsManager
    VApplicationInfo
    VArchive
    VArchiveHandler
    VArchiveHandlerPlugin
    VBookmark
    VBookmarkManager
    VCommandOptions
    VCompressionFilter
    VDesktopFile
    VFileString
    VFileSupport
    VFileSystemWatcher
    VGlobal
    VSaveFile
    VSettings
    VStandardDirectories
    VStringHandler
    VUserAccount
    VUserAccountList
)

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeCore COMPONENT Devel)
