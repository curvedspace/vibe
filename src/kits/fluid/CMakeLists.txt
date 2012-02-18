include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Quick_INCLUDE_DIRS}
    ${VIBE_CORE_INCLUDEDIR}
    ${GLESv2_INCLUDEDIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_definitions(-DQT_PLUGIN -DQT_SHARED)

set(SOURCES
    desktopitem.cpp
    fluidplugin.cpp
    iconimageprovider.cpp
    styleitem.cpp
    tooltiparea.cpp
)

set(RESOURCES
    fluid.qrc
)

qt5_add_resources(RESOURCES_RCC ${RESOURCES})

add_library(VibeFluidPlugin SHARED ${SOURCES} ${RESOURCES_RCC})
target_link_libraries(VibeFluidPlugin ${Qt5Quick_LIBRARIES} ${Qt5Widgets_LIBRARIES})

install(TARGETS VibeFluidPlugin
    LIBRARY DESTINATION ${CMAKE_INSTALL_IMPORTSDIR}/VibeFluid)

set(QML_FILES
    qml/qmldir
    qml/Button.qml
    qml/Frame.qml
    qml/TextField.qml
)

set(QML_CUSTOM_FILES
    qml/custom/qmldir
    qml/custom/BasicButton.qml
    qml/custom/ButtonColumn.qml
    qml/custom/ButtonGroup.js
    qml/custom/Button.qml
    qml/custom/ButtonRow.qml
    qml/custom/CheckBox.qml
    qml/custom/GroupBox.qml
    qml/custom/ProgressBar.qml
    qml/custom/Slider.qml
    qml/custom/SpinBox.qml
    qml/custom/Splitter.qml
    qml/custom/TextField.qml

    qml/custom/ButtonBehavior.qml
    qml/custom/ModalPopupBehavior.qml
)

set(QML_PRIVATE_FILES
    qml/private/ButtonShadow.qml
    qml/private/TextFieldFocus.qml
)

install(FILES ${QML_FILES} DESTINATION ${CMAKE_INSTALL_IMPORTSDIR}/VibeFluid)
install(FILES ${QML_CUSTOM_FILES} DESTINATION ${CMAKE_INSTALL_IMPORTSDIR}/VibeFluid/custom)
install(FILES ${QML_PRIVATE_FILES} DESTINATION ${CMAKE_INSTALL_IMPORTSDIR}/VibeFluid/private)
