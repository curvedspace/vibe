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
    mauitheme.cpp
)

add_library(maui SHARED ${SOURCES})
target_link_libraries(maui ${Qt5Widgets_LIBRARIES} VibeCore)

install(TARGETS maui LIBRARY
    DESTINATION ${CMAKE_INSTALL_PLUGINSDIR}/platformthemes COMPONENT Framework)
