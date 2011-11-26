add_definitions(-DQUBE_STORAGE_LIB)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTXML_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/headers)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SOURCES
    bookmark.cpp
    bookmarkmanager.cpp
    fileplacesmodel.cpp
    fileplacesitem.cpp
    fileplacessharedbookmarks.cpp
    finddirectory.cpp
)

set(HEADERS
    bookmark.h
    bookmarkmanager.h
    fileplacesmodel.h
    finddirectory.h
)

automoc4(VibeStorage SOURCES)
add_library(VibeStorage SHARED ${SOURCES})

target_link_libraries(VibeStorage
    LINK_INTERFACE_LIBRARIES ${QT_QTGUI_LIBRARY} ${QT_QTXML_LIBRARY} VibeCore VibeHardware)
set_target_properties(VibeStorage PROPERTIES
    VERSION ${VIBE_VERSION_STR}
    SOVERSION ${VIBE_VERSION_MAJOR}.${VIBE_VERSION_MINOR}.${VIBE_VERSION_RELEASE})

install(FILES ${HEADERS}
    DESTINATION include/Vibe/Storage COMPONENT Devel)
install(TARGETS VibeStorage
    LIBRARY DESTINATION lib COMPONENT Framework)
