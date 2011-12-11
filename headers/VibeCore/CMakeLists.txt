set(HEADERS
    VApplicationInfo
    VCommandOptions
    VGlobal
    VMimeType
    VSaveFile
    VStringHandler
)

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeCore COMPONENT Devel)
