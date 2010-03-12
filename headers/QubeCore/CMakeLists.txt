project(QubeCoreHeaders)

set(QubeCoreHeaderFiles
    QubeGlobal
    QubeCoreGlobals
    MimeType
)

install(FILES ${QubeCoreHeaderFiles} DESTINATION ${QT_INCLUDE_DIR}/Qube/QubeCore COMPONENT Devel)
