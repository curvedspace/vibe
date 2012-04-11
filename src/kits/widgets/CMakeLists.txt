add_definitions(-DVIBE_BUILDING)

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_BINARY_DIR}/src/kits/core
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(SOURCES
    vbreadcrumbview.cpp
)

set(HEADERS
    vbreadcrumbview.h
)

add_library(VibeWidgets SHARED ${SOURCES})
target_link_libraries(VibeWidgets ${Qt5Core_LIBRARIES} ${Qt5DBus_LIBRARIES} ${Qt5Gui_LIBRARIES}
    ${Qt5Widgets_LIBRARIES} VibeCore)
set_target_properties(VibeWidgets PROPERTIES
    VERSION ${VIBE_VERSION_STRING}
    SOVERSION ${VIBE_VERSION_MAJOR}.${VIBE_VERSION_MINOR}.${VIBE_VERSION_PATCH})

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeWidgets COMPONENT Devel)
install(TARGETS VibeWidgets LIBRARY
    DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Framework)
