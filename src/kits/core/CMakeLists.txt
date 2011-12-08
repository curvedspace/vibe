add_definitions(-DVIBE_BUILDING)

include_directories(
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTXML_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_CURRENT_BINARY_DIR}
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/vglobal.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/vglobal.h"
    IMMEDIATE @ONLY)

set(SOURCES
    vcommandoptions.cpp
    vglobals.cpp
    vmimetype.cpp
    vsavefile.cpp
    vstringhandler.cpp
)

set(PUBLIC_HEADERS
    vcommandoptions.h
    vmimetype.h
    vsavefile.h
    vstringhandler.h
    ${CMAKE_CURRENT_BINARY_DIR}/vglobal.h
)

set(RESOURCES
    core.qrc
)

automoc4(VibeCore SOURCES)
qt4_add_resources(RESOURCES_RCC ${RESOURCES})

add_library(VibeCore SHARED ${SOURCES} ${RESOURCES_RCC})
target_link_libraries(VibeCore
    LINK_INTERFACE_LIBRARIES ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY})
set_target_properties(VibeCore PROPERTIES
    VERSION ${VIBE_VERSION_STR}
    SOVERSION ${VIBE_VERSION_MAJOR}.${VIBE_VERSION_MINOR}.${VIBE_VERSION_RELEASE})

install(FILES ${PUBLIC_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeCore COMPONENT Devel)
install(TARGETS VibeCore LIBRARY
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Framework)
