add_definitions(-DVIBE_BUILDING ${Qt5DBus_DEFINITIONS} ${Qt5Gui_DEFINITIONS})

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5DBus_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_SOURCE_DIR}/headers/VibeCore
    ${CMAKE_BINARY_DIR}/src/kits/core
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(SOURCES
    vcolorscheme.cpp
    vguiapplication.cpp
    vmainapplicationadaptor.cpp

)

set(HEADERS
    vcolorscheme.h
    vguiapplication.h
)

add_library(VibeGui SHARED ${SOURCES})
target_link_libraries(VibeGui ${Qt5DBus_LIBRARIES} ${Qt5Gui_LIBRARIES} VibeCore)
set_target_properties(VibeGui PROPERTIES
    VERSION ${VIBE_VERSION_STRING}
    SOVERSION ${VIBE_VERSION_MAJOR}.${VIBE_VERSION_MINOR}.${VIBE_VERSION_PATCH})

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeGui COMPONENT Devel)
install(TARGETS VibeGui LIBRARY
    DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Framework)
