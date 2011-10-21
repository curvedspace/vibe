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
    bookmarkmanager.h
    fileplacesmodel.h
)

set(PRIVATE_HEADERS
    fileplacesitem_p.h
    fileplacessharedbookmarks_p.h
)

qt4_wrap_cpp(HEADERS_MOC ${HEADERS})
qt4_wrap_cpp(PRIVATE_HEADERS_MOC ${PRIVATE_HEADERS})

add_library(QubeStorage SHARED ${SOURCES} ${HEADERS_MOC} ${PRIVATE_HEADERS_MOC})

target_link_libraries(QubeStorage
	LINK_INTERFACE_LIBRARIES ${QT_QTGUI_LIBRARY} QubeCore)
set_target_properties(QubeStorage PROPERTIES
	VERSION ${QUBE_VERSION}
	SOVERSION 0.0.0)

install(FILES ${HEADERS}
    DESTINATION include/Qube/QubeStorage COMPONENT Devel)
install(TARGETS QubeStorage
    LIBRARY DESTINATION lib COMPONENT Framework)
