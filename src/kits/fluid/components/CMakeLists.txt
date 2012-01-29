set(QML_FILES
    qml/qmldir
    qml/Button.qml
    qml/Frame.qml
    qml/TextField.qml
)

set(QML_PRIVATE_FILES
    qml/private/ButtonShadow.qml
    qml/private/TextFieldFocus.qml
)

install(FILES ${QML_FILES} DESTINATION ${CMAKE_INSTALL_IMPORTSDIR}/VibeFluid)
install(FILES ${QML_PRIVATE_FILES} DESTINATION ${CMAKE_INSTALL_IMPORTSDIR}/VibeFluid/private)
