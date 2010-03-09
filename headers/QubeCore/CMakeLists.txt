project(QubeCoreHeaders)

set(QubeCoreHeaderFiles
    QubeGlobal
)

install(FILES ${QubeCoreHeaderFiles} DESTINATION ${QT_INCLUDE_DIR}/Qube/QubeCore COMPONENT Devel)
