include_directories(
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTGUI_INCLUDE_DIR}
    ${QCONF_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/src/servers/settings
)

add_definitions(-DQT_PLUGIN -DQT_SHARED)

set(SOURCES
    vibeplatform.cpp
)

automoc4(vibe SOURCES)
add_library(vibe SHARED ${SOURCES})
target_link_libraries(vibe ${QT_QTGUI_LIBRARIES} ${QCONF_LIBRARIES})

install(TARGETS vibe LIBRARY
    DESTINATION ${QT_PLUGINS_DIR}/gui_platform COMPONENT Framework)
