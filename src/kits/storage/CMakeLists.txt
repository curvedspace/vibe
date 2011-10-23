add_definitions(-DQUBE_STORAGE_LIB)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/headers)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SOURCES
    bookmark.cpp
    bookmarkmanager.cpp
    fileplacesmodel.cpp
    fileplacessharedbookmarks.cpp
)

set(HEADERS
    bookmark.h
)

set(MOC_HEADERS
    bookmarkmanager.h
    fileplacesmodel.h
    fileplacesitem_p.h
    fileplacessharedbookmarks_p.h
)

qt4_wrap_cpp(HEADERS_MOC ${MOC_HEADERS})

add_library(QubeStorage SHARED ${SOURCES} ${HEADERS_MOC})

target_link_libraries(QubeStorage
	LINK_INTERFACE_LIBRARIES ${QT_QTGUI_LIBRARY} QubeCore)
set_target_properties(QubeStorage PROPERTIES
	VERSION ${QUBE_VERSION}
	SOVERSION 0.0.0)

install(FILES ${HEADERS} ${MOC_HEADERS}
    DESTINATION include/Qube/Qube/Storage COMPONENT Devel)
install(TARGETS QubeStorage
    LIBRARY DESTINATION lib COMPONENT Framework)
