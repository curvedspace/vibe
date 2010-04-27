project(QubeServersHeaders)

set(QubeServersHeaderFiles
    SettingsInterface
)

install(FILES ${QubeServersHeaderFiles}
    DESTINATION ${QUBE_DEVELOPMENT_INCLUDE_DIR}/Qube/QubeServers COMPONENT Devel)
