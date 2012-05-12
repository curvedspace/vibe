include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${_qt5Gui_install_prefix}/include/QtGui/5.0.0
    ${_qt5Gui_install_prefix}/include/QtGui/5.0.0/QtGui
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_BINARY_DIR}/src/kits/core
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_definitions(${Qt5Core_DEFINITIONS} ${Qt5Gui_DEFINITIONS})

set(SOURCES
    main.cpp
    mauitheme.cpp
)

add_library(maui SHARED ${SOURCES})
target_link_libraries(maui ${Qt5Gui_LIBRARIES} VibeCore)

install(TARGETS maui LIBRARY
    DESTINATION ${CMAKE_INSTALL_PLUGINSDIR}/platformthemes COMPONENT Framework)
