add_definitions(-DQUBE_CORE_LIB)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTXML_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/headers ${CMAKE_CURRENT_BINARY_DIR})

set(SOURCES
    globals.cpp
    mimetype.cpp
    singleton.h
 )

set(HEADERS
    global.h
    globals.h
    mimetype.h
    singleton.h
)

set(RESOURCES
    core.qrc
)

qt4_wrap_cpp(HEADERS_MOC ${HEADERS})
qt4_add_resources(RESOURCES_RCC ${RESOURCES})

add_library(QubeCore SHARED ${SOURCES} ${HEADERS_MOC} ${RESOURCES_RCC})

target_link_libraries(QubeCore
	LINK_INTERFACE_LIBRARIES ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY})
set_target_properties(QubeCore PROPERTIES
	VERSION ${QUBE_VERSION}
	SOVERSION 0.0.0)

install(FILES ${HEADERS}
    DESTINATION include/Qube/QubeCore COMPONENT Devel)
install(TARGETS QubeCore LIBRARY
    DESTINATION lib COMPONENT Framework)
