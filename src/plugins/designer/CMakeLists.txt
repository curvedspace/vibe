include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTDESIGNER_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/headers ${CMAKE_BINARY_DIR}/src/kits/core)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN -DQT_NO_DEBUG -DQT_SHARED)

set(SOURCES
    vibewidgets.cpp
    navigationbarplugin.cpp
)

qt4_add_resources(RESOURCES_RCC icons.qrc)

automoc4(VibeWidgetsPlugin SOURCES)
add_library(VibeWidgetsPlugin SHARED ${SOURCES} ${RESOURCES_RCC})

target_link_libraries(VibeWidgetsPlugin ${QT_QTDESIGNER_LIBRARY} VibeGui)

install(TARGETS VibeWidgetsPlugin LIBRARY
    DESTINATION ${QT_PLUGINS_DIR}/designer COMPONENT Devel)
