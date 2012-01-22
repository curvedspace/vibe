set(QML_FILES
    qml/qmldir
    qml/TextField.qml
)

install(FILES ${QML_FILES} DESTINATION ${CMAKE_INSTALL_IMPORTSDIR}/VibeFluid)
