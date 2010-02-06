project(QubeQtDesignerPlugins)

include_directories(${CMAKE_SOURCE_DIR}/src/core)
include_directories(${CMAKE_SOURCE_DIR}/src/gui)

set(QubeQtDesignerPluginsSources
    qnavbarplugin.cpp
)

qt4_add_resources(icons.qrc)
