project(QubeWidgets)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTDESIGNER_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/kits/core ${CMAKE_SOURCE_DIR}/headers/QubeCore)
include_directories(${CMAKE_SOURCE_DIR}/src/kits/gui ${CMAKE_SOURCE_DIR}/headers/QubeGui)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN -DQT_NO_DEBUG -DQT_SHARED)

set(QubeWidgetsSources
    qubewidgets.cpp
    navbarplugin/navbarplugin.cpp
    #navbarplugin/navbarplugintaskmenu.cpp
    #navbarplugin/navbarplugintaskmenufactory.cpp
    #navbarplugin/navbarplugindialog.cpp
)

qt4_automoc(${QubeWidgetsSources})
qt4_add_resources(QubeWidgetsResourceFiles icons.qrc)

add_library(QubeWidgetsPlugin SHARED ${QubeWidgetsSources} ${QubeWidgetsResourceFiles})

target_link_libraries(QubeWidgetsPlugin ${QT_QTDESIGNER_LIBRARY} QubeGui)

install(TARGETS QubeWidgetsPlugin LIBRARY
    DESTINATION ${QUBE_SYSTEM_PLUGINS_DIR}/designer COMPONENT Devel)
