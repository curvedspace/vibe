project(QubeServicesHeaders)

set(QubeServicesHeaderFiles
    SettingsInterface
)

install(FILES ${QubeServicesHeaderFiles}
    DESTINATION ${QUBE_DEVELOPMENT_INCLUDE_DIR}/Qube/QubeServices COMPONENT Devel)
