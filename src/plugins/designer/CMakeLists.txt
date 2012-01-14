include_directories(
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTGUI_INCLUDE_DIR}
    ${QT_QTDESIGNER_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_BINARY_DIR}/src/kits/core
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN -DQT_NO_DEBUG -DQT_SHARED)

set(SOURCES
    vibewidgets.cpp
    fileplacesviewplugin.cpp
    lineeditplugin.cpp
    navigationbarplugin.cpp
    titlewidgetplugin.cpp
)

qt4_add_resources(RESOURCES_RCC icons.qrc)

automoc4(VibeWidgetsPlugin SOURCES)
add_library(VibeWidgetsPlugin SHARED ${SOURCES} ${RESOURCES_RCC})
target_link_libraries(VibeWidgetsPlugin ${QT_QTDESIGNER_LIBRARIES} VibeGui VibeStorage)

install(TARGETS VibeWidgetsPlugin LIBRARY
    DESTINATION ${QT_PLUGINS_DIR}/designer COMPONENT Devel)
