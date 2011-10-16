include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/headers ${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_BINARY_DIR}/src/servers/settings)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN -DQT_NO_DEBUG -DQT_SHARED)

set(QubeGuiPlatformSources
    qubeguiplatform.cpp
)

qt4_automoc(${QubeGuiPlatformSources})

add_library(qube SHARED ${QubeGuiPlatformSources})
target_link_libraries(qube ${QT_QTGUI_LIBRARY} QubeServices)

install(TARGETS qube LIBRARY
    DESTINATION ${QT_PLUGINS_DIR}/gui_platform COMPONENT Framework)
