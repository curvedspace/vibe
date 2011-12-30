add_definitions(-DVIBE_BUILDING)

include_directories(
    ${QT_INCLUDE_DIR}
    ${QT_QTGUI_INCLUDE_DIR}
    ${QT_QTDBUS_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_BINARY_DIR}/src/kits/core
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}
)

set(SOURCES
    vbookmark.cpp
    vbookmarkmanager.cpp
    vbookmarkmanageradaptor.cpp
    vfileplacesmodel.cpp
    private/fileplacesitem.cpp
    private/fileplacesviewdelegate.cpp
    vfileplacesview.cpp
    vfilesystemmodel.cpp
)

set(HEADERS
    vbookmark.h
    vbookmarkmanager.h
    vfileplacesmodel.h
    vfileplacesview.h
    vfilesystemmodel.h
)

automoc4(VibeStorage SOURCES)
add_library(VibeStorage SHARED ${SOURCES})
target_link_libraries(VibeStorage
    LINK_INTERFACE_LIBRARIES ${QT_QTGUI_LIBRARIES} ${QT_QTDBUS_LIBRARIES} VibeCore VibeHardware)
set_target_properties(VibeStorage PROPERTIES
    VERSION ${VIBE_VERSION_STR}
    SOVERSION ${VIBE_VERSION_MAJOR}.${VIBE_VERSION_MINOR}.${VIBE_VERSION_RELEASE})

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeStorage COMPONENT Devel)
install(TARGETS VibeStorage LIBRARY
    DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Framework)
