add_definitions(-DQUBE_CORE_LIB)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTXML_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/headers ${CMAKE_CURRENT_BINARY_DIR})

set(SOURCES
    commandoptions.cpp
    globals.cpp
    mimetype.cpp
    savefile.cpp
    stringhandler.cpp
)

set(PUBLIC_HEADERS
    commandoptions.h
    global.h
    globals.h
    mimetype.h
    savefile.h
    singleton.h
    stringhandler.h
)

set(RESOURCES
    core.qrc
)

qt4_add_resources(RESOURCES_RCC ${RESOURCES})

add_library(QubeCore SHARED ${SOURCES} ${RESOURCES_RCC})

target_link_libraries(QubeCore
	LINK_INTERFACE_LIBRARIES ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY})
set_target_properties(QubeCore PROPERTIES
	VERSION ${QUBE_VERSION}
	SOVERSION 0.0.0)

install(FILES ${PUBLIC_HEADERS}
    DESTINATION include/Qube/QubeCore COMPONENT Devel)
install(TARGETS QubeCore LIBRARY
    DESTINATION lib COMPONENT Framework)
