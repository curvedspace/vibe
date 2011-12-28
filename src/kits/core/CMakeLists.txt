add_definitions(-DVIBE_BUILDING ${QT_DEFINITIONS} -Wno-multichar)

include_directories(
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
    vapplicationinfo.cpp
    vcommandoptions.cpp
    vdesktopfile.cpp
    vglobals.cpp
    vmimetype.cpp
    vsavefile.cpp
    vstandarddirectories.cpp
    vstringhandler.cpp

    settings/settingsschema.cpp
    settings/settingsschemaloader.cpp
    settings/vsettings.cpp
)

set(HEADERS
    vapplicationinfo.h
    vcommandoptions.h
    vdesktopfile.h
    vmimetype.h
    vsavefile.h
    vstandarddirectories.h
    vstringhandler.h
    ${CMAKE_CURRENT_BINARY_DIR}/vglobal.h

    settings/vsettings.h
)

set(RESOURCES
    core.qrc
)

automoc4(VibeCore SOURCES)
qt4_add_resources(RESOURCES_RCC ${RESOURCES})

add_library(VibeCore SHARED ${SOURCES} ${RESOURCES_RCC})
target_link_libraries(VibeCore
    ${QT_QTCORE_LIBRARIES}
    ${QT_QTXML_LIBRARIES})
set_target_properties(VibeCore PROPERTIES
    VERSION ${VIBE_VERSION_STR}
    SOVERSION ${VIBE_VERSION_MAJOR}.${VIBE_VERSION_MINOR}.${VIBE_VERSION_RELEASE})

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeCore COMPONENT Devel)
install(TARGETS VibeCore LIBRARY
    DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Framework)
