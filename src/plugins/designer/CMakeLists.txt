include_directories(
    ${Qt5Designer_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_BINARY_DIR}/src/core
    ${CMAKE_BINARY_DIR}/src/widgets
)

add_definitions(
    ${Qt5Designer_DEFINITIONS}
    -DQT_PLUGIN
)

set(SOURCES
    main.cpp
    fileplacesviewplugin.cpp
    lineeditplugin.cpp
    titlewidgetplugin.cpp
)

qt5_add_resources(RESOURCES_RCC icons.qrc)

add_library(VibeDesignerPlugin SHARED ${SOURCES} ${RESOURCES_RCC})
target_link_libraries(VibeDesignerPlugin ${Qt5Designer_LIBRARIES} VibeWidgets)

install(TARGETS VibeDesignerPlugin LIBRARY DESTINATION ${CMAKE_INSTALL_PLUGINSDIR}/designer)
