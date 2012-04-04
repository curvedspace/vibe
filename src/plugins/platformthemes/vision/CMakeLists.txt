include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_BINARY_DIR}/src/kits/core
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_definitions(${Qt5Widgets_CFLAGS})

set(SOURCES
    main.cpp
    visiontheme.cpp
)

add_library(vision SHARED ${SOURCES})
target_link_libraries(vision ${Qt5Widgets_LIBRARIES} VibeCore)

install(TARGETS vision LIBRARY
    DESTINATION ${CMAKE_INSTALL_PLUGINSDIR}/platformthemes COMPONENT Framework)
