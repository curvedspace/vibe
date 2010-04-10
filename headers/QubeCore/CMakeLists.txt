project(QubeCoreHeaders)

set(QubeCoreHeaderFiles
    QubeGlobal
    QubeCoreGlobals
    MimeType
)

install(FILES ${QubeCoreHeaderFiles}
    DESTINATION ${QUBE_DEVELOPMENT_INCLUDE_DIR}/Qube/QubeCore COMPONENT Devel)
