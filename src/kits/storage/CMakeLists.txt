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

automoc4(QubeStorage SOURCES)
add_library(QubeStorage SHARED ${SOURCES})

target_link_libraries(QubeStorage
	LINK_INTERFACE_LIBRARIES ${QT_QTGUI_LIBRARY} ${QT_QTXML_LIBRARY} QubeCore QubeHardware)
set_target_properties(QubeStorage PROPERTIES
	VERSION ${QUBE_VERSION}
	SOVERSION 0.0.0)

install(FILES ${HEADERS}
    DESTINATION include/Qube/Storage COMPONENT Devel)
install(TARGETS QubeStorage
    LIBRARY DESTINATION lib COMPONENT Framework)
