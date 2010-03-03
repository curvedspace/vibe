project(QubeQtDesignerPlugins)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/core ${CMAKE_SOURCE_DIR}/headers/QubeCore)
include_directories(${CMAKE_SOURCE_DIR}/src/gui ${CMAKE_SOURCE_DIR}/headers/QubeGui)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN -DQT_SHARED)

set(QubeQtDesignerPluginsSources
    qnavbarplugin.cpp
)

qt4_automoc(${QubeQtDesignerPluginsSources})
qt4_add_resources(QubeQtDesignerPluginResourceFiles icons.qrc)

add_library(QubeQtDesignerPlugins SHARED ${QubeQtDesignerPluginsSources} ${QubeQtDesignerPluginResourceFiles})

target_link_libraries(QubeQtDesignerPlugins)

install(TARGETS QubeQtDesignerPlugins LIBRARY DESTINATION ${QT_PLUGINS_DIR} COMPONENT Devel)
