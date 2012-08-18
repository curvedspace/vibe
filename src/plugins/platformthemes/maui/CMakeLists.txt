include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_BINARY_DIR}/src/core
)

add_definitions(${Qt5Core_DEFINITIONS} ${Qt5Gui_DEFINITIONS})

set(SOURCES
    main.cpp
    mauitheme.cpp
)

add_library(maui SHARED ${SOURCES})
target_link_libraries(maui ${Qt5Gui_LIBRARIES} VibeGui)

install(TARGETS maui LIBRARY DESTINATION ${CMAKE_INSTALL_PLUGINSDIR}/platformthemes)
