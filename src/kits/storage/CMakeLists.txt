add_definitions(-DVIBE_BUILDING)

include_directories(
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTGUI_INCLUDE_DIR}
    ${QT_QTXML_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_BINARY_DIR}/src/kits/core
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}
)

set(SOURCES
    vbookmark.cpp
    vbookmarkmanager.cpp
    vfileplacesmodel.cpp
    vfileplacesitem.cpp
    vfileplacessharedbookmarks.cpp
    vfinddirectory.cpp
)

set(HEADERS
    vbookmark.h
    vbookmarkmanager.h
    vfileplacesmodel.h
    vfinddirectory.h
)

automoc4(VibeStorage SOURCES)

add_library(VibeStorage SHARED ${SOURCES})
target_link_libraries(VibeStorage
    LINK_INTERFACE_LIBRARIES ${QT_QTGUI_LIBRARY} ${QT_QTXML_LIBRARY} VibeCore VibeHardware)
set_target_properties(VibeStorage PROPERTIES
    VERSION ${VIBE_VERSION_STR}
    SOVERSION ${VIBE_VERSION_MAJOR}.${VIBE_VERSION_MINOR}.${VIBE_VERSION_RELEASE})

install(FILES ${HEADERS}
    DESTINATION include/VibeStorage COMPONENT Devel)
install(TARGETS VibeStorage
    LIBRARY DESTINATION lib COMPONENT Framework)
