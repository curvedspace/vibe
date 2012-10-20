include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_BINARY_DIR}/src/core
    ${CMAKE_BINARY_DIR}/src/gui
)

add_definitions(${Qt5Core_DEFINITIONS} ${Qt5Gui_DEFINITIONS})

set(SOURCES
    main.cpp
    hawaiitheme.cpp
)

add_library(hawaii SHARED ${SOURCES})
target_link_libraries(hawaii ${Qt5Gui_LIBRARIES} VibeGui)

install(TARGETS hawaii LIBRARY DESTINATION ${CMAKE_INSTALL_PLUGINSDIR}/platformthemes)
