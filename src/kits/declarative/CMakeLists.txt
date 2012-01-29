add_definitions(-DVIBE_BUILDING)

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Declarative_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_BINARY_DIR}/src/kits/core
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(SOURCES
    vdeclarativeiconimageprovider.cpp
)

set(HEADERS
    vdeclarativeiconimageprovider.h
)

add_library(VibeDeclarative SHARED ${SOURCES})
target_link_libraries(VibeDeclarative ${Qt5Declarative_LIBRARIES})
set_target_properties(VibeDeclarative PROPERTIES
    VERSION ${VIBE_VERSION_STR}
    SOVERSION ${VIBE_VERSION_MAJOR}.${VIBE_VERSION_MINOR}.${VIBE_VERSION_RELEASE})

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeDeclarative COMPONENT Devel)
install(TARGETS VibeDeclarative LIBRARY
    DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Framework)
