project(QubeCoreHeaders)

set(QubeCoreHeaderFiles
    QubeGlobal
    MimeType
)

install(FILES ${QubeCoreHeaderFiles} DESTINATION ${QT_INCLUDE_DIR}/Qube/QubeCore COMPONENT Devel)
