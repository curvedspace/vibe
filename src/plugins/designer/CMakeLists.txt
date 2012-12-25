include_directories(
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_BINARY_DIR}/src/core
    ${CMAKE_BINARY_DIR}/src/widgets
)

add_definitions(-DQT_PLUGIN)

set(SOURCES
    main.cpp
    fileplacesviewplugin.cpp
    lineeditplugin.cpp
    titlewidgetplugin.cpp
)

qt5_add_resources(RESOURCES_RCC icons.qrc)

add_library(VibeDesignerPlugin SHARED ${SOURCES} ${RESOURCES_RCC})
qt5_use_modules(VibeDesignerPlugin Designer)
target_link_libraries(VibeDesignerPlugin VibeWidgets)

install(TARGETS VibeDesignerPlugin LIBRARY DESTINATION ${CMAKE_INSTALL_PLUGINSDIR}/designer)
