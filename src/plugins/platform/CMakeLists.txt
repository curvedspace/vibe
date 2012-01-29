include_directories(
    ${Qt5Widgets_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_BINARY_DIR}/src/kits/core
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_definitions(${Qt5Widgets_CFLAGS} -DQT_PLUGIN)

set(SOURCES
    vibeplatform.cpp
)

add_library(vibe SHARED ${SOURCES})
set_target_properties(vibe PROPERTIES PREFIX "")
target_link_libraries(vibe ${Qt5Widgets_LIBRARIES})

install(TARGETS vibe LIBRARY
    DESTINATION ${CMAKE_INSTALL_PLUGINSDIR}/gui_platform COMPONENT Framework)
